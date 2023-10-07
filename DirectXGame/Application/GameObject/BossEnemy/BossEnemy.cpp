#include "BossEnemy.h"
#include "Application/GameObject/BlockManager/BlockManager.h"

BossEnemy::~BossEnemy()
{
	
	//timedCall_の解放
	for (TimedCall* timedCall : timedCalls_) {
		delete timedCall;
	}

}

void BossEnemy::Initialize(Model* model, BlockManager* blockManager)
{

	// ワールドトランスフォーム
	worldTransform_.Initialize();

	// モデル
	model_ = model;

	// ブロックマネージャー
	blockManager_ = blockManager;

	// エネミーの攻撃関数
	enemyAttackFunc_ = std::function<void()>(std::bind(&BlockManager::EnemyAttackBlockGenerate, blockManager_));

	Setting();

	//コールバック設定
	std::function<void(uint32_t, WorldTransform*)> f = std::function<void(uint32_t, WorldTransform*)>(std::bind(&BossEnemy::OnCollision, this, std::placeholders::_1, std::placeholders::_2));
	collider_.SetFunction(f);

}

void BossEnemy::Update()
{

	for (TimedCall* timedCall : timedCalls_) {
		timedCall->Update();
	}

	worldTransform_.UpdateMatrix();

	Vector2 position = { worldTransform_.matWorld_.m[3][0],worldTransform_.matWorld_.m[3][1] };
	collider_.Update(position);

}

void BossEnemy::Draw(const ViewProjection& viewProjection)
{

	model_->Draw(worldTransform_, viewProjection);

}

void BossEnemy::Setting()
{

	// ワールドトランスフォーム
	worldTransform_.translation_ = { 10.0f,39.0f, -1.0f };
	worldTransform_.rotation_ = { 0.0f,0.0f,0.0f };
	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransform_.UpdateMatrix();

	// コライダー
	collider_.Initialize(&worldTransform_, Vector2(20.0f, 2.0f));

	// HP
	hp_ = kInitialHp_;

	//timedCallの解放
	timedCalls_.remove_if([](TimedCall* timedCall) {
		delete timedCall;
		return true;
	});

	// 死亡フラグ
	isDead_ = false;

	// 仮置き
	SetDropBlockTimer();

}

void BossEnemy::OnCollision(uint32_t collisonObj, WorldTransform* worldTransform)
{
	if (collisonObj == BlockState::kPlayerAttack) {
		Damage();
	}
	worldTransform;

}

void BossEnemy::DropBlock()
{

	enemyAttackFunc_();
	SetDropBlockTimer();

}

void BossEnemy::SetDropBlockTimer()
{
	timedCalls_.push_back(new TimedCall(std::bind(&BossEnemy::DropBlock, this), kDropBlockInterval_));
}

void BossEnemy::Damage()
{

	hp_ -=  blockManager_->GetBossDamage();
	if (hp_ <= 0) {
		isDead_ = false;
	}

}
