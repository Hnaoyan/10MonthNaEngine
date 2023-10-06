#include "Block.h"

Block::~Block()
{

	if (state_) {
		delete state_;
	}

}

void Block::Initialize(Model* model, BlockState blockstate, const Vector3& transfrom, const Vector2& colliderSize)
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

	// 状態
	switch (blockstate)
	{
	case kScaffold:
		state_ = new BlockStateScaffold();
		collider_.SetCollisionAttribute(CollisionAttribute::blockScaffold);
		collider_.SetCollisionMask(0xffffffff - CollisionAttribute::blockScaffold);
		break;
	case kEnemyAttack:
		state_ = new BlockStateEnemyAttack();
		collider_.SetCollisionAttribute(CollisionAttribute::blockScaffold);
		collider_.SetCollisionMask(0xffffffff - CollisionAttribute::blockScaffold);
		break;
	default:
		break;
	}
	state_->Initialize(this);
	collider_.SetFunction(collisionFunction_);

	// 状態名
	stateName_ = blockstate;

	// 死亡フラグ
	isDead_ = false;

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

	model_->Draw(worldTransform_, viewProjection);

}

void Block::ChangeState(BlockState blockstate)
{

	if (state_) {
		delete state_;
		switch (blockstate)
		{
		case kScaffold:
			state_ = new BlockStateScaffold();
			collider_.SetCollisionAttribute(CollisionAttribute::blockScaffold);
			collider_.SetCollisionMask(0xffffffff - CollisionAttribute::blockScaffold);
			break;
		case kScaffoldColor:
			state_ = new BlockStateScaffoldColor();
			collider_.SetCollisionAttribute(CollisionAttribute::blockScaffoldColor);
			collider_.SetCollisionMask(0xffffffff - CollisionAttribute::blockScaffoldColor);
			break;
		case kPlayerAttack:
			state_ = new BlockStatePlayerAttack();
			collider_.SetCollisionAttribute(CollisionAttribute::blockPlayerAttack);
			collider_.SetCollisionMask(0xffffffff - CollisionAttribute::blockPlayerAttack);
			break;
		case kEnemyAttack:
			state_ = new BlockStateEnemyAttack();
			collider_.SetCollisionAttribute(CollisionAttribute::blockEnemyAttack);
			collider_.SetCollisionMask(0xffffffff - CollisionAttribute::blockEnemyAttack);
			break;
		default:
			break;
		}
		state_->Initialize(this);
		collider_.SetFunction(collisionFunction_);
		stateName_ = blockstate;
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
	std::function<void(uint32_t, WorldTransform*)> f = std::function<void(uint32_t, WorldTransform*)>(std::bind(&BlockStateScaffold::OnCollision, this, std::placeholders::_1));
	pBlock_->SetCollisionFunction(f);

}

void BlockStateScaffold::Update()
{
}

void BlockStateScaffold::OnCollision(uint32_t collisonObj)
{
	collisonObj;
}

void BlockStateScaffoldColor::Initialize(Block* pBlock)
{

	// ポインタ
	pBlock_ = pBlock;

	//コールバック設定
	std::function<void(uint32_t, WorldTransform*)> f = std::function<void(uint32_t, WorldTransform*)>(std::bind(&BlockStateScaffoldColor::OnCollision, this, std::placeholders::_1));
	pBlock_->SetCollisionFunction(f);

}

void BlockStateScaffoldColor::Update()
{
}

void BlockStateScaffoldColor::OnCollision(uint32_t collisonObj)
{
	collisonObj;
}

void BlockStatePlayerAttack::Initialize(Block* pBlock)
{

	// ポインタ
	pBlock_ = pBlock;

	//コールバック設定
	std::function<void(uint32_t, WorldTransform*)> f = std::function<void(uint32_t, WorldTransform*)>(std::bind(&BlockStatePlayerAttack::OnCollision, this, std::placeholders::_1));
	pBlock_->SetCollisionFunction(f);

}

void BlockStatePlayerAttack::Update()
{
}

void BlockStatePlayerAttack::OnCollision(uint32_t collisonObj)
{
	collisonObj;
}

void BlockStateEnemyAttack::Initialize(Block* pBlock)
{
	// ポインタ
	pBlock_ = pBlock;

	//コールバック設定
	std::function<void(uint32_t, WorldTransform*)> f = std::function<void(uint32_t, WorldTransform*)>(std::bind(&BlockStateEnemyAttack::OnCollision, this, std::placeholders::_1));
	pBlock_->SetCollisionFunction(f);

}

void BlockStateEnemyAttack::Update()
{
}

void BlockStateEnemyAttack::OnCollision(uint32_t collisonObj)
{
	collisonObj;
}
