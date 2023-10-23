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
	animationFrame_ = 20;

#pragma region 調整項目クラス
	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名設定
	const char* groupName = "Player";
	// 指定した名前でグループ追加
	globalVariables->CreateGroup(groupName);

	// メンバ変数の調整したい項目をグローバル変数に追加

	//移動中間スケール
	globalVariables->AddItem(groupName, "moveAnimationMiddleScaleAdd", moveAnimationMiddleScaleAdd_);
	// ジャンプサイズ
	globalVariables->AddItem(groupName, "vibrationAnimationJumpScale", vibrationAnimationJumpScale_);
	// めり込みサイズ
	globalVariables->AddItem(groupName, "vibrationAnimationFillScale", vibrationAnimationFillScale_);
	// 振動エンド位置
	globalVariables->AddItem(groupName, "vibrationAnimationHighPostionAdd", vibrationAnimationHighPostionAdd_);
	// 振動めり込み位置
	globalVariables->AddItem(groupName, "vibrationAnimationFillPostionAdd", vibrationAnimationFillPostionAdd_);

	// 移動T
	globalVariables->AddItem(groupName, "moveAnimationT", static_cast<int>(moveAnimationT_));
	// 移動ミスT
	globalVariables->AddItem(groupName, "moveErrorAnimationT_", static_cast<int>(moveErrorAnimationT_));
	// 振動T
	globalVariables->AddItem(groupName, "vibrationAnimationT", static_cast<int>(vibrationAnimationT_));
	// クリアT
	globalVariables->AddItem(groupName, "clearAnimationT", static_cast<int>(clearAnimationT_));

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

	actionNumber_ = ActionNumber::kNone;

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
	case Player::ActionNumber::kRight:
	case Player::ActionNumber::kUp:
	case Player::ActionNumber::kDown:
		MoveAnimationInitialize(actionNumber_);
		break;
	case Player::ActionNumber::kLeftError:
	case Player::ActionNumber::kRightError:
	case Player::ActionNumber::kUpError:
	case Player::ActionNumber::kDownError:
		MoveErrorAnimationInitialize(actionNumber_);
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
	case Player::ActionNumber::kRight:
	case Player::ActionNumber::kUp:
	case Player::ActionNumber::kDown:
		MoveAnimationUpdate();
		break;
	case Player::ActionNumber::kLeftError:
	case Player::ActionNumber::kRightError:
	case Player::ActionNumber::kUpError:
	case Player::ActionNumber::kDownError:
		MoveErrorAnimationUpdate();
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
	animationFrame_ = moveAnimationT_;

	// スタートスケール
	moveAnimationStartScale_ = worldTransform_.scale_;
	// 中間スケール
	moveAnimationMiddleScale_ = VectorLib::Add(worldTransform_.scale_, moveAnimationMiddleScaleAdd_);

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

	animationT_ += 1.0f / animationFrame_;
	if (animationT_ >= 1.0f) {
		animationT_ = 1.0f;
		moveAnimationWorldTransform_.rotation_ = MathCalc::EaseInCubicF(animationT_, moveAnimationStartRotate_, moveAnimationEndRotate_);
		// 親子関係外す
		worldTransform_.translation_ = { worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1], worldTransform_.matWorld_.m[3][2] };
		worldTransform_.rotation_ = worldTransform_.rotation_ + moveAnimationWorldTransform_.rotation_;
		// 角度
		if (worldTransform_.rotation_.x <= -6.28f || worldTransform_.rotation_.x >= 6.28f) {
			worldTransform_.rotation_.x = 0.0f;
		}
		if (worldTransform_.rotation_.y <= -6.28f || worldTransform_.rotation_.y >= 6.28f) {
			worldTransform_.rotation_.y = 0.0f;
		}

		worldTransform_.parent_ = nullptr;
		// トランスフォーム
		worldTransform_.translation_ = { position_.x * MapSystem::kSquareSize_.x, position_.y * MapSystem::kSquareSize_.y, -10.0f };
	}
	else {
		moveAnimationWorldTransform_.rotation_ = MathCalc::EaseInCubicF(animationT_, moveAnimationStartRotate_, moveAnimationEndRotate_);
	}

	// 1.0f / 2.0f
	if (animationT_ < 1.0f / 2.0f) {
		float t = animationT_ * 2.0f;
		worldTransform_.scale_ = MathCalc::EaseOutCubicF(t, moveAnimationStartScale_, moveAnimationMiddleScale_);
	}
	//  1.0f / 2.0f
	else {
		float t = (animationT_ - 1.0f / 2.0f) * 2.0f;
		worldTransform_.scale_ = MathCalc::EaseInCubicF(t, moveAnimationMiddleScale_, moveAnimationStartScale_);
	}

	// 更新
	moveAnimationWorldTransform_.UpdateMatrix();
	worldTransform_.UpdateMatrix();

}

