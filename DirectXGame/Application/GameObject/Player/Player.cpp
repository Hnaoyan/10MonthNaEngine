#include "Player.h"
#include <algorithm>

#include "Application/Others/Math2d/Math2d.h"
#include <GlobalVariables.h>
#include <imgui.h>

#include "Application/Others/MapSystem/MapSystem.h"

void Player::Initialize(Model* model,const Vector2& position)
{

	// ワールドトランスフォーム
	worldTransform_.Initialize();

	// モデル
	model_ = model;

#pragma region 調整項目クラス
	// 調整項目クラスのインスタンス取得
	//GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名設定
	//const char* groupName = "Player";
	// 指定した名前でグループ追加
	//globalVariables->CreateGroup(groupName);

	// メンバ変数の調整したい項目をグローバル変数に追加

	ApplyGlobalVariables();

#pragma endregion

	Setting(position);

}

void Player::Update(const Vector2& position)
{

#ifdef _DEBUG
	ApplyGlobalVariables();
#endif // _DEBUG

	position_ = position;
	worldTransform_.translation_ = { position.x * MapSystem::kSquareSize_.x, position.y * MapSystem::kSquareSize_.y, -2.0f };
	worldTransform_.UpdateMatrix();

}

void Player::Draw(const ViewProjection& viewProjection)
{

	model_->Draw(worldTransform_, viewProjection);

}

void Player::Setting(const Vector2& position)
{

	// ワールドトランスフォーム
	worldTransform_.translation_ = { position.x * MapSystem::kSquareSize_.x, position.y * MapSystem::kSquareSize_.y, -2.0f };
	worldTransform_.rotation_ = { 0.0f,0.0f,0.0f };
	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransform_.UpdateMatrix();

}

void Player::ApplyGlobalVariables()
{

	// 調整項目クラスのインスタンス取得
	//GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名の設定
	//const char* groupName = "Player";

}
