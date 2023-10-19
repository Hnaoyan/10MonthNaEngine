#include "Goal.h"
#include "Others/MapSystem/MapSystem.h"
#include "ImGuiManager.h"

void Goal::Initialize(Model* model, const Vector2& position)
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

void Goal::Update()
{

#ifdef _DEBUG
	ApplyGlobalVariables();
#endif // _DEBUG

	ImGui::Begin("goal");
	ImGui::DragFloat3("rota", &worldTransform_.rotation_.x, 0.01f, -6.0f, 6.0f);
	ImGui::End();

	worldTransform_.UpdateMatrix();

}

void Goal::Draw(const ViewProjection& viewProjection)
{

	model_->Draw(worldTransform_, viewProjection);

}

void Goal::Setting(const Vector2& position)
{

	// ワールドトランスフォーム
	worldTransform_.translation_ = { position.x * MapSystem::kSquareSize_.x, position.y * MapSystem::kSquareSize_.y, -0.01f };
	worldTransform_.rotation_ = { 4.71f,0.0f,0.0f };
	worldTransform_.scale_ = { 5.0f,5.0f,5.0f };
	worldTransform_.UpdateMatrix();

}

void Goal::ApplyGlobalVariables()
{
}
