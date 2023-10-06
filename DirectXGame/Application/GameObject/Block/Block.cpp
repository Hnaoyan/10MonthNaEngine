#include "Block.h"

Block::~Block()
{

	if (state) {
		delete state;
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
	colliderSize_ = colliderSize;

	// 状態
	switch (blockstate)
	{
	case kScaffold:
		state = new BlockStateScaffold();
		break;
	case kEnemyAttack:
		state = new BlockStateEnemyAttack();
		break;
	default:
		break;
	}
	state->Initialize(this);

	// 状態名
	stateName = blockstate;

	// 死亡フラグ
	isDead_ = false;

}

void Block::Update()
{

	//行動
	state->Update(this);

	worldTransform_.UpdateMatrix();

}

void Block::Draw(const ViewProjection& viewProjection)
{

	model_->Draw(worldTransform_, viewProjection);

}

void Block::ChangeState(BaseBlockState* newState)
{

	if (state) {
		delete state;
		state = newState;
		state->Initialize(this);
	}

}

void Block::ScaffoldRise()
{

	// 足場以外はreturn
	if (stateName == kPlayerAttack || stateName == kEnemyAttack) {
		return;
	}

	worldTransform_.translation_.y += colliderSize_.y;
	worldTransform_.UpdateMatrix();

}

void BlockStateScaffold::Initialize(Block* pBlock)
{
	pBlock;
}

void BlockStateScaffold::Update(Block* pBlock)
{
	pBlock;
}

void BlockStateScaffoldColor::Initialize(Block* pBlock)
{
	pBlock;
}

void BlockStateScaffoldColor::Update(Block* pBlock)
{
	pBlock;
}

void BlockStatePlayerAttack::Initialize(Block* pBlock)
{
	pBlock;
}

void BlockStatePlayerAttack::Update(Block* pBlock)
{
	pBlock;
}

void BlockStateEnemyAttack::Initialize(Block* pBlock)
{
	pBlock;
}

void BlockStateEnemyAttack::Update(Block* pBlock)
{
	pBlock;
}