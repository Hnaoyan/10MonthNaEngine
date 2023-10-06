#include "Area.h"

void Area::Initialize(Model* model)
{

	// ワールドトランスフォーム
	worldTransform_.Initialize();

	// モデル
	model_ = model;

	Setting();

}

void Area::Update()
{

	worldTransform_.UpdateMatrix();

}

void Area::Draw(const ViewProjection& viewProjection)
{

	model_->Draw(worldTransform_, viewProjection);

}

void Area::Setting()
{

	worldTransform_.translation_ = { 0.0f,0.0f,0.0f };
	worldTransform_.rotation_ = { 0.0f,0.0f,0.0f };
	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransform_.UpdateMatrix();

}
