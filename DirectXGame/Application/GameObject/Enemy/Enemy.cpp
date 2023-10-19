#include "Enemy.h"
#include "Others/MapSystem/MapSystem.h"

void Enemy::Initialize(Model* model, const Vector2& position)
{

	// ワールドトランスフォーム
	worldTransform_.Initialize();

	// モデル
	model_ = model;

#pragma region 調整項目クラス
	// 調整項目クラスのインスタンス取得
	//GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名設定
	//const char* groupName = "Enemy";
	// 指定した名前でグループ追加
	//globalVariables->CreateGroup(groupName);

	// メンバ変数の調整したい項目をグローバル変数に追加

	ApplyGlobalVariables();

#pragma endregion

	Setting(position);

}

void Enemy::Update(const Vector2& position)
{

#ifdef _DEBUG
	ApplyGlobalVariables();
#endif // _DEBUG

	position_ = position;
	worldTransform_.translation_ = { position.x * MapSystem::kSquareSize_.x, position.y * MapSystem::kSquareSize_.y, -10.0f };
	worldTransform_.UpdateMatrix();

}

void Enemy::Draw(const ViewProjection& viewProjection)
{

	model_->Draw(worldTransform_, viewProjection);

}

void Enemy::Setting(const Vector2& position)
{

	// ワールドトランスフォーム
	worldTransform_.translation_ = { position.x * MapSystem::kSquareSize_.x, position.y * MapSystem::kSquareSize_.y, -10.0f };
	worldTransform_.rotation_ = { -1.5f,0.0f,0.0f };
	worldTransform_.scale_ = { 10.0f,10.0f,10.0f };
	worldTransform_.UpdateMatrix();

}

void Enemy::ApplyGlobalVariables()
{
}
