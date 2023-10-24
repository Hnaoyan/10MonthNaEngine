#include "Cage.h"
#include "Others/MapSystem/MapSystem.h"
#include "../Engine/Math/MathCalc.h"
#include <GlobalVariables.h>

void Cage::Initialize(Model* model, const Vector2& position)
{

	// ワールドトランスフォーム
	worldTransform_.Initialize();

	// モデル
	model_ = model;

#pragma region 調整項目クラス
	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名設定
	const char* groupName = "Cage";
	// 指定した名前でグループ追加
	globalVariables->CreateGroup(groupName);

	// メンバ変数の調整したい項目をグローバル変数に追加
	
	//中間スケール
	globalVariables->AddItem(groupName, "scaleAdd", scaleAdd_);

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
	position_ = position;

	worldTransform_.translation_ = { position.x * MapSystem::kSquareSize_.x, position.y * MapSystem::kSquareSize_.y, positionZ_ };
	worldTransform_.rotation_ = { -1.5f,0.0f,0.0f };
	worldTransform_.scale_ = { 7.5f,7.5f,7.5f };
	model_->SetAlphaValue(0.85f);
	worldTransform_.UpdateMatrix();

}

void Cage::ActionAnimationInitialize()
{

	// アニメーション
	// するか
	isAnimation_ = true;
	// 初期スケール
	initScale_ = worldTransform_.scale_;
	// スケール中間
	middleScale_ = worldTransform_.scale_ + scaleAdd_;
	// t
	animationT_ = 0.0f;
	// フレーム
	animationFrame_ = 10;

}

void Cage::ActionAnimationUpdate()
{

	if (isAnimation_) {
		animationT_ += 1.0f / static_cast<float>(animationFrame_);
		if (animationT_ >= 1.0f) {
			animationT_ = 1.0f;
			isAnimation_ = false;
		}
		// 1.0f / 2.0f
		if (animationT_ < 1.0f / 2.0f) {
			float t = animationT_ * 2.0f;
			worldTransform_.scale_ = MathCalc::EaseOutCubicF(t, initScale_, middleScale_);
		}
		//  1.0f / 2.0f
		else {
			float t = (animationT_ - 1.0f / 2.0f) * 2.0f;
			worldTransform_.scale_ = MathCalc::EaseInCubicF(t, middleScale_, initScale_);
		}
		worldTransform_.UpdateMatrix();
	}
	
}

void Cage::ApplyGlobalVariables()
{

	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "Cage";

	//中間スケール
	scaleAdd_ = globalVariables->GetVector3Value(groupName, "scaleAdd");

}
