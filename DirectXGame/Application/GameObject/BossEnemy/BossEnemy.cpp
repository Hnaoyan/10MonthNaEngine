#include "BossEnemy.h"

void BossEnemy::Initialize(Model* model)
{

	// ワールドトランスフォーム
	worldTransform_.Initialize();

	// モデル
	model_ = model;

	Setting();

}

void BossEnemy::Update()
{

	worldTransform_.UpdateMatrix();

}

void BossEnemy::Draw(const ViewProjection& viewProjection)
{

	model_->Draw(worldTransform_, viewProjection);

}

void BossEnemy::Setting()
{

	// ワールドトランスフォーム
	worldTransform_.translation_ = { 0.0f,0.0f,0.0f };
	worldTransform_.rotation_ = { 0.0f,0.0f,0.0f };
	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransform_.UpdateMatrix();

}
