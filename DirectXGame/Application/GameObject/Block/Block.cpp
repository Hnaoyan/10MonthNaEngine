#include "Block.h"
#include "Application/GameObject/BlockManager/BlockManager.h"
#include "Application/GameObject/Area/Area.h"

Block::~Block()
{

	if (state_) {
		delete state_;
	}

}

void Block::Initialize(Model* model, BlockState blockState, const Vector3& transfrom, const Vector2& colliderSize, BlockManager* blockManager)
{

	// ワールドトランスフォーム
	worldTransform_.Initialize();
	worldTransform_.translation_ = transfrom;
	worldTransform_.rotation_ = { 0.0f,0.0f,0.0f };
	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransform_.UpdateMatrix();

	// モデル
	model_ = model;

	// 速度
	velocity_ = {0.0f,0.0f};

	// コライダーサイズ
	Vector2 position = {worldTransform_.matWorld_.m[3][0],worldTransform_.matWorld_.m[3][1] };
	collider_.Initialize(&worldTransform_, colliderSize);

	// 状態名
	stateName_ = blockState;

	// ブロックマネージャー
	blockManager_ = blockManager;

	// 状態
	switch (stateName_)
	{
	case kScaffold:
		state_ = new BlockStateScaffold();
		collider_.SetCollisionAttribute(CollisionAttribute::blockScaffold);
		collider_.SetCollisionMask(0xffffffff - CollisionAttribute::blockScaffold);
		// テクスチャハンドル
		textureHandle_ = blockManager_->GetTextureHandles().at(BlockState::kScaffold);
		break;

	case kEnemyAttack:
		state_ = new BlockStateEnemyAttack();
		collider_.SetCollisionAttribute(CollisionAttribute::blockScaffold);
		collider_.SetCollisionMask(0xffffffff - CollisionAttribute::blockScaffold);
		// テクスチャハンドル
		textureHandle_ = blockManager_->GetTextureHandles().at(BlockState::kEnemyAttack);
		break;

	default:
		break;
	}

	state_->Initialize(this);
	collider_.SetFunction(collisionFunction_);

	// 死亡フラグ
	isDead_ = false;

	// 親か
	isParent_ = true;

}

void Block::Update()
{

	//行動
	state_->Update();

	worldTransform_.UpdateMatrix();
	
	Vector2 position = { worldTransform_.matWorld_.m[3][0],worldTransform_.matWorld_.m[3][1] };
	collider_.Update(position);

}

void Block::Draw(const ViewProjection& viewProjection)
{

	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}

void Block::ChangeState(BlockState blockstate)
{

	uint32_t mask = 0xffffffff;

	if (state_) {
		delete state_;
		stateName_ = blockstate;
		switch (stateName_)
		{
		case kScaffold:
			state_ = new BlockStateScaffold();
			collider_.SetCollisionAttribute(CollisionAttribute::blockScaffold);
			mask -= CollisionAttribute::blockScaffold + CollisionAttribute::blockScaffoldColor + CollisionAttribute::blockPlayerAttack;
			collider_.SetCollisionMask(mask);
			// テクスチャハンドル
			textureHandle_ = blockManager_->GetTextureHandles().at(BlockState::kScaffold);
			break;

		case kScaffoldColor:
			state_ = new BlockStateScaffoldColor();
			collider_.SetCollisionAttribute(CollisionAttribute::blockScaffoldColor);
			collider_.SetCollisionMask(mask - CollisionAttribute::blockScaffoldColor);
			// テクスチャハンドル
			textureHandle_ = blockManager_->GetTextureHandles().at(BlockState::kScaffoldColor);
			break;

		case kPlayerAttack:
			state_ = new BlockStatePlayerAttack();
			collider_.SetCollisionAttribute(CollisionAttribute::blockPlayerAttack);
			collider_.SetCollisionMask(mask - CollisionAttribute::blockPlayerAttack);
			// テクスチャハンドル
			textureHandle_ = blockManager_->GetTextureHandles().at(BlockState::kPlayerAttack);
			break;

		case kEnemyAttack:
			state_ = new BlockStateEnemyAttack();
			collider_.SetCollisionAttribute(CollisionAttribute::blockEnemyAttack);
			collider_.SetCollisionMask(mask - CollisionAttribute::blockEnemyAttack);
			// テクスチャハンドル
			textureHandle_ = blockManager_->GetTextureHandles().at(BlockState::kEnemyAttack);
			break;

		default:
			break;
		}

		state_->Initialize(this);
		collider_.SetFunction(collisionFunction_);
	}

}

