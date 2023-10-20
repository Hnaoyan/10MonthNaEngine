#include "Player.h"
#include <algorithm>

#include "Application/Others/Math2d/Math2d.h"
#include <GlobalVariables.h>
#include <imgui.h>

#include "Application/Others/MapSystem/MapSystem.h"
#include "../Engine/Math/MathCalc.h"

void Player::Initialize(Model* model,const Vector2& position)
{

	// ワールドトランスフォーム
	worldTransform_.Initialize();

	moveAnimationWorldTransform_.Initialize();

	// モデル
	model_ = model;

	// tマックス
	animationTMax_ = 20;

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

}

void Player::Draw(const ViewProjection& viewProjection)
{

	model_->Draw(worldTransform_, viewProjection);

}

void Player::Setting(const Vector2& position)
{

	// ワールドトランスフォーム
	worldTransform_.translation_ = { position.x * MapSystem::kSquareSize_.x, position.y * MapSystem::kSquareSize_.y, -10.0f };
	worldTransform_.rotation_ = { 0.0f,0.0f,0.0f };
	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransform_.parent_ = nullptr;
	worldTransform_.UpdateMatrix();

	actionNumber_ = ActionNumber::None;

}

void Player::WaitingAnimationInitialize()
{
}

void Player::WaitingAnimationUpdate()
{

	worldTransform_.rotation_.x += 0.01f;
	worldTransform_.UpdateMatrix();

}

void Player::ActionAnimationInitialize(uint32_t num)
{

	actionNumber_ = static_cast<ActionNumber>(num);

	switch (actionNumber_)
	{
	case Player::ActionNumber::kLeft:
		MoveAnimationInitialize(actionNumber_);
		break;
	case Player::ActionNumber::kRight:
		MoveAnimationInitialize(actionNumber_);
		break;
	case Player::ActionNumber::kUp:
		MoveAnimationInitialize(actionNumber_);
		break;
	case Player::ActionNumber::kDown:
		MoveAnimationInitialize(actionNumber_);
		break;
	case Player::ActionNumber::kVibration:
		VibrationAnimationInitialize();
		break;
	default:
		break;
	}

}

void Player::ActionAnimationUpdate()
{

	switch (actionNumber_)
	{
	case Player::ActionNumber::kLeft:
		MoveAnimationUpdate();
		break;
	case Player::ActionNumber::kRight:
		MoveAnimationUpdate();
		break;
	case Player::ActionNumber::kUp:
		MoveAnimationUpdate();
		break;
	case Player::ActionNumber::kDown:
		MoveAnimationUpdate();
		break;
	case Player::ActionNumber::kVibration:
		VibrationAnimationUpdate();
		break;
	default:
		break;
	}


}

void Player::MoveAnimationInitialize(ActionNumber actionNumber)
{

	// t
	animationT_ = 0;
	// tマックス
	animationTMax_ = 20;

	// MoveAnimation
	// スタート角度
	moveAnimationStartRotate_ = { 0.0f,0.0f,0.0f };
	moveAnimationEndRotate_ = { 0.0f,0.0f,0.0f };
	// 親
	moveAnimationWorldTransform_.translation_ = worldTransform_.translation_;
	moveAnimationWorldTransform_.translation_.z += MapSystem::kSquareSize_.y / 2.0f;
	moveAnimationWorldTransform_.rotation_ = { 0.0f,0.0f,0.0f };

	worldTransform_.translation_ = { 0.0f, 0.0f, -MapSystem::kSquareSize_.y / 2.0f };

	switch (actionNumber)
	{
	case ActionNumber::kLeft:
		// エンド角度
		moveAnimationEndRotate_.y += 1.57f;
		moveAnimationWorldTransform_.translation_.x -= MapSystem::kSquareSize_.x / 2.0f;
		worldTransform_.translation_.x += MapSystem::kSquareSize_.x / 2.0f;
		break;
	case ActionNumber::kRight:
		// エンド角度
		moveAnimationEndRotate_.y -= 1.57f;
		moveAnimationWorldTransform_.translation_.x += MapSystem::kSquareSize_.x / 2.0f;
		worldTransform_.translation_.x -= MapSystem::kSquareSize_.x / 2.0f;
		break;
	case ActionNumber::kUp:
		// エンド角度
		moveAnimationEndRotate_.x += 1.57f;
		moveAnimationWorldTransform_.translation_.y += MapSystem::kSquareSize_.y / 2.0f;
		worldTransform_.translation_.y -= MapSystem::kSquareSize_.y / 2.0f;
		break;
	case ActionNumber::kDown:
		// エンド角度
		moveAnimationEndRotate_.x -= 1.57f;
		moveAnimationWorldTransform_.translation_.y -= MapSystem::kSquareSize_.y / 2.0f;
		worldTransform_.translation_.y += MapSystem::kSquareSize_.y / 2.0f;
		break;
	default:
		break;
	}
	moveAnimationWorldTransform_.UpdateMatrix();
	
	worldTransform_.parent_ = &moveAnimationWorldTransform_;
	worldTransform_.UpdateMatrix();

}

void Player::MoveAnimationUpdate()
{

	animationT_ += 1.0f / animationTMax_;
	if (animationT_ >= 1.0f) {
		animationT_ = 1.0f;
		moveAnimationWorldTransform_.rotation_ = MathCalc::EaseInCubicF(animationT_, moveAnimationStartRotate_, moveAnimationEndRotate_);
		// 親子関係外す
		worldTransform_.translation_ = { worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1], worldTransform_.matWorld_.m[3][2] };
		worldTransform_.rotation_ = worldTransform_.rotation_ + moveAnimationWorldTransform_.rotation_;
		worldTransform_.parent_ = nullptr;
		// トランスフォーム
		worldTransform_.translation_ = { position_.x * MapSystem::kSquareSize_.x, position_.y * MapSystem::kSquareSize_.y, -10.0f };
	}
	else {
		moveAnimationWorldTransform_.rotation_ = MathCalc::EaseInCubicF(animationT_, moveAnimationStartRotate_, moveAnimationEndRotate_);
	}

	// 更新
	moveAnimationWorldTransform_.UpdateMatrix();
	worldTransform_.UpdateMatrix();

}

void Player::VibrationAnimationInitialize()
{

	// t
	animationT_ = 0;
	// tマックス
	animationTMax_ = 40;

	// スタート角度
	vibrationAnimationGroundPostion_ = worldTransform_.translation_;
	// エンド角度
	vibrationAnimationHighPostion_ = { worldTransform_.translation_ .x, worldTransform_.translation_.y, worldTransform_.translation_.z - 15.0f };

}

void Player::VibrationAnimationUpdate()
{

	animationT_ += 1.0f / animationTMax_;

	if (animationT_ < 0.5f) {
		float t = animationT_ * 2.0f;
		if (t > 1.0f) {
			t = 1.0f;
		}
		worldTransform_.translation_ = MathCalc::EaseInQuadF(t, vibrationAnimationGroundPostion_, vibrationAnimationHighPostion_);
	}
	else {
		float t = (animationT_ - 0.5f) * 2.0f;
		if (t > 1.0f) {
			t = 1.0f;
		}
		worldTransform_.translation_ = MathCalc::EaseOutQuadF(t, vibrationAnimationHighPostion_, vibrationAnimationGroundPostion_);
	}

	worldTransform_.UpdateMatrix();

}

void Player::ApplyGlobalVariables()
{

	// 調整項目クラスのインスタンス取得
	//GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名の設定
	//const char* groupName = "Player";

}