void Player::MoveErrorAnimationInitialize(ActionNumber actionNumber)
{

	// t
	animationT_ = 0;
	// tマックス
	animationFrame_ = moveErrorAnimationT_;

	// スタートスケール
	moveAnimationStartScale_ = worldTransform_.scale_;
	// 中間スケール
	moveAnimationMiddleScale_ = VectorLib::Add(worldTransform_.scale_, moveAnimationMiddleScaleAdd_);

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
	case ActionNumber::kLeftError:
		// エンド角度
		moveAnimationEndRotate_.y += 1.0f;
		moveAnimationWorldTransform_.translation_.x -= MapSystem::kSquareSize_.x / 2.0f;
		worldTransform_.translation_.x += MapSystem::kSquareSize_.x / 2.0f;
		break;
	case ActionNumber::kRightError:
		// エンド角度
		moveAnimationEndRotate_.y -= 1.0f;
		moveAnimationWorldTransform_.translation_.x += MapSystem::kSquareSize_.x / 2.0f;
		worldTransform_.translation_.x -= MapSystem::kSquareSize_.x / 2.0f;
		break;
	case ActionNumber::kUpError:
		// エンド角度
		moveAnimationEndRotate_.x += 1.0f;
		moveAnimationWorldTransform_.translation_.y += MapSystem::kSquareSize_.y / 2.0f;
		worldTransform_.translation_.y -= MapSystem::kSquareSize_.y / 2.0f;
		break;
	case ActionNumber::kDownError:
		// エンド角度
		moveAnimationEndRotate_.x -= 1.0f;
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

void Player::MoveErrorAnimationUpdate()
{

	animationT_ += 1.0f / animationFrame_;
	// 3.0f / 4.0f
	if (animationT_ < 3.0f / 4.0f) {
		float t = animationT_ * 4.0f / 3.0f;
		moveAnimationWorldTransform_.rotation_ = MathCalc::EaseOutCubicF(t, moveAnimationStartRotate_, moveAnimationEndRotate_);
		worldTransform_.scale_ = MathCalc::EaseOutCubicF(t, moveAnimationStartScale_, moveAnimationMiddleScale_);
	}
	//  1.0f / 4.0f
	else {
		float t = (animationT_ - 3.0f / 4.0f) * 4.0f;
		moveAnimationWorldTransform_.rotation_ = MathCalc::EaseInCubicF(t, moveAnimationEndRotate_, moveAnimationStartRotate_);
		worldTransform_.scale_ = MathCalc::EaseOutCubicF(t, moveAnimationMiddleScale_, moveAnimationStartScale_);
	}

	if (animationT_ >= 1.0f) {
		animationT_ = 1.0f;
		moveAnimationWorldTransform_.rotation_ = moveAnimationStartRotate_;
		// 親子関係外す
		worldTransform_.translation_ = { worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1], worldTransform_.matWorld_.m[3][2] };
		worldTransform_.rotation_ = worldTransform_.rotation_ + moveAnimationWorldTransform_.rotation_;
		worldTransform_.parent_ = nullptr;
		// トランスフォーム
		worldTransform_.translation_ = { position_.x * MapSystem::kSquareSize_.x, position_.y * MapSystem::kSquareSize_.y, -10.0f };
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
	animationFrame_ = vibrationAnimationT_;

	// スタート位置
	vibrationAnimationGroundPostion_ = worldTransform_.translation_;
	// エンド位置
	vibrationAnimationHighPostion_ = worldTransform_.translation_ + vibrationAnimationHighPostionAdd_;
	// めり込み位置
	vibrationAnimationFillPostion_ = worldTransform_.translation_ + vibrationAnimationFillPostionAdd_;
	// スタート角度
	vibrationAnimationStartRotate_ = worldTransform_.rotation_;
	// 角度によって変更
	if (worldTransform_.rotation_.y == 0.0f || worldTransform_.rotation_.y == 3.14f) {
		// エンド角度
		vibrationAnimationEndRotate_ = { worldTransform_.rotation_.x + 6.28f, worldTransform_.rotation_.y, worldTransform_.rotation_.z };
	}
	else {
		// エンド角度
		vibrationAnimationEndRotate_ = { worldTransform_.rotation_.x - 6.28f, worldTransform_.rotation_.y, worldTransform_.rotation_.z };
	}

}

void Player::VibrationAnimationUpdate()
{

	animationT_ += 1.0f / animationFrame_;

	// ジャンプ準備 1/10
	if (animationT_ < 1.0f / 10.0f) {
		float t = animationT_ * 10.0f;
		worldTransform_.translation_ = MathCalc::EaseInQuadF(t, vibrationAnimationGroundPostion_, vibrationAnimationFillPostion_);
		worldTransform_.scale_ = MathCalc::EaseInQuadF(t, Vector3{ 1.0f, 1.0f, 1.0f}, vibrationAnimationFillScale_);
	}
	// ジャンプ上昇 2/10
	else if (animationT_ < 3.0f / 10.0f) {
		float t = (animationT_ - 1.0f / 10.0f) * 5.0f;
		worldTransform_.translation_ = MathCalc::EaseOutQuadF(t, vibrationAnimationFillPostion_, vibrationAnimationHighPostion_);
		worldTransform_.scale_ = MathCalc::EaseOutQuadF(t, vibrationAnimationFillScale_, vibrationAnimationJumpScale_);
	}
	// 回転         2/10
	else if (animationT_ < 5.0f / 10.0f) {
		float t = (animationT_ - 3.0f / 10.0f) * 5.0f;
		worldTransform_.rotation_ = MathCalc::EaseInQuadF(t, vibrationAnimationStartRotate_, vibrationAnimationEndRotate_);
		worldTransform_.scale_ = MathCalc::EaseInQuadF(t, vibrationAnimationJumpScale_, Vector3{ 1.0f, 1.0f, 1.0f });
	}
	// 硬直         1/10
	else if (animationT_ < 6.0f / 10.0f) {
		//float t = (animationT_ - 5.0f / 10.0f) * 10.0f;
		worldTransform_.rotation_ = vibrationAnimationStartRotate_;
	}
	// 下降        2/10
	else if (animationT_ < 8.0f / 10.0f) {
		float t = (animationT_ - 6.0f / 10.0f) * 5.0f;
		worldTransform_.translation_ = MathCalc::EaseOutQuadF(t, vibrationAnimationHighPostion_, vibrationAnimationFillPostion_);
		worldTransform_.scale_ = MathCalc::EaseInQuadF(t, Vector3{ 1.0f, 1.0f, 1.0f }, vibrationAnimationFillScale_);
	}
	// 硬直        1/10
	else if (animationT_ < 9.0f / 10.0f) {
		//float t = (animationT_ - 8.0f / 10.0f) * 5.0f;
	}
	// 復帰         1/10
	else {
		float t = (animationT_ - 9.0f / 10.0f) * 10.0f;
		worldTransform_.translation_ = MathCalc::EaseOutQuadF(t, vibrationAnimationFillPostion_, vibrationAnimationGroundPostion_);
		worldTransform_.scale_ = MathCalc::EaseOutQuadF(t, vibrationAnimationFillScale_, Vector3{ 1.0f, 1.0f, 1.0f });
	}

	worldTransform_.UpdateMatrix();

}

void Player::ClearAnimationInitialize()
{

	// t
	animationT_ = 0;
	// tマックス
	animationFrame_ = clearAnimationT_;

	// スタート位置
	clearAnimationStartPostion_ = worldTransform_.translation_;
	// エンド位置
	clearAnimationEndPostion_ = { worldTransform_.translation_.x , worldTransform_.translation_.y ,  worldTransform_.translation_.z - 150.0f};
	// スタート角度
	clearArnimationStartRotate_ = worldTransform_.rotation_;
	// 角度によって変更
	if (worldTransform_.rotation_.y == 0.0f || worldTransform_.rotation_.y == 3.14f) {
		// エンド角度
		clearAnimationEndRotate_ = { worldTransform_.rotation_.x, worldTransform_.rotation_.y, worldTransform_.rotation_.z + 6.28f * 10.0f };
	}
	else {
		// エンド角度
		clearAnimationEndRotate_ = { worldTransform_.rotation_.x, worldTransform_.rotation_.y, worldTransform_.rotation_.z - 6.28f * 10.0f };
	}

}

void Player::ClearAnimationUpdate()
{

	animationT_ += 1.0f / animationFrame_;

	// 飛ぶ
	if (animationT_ >= 1.0f / 4.0f) {
		float t = (animationT_ - 1.0f / 4.0f) * 4.0f / 3.0f;
		worldTransform_.translation_ = MathCalc::EaseInQuadF(t, clearAnimationStartPostion_, clearAnimationEndPostion_);
	}

	worldTransform_.rotation_ = MathCalc::EaseInQuadF(animationT_, clearArnimationStartRotate_, clearAnimationEndRotate_);

	worldTransform_.UpdateMatrix();

}

void Player::ApplyGlobalVariables()
{

	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "Player";

	//移動中間スケール
	moveAnimationMiddleScaleAdd_ = globalVariables->GetVector3Value(groupName, "moveAnimationMiddleScaleAdd");
	// ジャンプサイズ
	vibrationAnimationJumpScale_ = globalVariables->GetVector3Value(groupName, "vibrationAnimationJumpScale");
	// めり込みサイズ
	vibrationAnimationFillScale_ = globalVariables->GetVector3Value(groupName, "vibrationAnimationFillScale");
	// 振動エンド位置
	vibrationAnimationHighPostionAdd_ = globalVariables->GetVector3Value(groupName, "vibrationAnimationHighPostionAdd");
	// 振動めり込み位置
	vibrationAnimationFillPostionAdd_ = globalVariables->GetVector3Value(groupName, "vibrationAnimationFillPostionAdd");

	// 移動T
	moveAnimationT_ = static_cast<uint32_t>(globalVariables->GetIntValue(groupName, "moveAnimationT"));
	// 移動ミスT
	moveErrorAnimationT_ = static_cast<uint32_t>(globalVariables->GetIntValue(groupName, "moveErrorAnimationT_"));
	// 振動T
	vibrationAnimationT_ = static_cast<uint32_t>(globalVariables->GetIntValue(groupName, "vibrationAnimationT"));
	// クリアT
	clearAnimationT_ = static_cast<uint32_t>(globalVariables->GetIntValue(groupName, "clearAnimationT"));

}
