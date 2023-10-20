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

void Enemy::Update(const Vector2& position, bool enemyAwake)
{

#ifdef _DEBUG
	ApplyGlobalVariables();
#endif // _DEBUG

	position_ = position;
	float positionZ = 0.0f;
	if (enemyAwake) {
		positionZ = -15.0f;
	}
	else {
		positionZ = -10.0f;
	}

	worldTransform_.translation_ = { position.x * MapSystem::kSquareSize_.x, position.y * MapSystem::kSquareSize_.y, positionZ };
	worldTransform_.rotation_.z = (-1.57f) + rotate_;
	worldTransform_.UpdateMatrix();

}

void Enemy::Draw(const ViewProjection& viewProjection)
{

	model_->Draw(worldTransform_, viewProjection);

}

void Enemy::Setting(const Vector2& position)
{

	// ワールドトランスフォーム
	worldTransform_.translation_ = { position.x * MapSystem::kSquareSize_.x, position.y * MapSystem::kSquareSize_.y, -2.0f };
	rotate_ = -1.57f;
	worldTransform_.rotation_ = { -1.5f,0.0f,rotate_ };
	worldTransform_.scale_ = { 8.0f,8.0f,8.0f };
	worldTransform_.UpdateMatrix();

}

void Enemy::ApplyGlobalVariables()
{
}