void Block::ScaffoldRise()
{

	// 足場以外はreturn
	if (stateName_ == kPlayerAttack || stateName_ == kEnemyAttack) {
		return;
	}

	worldTransform_.translation_.y += collider_.GetSize().y;
	worldTransform_.UpdateMatrix();

}

void Block::OnCollision(uint32_t collisonObj, WorldTransform* worldTransform)
{

	collisionFunction_(collisonObj, worldTransform);

}

void BlockStateScaffold::Initialize(Block* pBlock)
{

	// ポインタ
	pBlock_ = pBlock;

	//コールバック設定
	std::function<void(uint32_t, WorldTransform*)> f = std::function<void(uint32_t, WorldTransform*)>(std::bind(&BlockStateScaffold::OnCollision, this, std::placeholders::_1, std::placeholders::_2));
	pBlock_->SetCollisionFunction(f);

}

void BlockStateScaffold::Update()
{
}

void BlockStateScaffold::OnCollision(uint32_t collisonObj, WorldTransform* worldTransform)
{

	//ステートを色足場に変更
	if (collisonObj & CollisionAttribute::player) {
		pBlock_->ChangeState(BlockState::kScaffoldColor);
	}

	worldTransform;

}

void BlockStateScaffoldColor::Initialize(Block* pBlock)
{

	// ポインタ
	pBlock_ = pBlock;

	//コールバック設定
	std::function<void(uint32_t, WorldTransform*)> f = std::function<void(uint32_t, WorldTransform*)>(std::bind(&BlockStateScaffoldColor::OnCollision, this, std::placeholders::_1, std::placeholders::_2));
	pBlock_->SetCollisionFunction(f);

}

void BlockStateScaffoldColor::Update()
{
}

void BlockStateScaffoldColor::OnCollision(uint32_t collisonObj, WorldTransform* worldTransform)
{
	collisonObj;
	worldTransform;
}

void BlockStatePlayerAttack::Initialize(Block* pBlock)
{

	// ポインタ
	pBlock_ = pBlock;

	//コールバック設定
	std::function<void(uint32_t, WorldTransform*)> f = std::function<void(uint32_t, WorldTransform*)>(std::bind(&BlockStatePlayerAttack::OnCollision, this, std::placeholders::_1, std::placeholders::_2));
	pBlock_->SetCollisionFunction(f);

}

void BlockStatePlayerAttack::Update()
{

	WorldTransform worldTransform = pBlock_->GetWorldTransform();
	worldTransform.translation_.y += pBlock_->GetVelocity().y;
	pBlock_->SetWorldTransform(worldTransform);

	if (worldTransform.matWorld_.m[3][1] >= pBlock_->GetBlockManager()->GetArea()->kBlockDeadLine_) {
		pBlock_->SetIsDead(true);
	}

}

void BlockStatePlayerAttack::OnCollision(uint32_t collisonObj, WorldTransform* worldTransform)
{
	collisonObj;
	worldTransform;
}

void BlockStateEnemyAttack::Initialize(Block* pBlock)
{
	// ポインタ
	pBlock_ = pBlock;

	//コールバック設定
	std::function<void(uint32_t, WorldTransform*)> f = std::function<void(uint32_t, WorldTransform*)>(std::bind(&BlockStateEnemyAttack::OnCollision, this, std::placeholders::_1, std::placeholders::_2));
	pBlock_->SetCollisionFunction(f);

}

void BlockStateEnemyAttack::Update()
{
}

void BlockStateEnemyAttack::OnCollision(uint32_t collisonObj, WorldTransform* worldTransform)
{
	collisonObj;
	worldTransform;
}
