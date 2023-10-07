#include "BlockManager.h"

#include "Application/GameObject/Area/Area.h"
#include <GlobalVariables.h>

BlockManager::~BlockManager()
{

	//block_の解放
	for (Block* block : blocks_) {
		delete block;
	}
	//timedCall_の解放
	for (TimedCall* timedCall : timedCalls_) {
		delete timedCall;
	}
	//playerAttacks_の解放
	for (PlayerAttack* playerAttack : playerAttacks_) {
		delete playerAttack;
	}

}

void BlockManager::Initialize(Model* model, std::vector<uint32_t> textureHandles)
{

	// モデル
	model_ = model;

	// テクスチャハンドル
	textureHandles_ = textureHandles;

#pragma region 調整項目クラス
	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名設定
	const char* groupName = "Block";
	// 指定した名前でグループ追加
	globalVariables->CreateGroup(groupName);

	// メンバ変数の調整したい項目をグローバル変数に追加

	globalVariables->AddItem(groupName, "kBaseFireBlockSpeed_", kBaseFireBlockSpeed_);
	globalVariables->AddItem(groupName, "kBaseScaffoldBlockGenerateInterval_", kBaseScaffoldBlockGenerateInterval_);
	globalVariables->AddItem(groupName, "kColliderSize_", kColliderSize_);

	ApplyGlobalVariables();

#pragma endregion

	Setting();

}

void BlockManager::Update()
{

#ifdef _DEBUG
	ApplyGlobalVariables();
#endif // _DEBUG

	for (Block* block : blocks_) {
		block->Update();
	}

	for (TimedCall* timedCall : timedCalls_) {
		timedCall->Update();
	}

	for (PlayerAttack* playerAttack : playerAttacks_) {
		playerAttack->Update();
	}

	// 保存データをNULLに 
	playerAttackUnionData_ = nullptr;

}

void BlockManager::Draw(const ViewProjection& viewProjection)
{
	for (Block* block : blocks_) {
		block->Draw(viewProjection);
	}

}

void BlockManager::Setting()
{

	//blockの解放
	blocks_.remove_if([](Block* block) {
		delete block;
		return true;
	});

	//timedCallの解放
	timedCalls_.remove_if([](TimedCall* timedCall) {
		delete timedCall;
		return true;
	});

	//playerAttackの解放
	playerAttacks_.remove_if([](PlayerAttack* playerAttack) {
		delete playerAttack;
		return true;
	});

	//足場ブロック生成インターバル
	scaffoldBlockGenerateInterval_ = kBaseScaffoldBlockGenerateInterval_;

	// 発射されているブロック数
	fireBlockCount_ = 0;

	SetScaffoldBlockGenerateTimer();

}

void BlockManager::DeleteBlock()
{

	//block_の解放
	blocks_.remove_if([](Block* block) {
		if (block->IsDead()) {
			delete block;
			return true;
		}
		return false;
	});

	//playerAttacks_の解放
	playerAttacks_.remove_if([](PlayerAttack* playerAttack){
		if (playerAttack->IsDead()) {
			delete playerAttack;
			return true;
		}
		return false;
	});

}

void BlockManager::BlockFiring()
{

	std::vector<Block*> fireBlocks;

	//とぶか?
	for (Block* block : blocks_) {
		if (block->GetStateName() == BlockState::kScaffoldColor) {
			fireBlockCount_++;
			fireBlocks.push_back(block);
		}
	}

	// プレイヤーのアタック関数
	std::list<PlayerAttack*> playerAttacks;

	//発射処理
	for (Block* block : fireBlocks) {
		block->ChangeState(BlockState::kPlayerAttack);

		// 所属したか
		bool isJoin = false;
		for (PlayerAttack* playerAttack : playerAttacks) {
			if (playerAttack->GetParentBlock()->GetWorldTransform().translation_.y
				== block->GetWorldTransform().translation_.y) {
				playerAttack->AddBlockList(block);
				Vector2 speed = { 0.0f, playerAttack->GetParentBlock()->GetVelocity().y + kBaseFireBlockSpeed_ };
				playerAttack->GetParentBlock()->SetVelocity(speed);
				isJoin = true;
				break;
			}
		}

		// してない(新しいアタックオブジェクト生成)
		if (!isJoin) {
			PlayerAttack* playerAttack = new PlayerAttack();
			playerAttack->Initialize(block);
			playerAttack->SetParentBlock(block);
			Vector2 speed = { 0.0f , kBaseFireBlockSpeed_ };
			block->SetVelocity(speed);
			playerAttacks.push_back(playerAttack);
		}
	}

	for (PlayerAttack* playerAttack : playerAttacks) {
		playerAttacks_.push_back(playerAttack);
	}

}

