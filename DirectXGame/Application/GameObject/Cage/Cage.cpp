#include "Cage.h"
#include "Others/MapSystem/MapSystem.h"

void Cage::Initialize(Model* model, const Vector2& position)
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

void Cage::Update()
{

#ifdef _DEBUG
	ApplyGlobalVariables();
#endif // _DEBUG

	worldTransform_.UpdateMatrix();

}

void Cage::Draw(const ViewProjection& viewProjection)
{

	model_->Draw(worldTransform_, viewProjection);

}

void Cage::Setting(const Vector2& position)
{

	// ワールドトランスフォーム
	worldTransform_.translation_ = { position.x * MapSystem::kSquareSize_.x, position.y * MapSystem::kSquareSize_.y, -15.0f };
	worldTransform_.rotation_ = { -1.5f,0.0f,0.0f };
	worldTransform_.scale_ = { 7.5f,7.5f,7.5f };
	model_->SetAlphaValue(0.85f);
	worldTransform_.UpdateMatrix();

}

void Cage::ApplyGlobalVariables()
{
}
