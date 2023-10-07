#include "BlockManager.h"

#include "Application/GameObject/Area/Area.h"

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

}

void BlockManager::Initialize(Model* model, std::vector<uint32_t> textureHandles)
{

	// モデル
	model_ = model;

	// テクスチャハンドル
	textureHandles_ = textureHandles;

	Setting();

}

void BlockManager::Update()
{

	for (Block* block : blocks_) {
		block->Update();
	}

	for (TimedCall* timedCall : timedCalls_) {
		timedCall->Update();
	}

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


	// コライダーサイズ
	colliderSize_ = { 2.0f, 2.0f };

	//足場ブロック生成インターバル
	scaffoldBlockGenerateInterval_ = 300;

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

}

void BlockManager::BlockFiring()
{

	Vector2 speed = { 0.0f , 0.2f };

	for (Block* block : blocks_) {
		if (block->GetStateName() == BlockState::kScaffoldColor) {
			block->ChangeState(BlockState::kPlayerAttack);
			block->SetVelocity(speed);
		}
	}

}

void BlockManager::ScaffoldBlockGenerate()
{
	for (Block* block : blocks_) {
		block->ScaffoldRise();
	}

	for (size_t i = 0; i < static_cast<size_t>(area_->kMap_.x); i++) {
		Block* block = new Block();
		Vector3 transform =
		{ colliderSize_.x / 2.0f + colliderSize_.x * static_cast<float>(i),
			colliderSize_.y / 2.0f, 0.0f };
		block->Initialize(model_, BlockState::kScaffold, transform, colliderSize_, this);
		blocks_.push_back(block);
	}

	SetScaffoldBlockGenerateTimer();

}

void BlockManager::SetScaffoldBlockGenerateTimer()
{
	timedCalls_.push_back(new TimedCall(std::bind(&BlockManager::ScaffoldBlockGenerate, this), scaffoldBlockGenerateInterval_));
}

void BlockManager::EnemyAttackBlockGenerate()
{
}