void BlockManager::PlayerAttackUnion(PlayerAttack* playerAttackUnionData)
{

	if (playerAttackUnionData == playerAttackUnionData_) {
		return;
	}

	// どちらの親が下にあるか
	if (playerAttackUnionData->GetParentBlock()->GetWorldTransform().translation_.y >
		playerAttackUnionData_->GetParentBlock()->GetWorldTransform().translation_.y) {

		//速度
		playerAttackUnionData_->GetParentBlock()->SetVelocity(playerAttackUnionData->GetParentBlock()->GetVelocity());
		playerAttackUnionData->GetParentBlock()->SetVelocity(Vector2(0.0f, 0.0f));
		// 合体
		playerAttackUnionData_->AddBlockList(playerAttackUnionData->GetBlock());
		WorldTransform* a = playerAttackUnionData_->GetParentBlock()->GetWorldTransformAddress();
		playerAttackUnionData->GetParentBlock()->ParentChange(a);


		// 合体したPlayerAttackを死亡判定に
		playerAttackUnionData->SetIsDead(true);

	}
	else{

		//速度
		playerAttackUnionData->GetParentBlock()->SetVelocity(playerAttackUnionData_->GetParentBlock()->GetVelocity());
		playerAttackUnionData_->GetParentBlock()->SetVelocity(Vector2(0.0f,0.0f));
		// 合体
		playerAttackUnionData->AddBlockList(playerAttackUnionData_->GetBlock());
		WorldTransform* a = playerAttackUnionData->GetParentBlock()->GetWorldTransformAddress();
		playerAttackUnionData_->GetParentBlock()->ParentChange(a);

		// 合体したPlayerAttackを死亡判定に
		playerAttackUnionData_->SetIsDead(true);

	}

	// 保存データをNULLに 
	playerAttackUnionData_ = nullptr;

}

void BlockManager::EnemyAttackBlockGenerate()
{

	Block* block = new Block();
	Vector3 transform =
	{ kColliderSize_.x / 2.0f,
		(static_cast<size_t>(area_->kMap_.y) - 1) * kColliderSize_.y + kColliderSize_.y / 2.0f, 0.0f };
	block->Initialize(model_, BlockState::kEnemyAttack, transform, kColliderSize_, this);
	block->SetVelocity(Vector2( 0.0f, kBaseFallBlockSpeed_));
	blocks_.push_back(block);

}

void BlockManager::EnemyAttackPlayerAttackChange(Block* block)
{
	PlayerAttack* playerAttack = new PlayerAttack();
	playerAttack->Initialize(block);
	playerAttack->SetParentBlock(block);
	Vector2 speed = { 0.0f , kBaseFireBlockSpeed_ };
	block->SetVelocity(speed);
	playerAttacks_.push_back(playerAttack);

}

void BlockManager::ScaffoldBlockGenerate()
{
	for (Block* block : blocks_) {
		block->ScaffoldRise();
	}

	for (size_t i = 0; i < static_cast<size_t>(area_->kMap_.x); i++) {
		Block* block = new Block();
		Vector3 transform =
		{ kColliderSize_.x / 2.0f + kColliderSize_.x * static_cast<float>(i),
			kColliderSize_.y / 2.0f, 0.0f };
		block->Initialize(model_, BlockState::kScaffold, transform, kColliderSize_, this);
		blocks_.push_back(block);
	}

	SetScaffoldBlockGenerateTimer();

}

void BlockManager::SetScaffoldBlockGenerateTimer()
{
	timedCalls_.push_back(new TimedCall(std::bind(&BlockManager::ScaffoldBlockGenerate, this), scaffoldBlockGenerateInterval_));
}

void BlockManager::ApplyGlobalVariables()
{

	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "Block";

	kBaseFireBlockSpeed_ = globalVariables->GetFloatValue(groupName, "kBaseFireBlockSpeed_");
	kBaseScaffoldBlockGenerateInterval_ = globalVariables->GetIntValue(groupName, "kBaseScaffoldBlockGenerateInterval_");
	kColliderSize_ = globalVariables->GetVector2Value(groupName, "kColliderSize_");

}
