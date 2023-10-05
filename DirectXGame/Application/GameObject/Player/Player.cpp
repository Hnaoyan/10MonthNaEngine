#include "Player.h"
#include <algorithm>
#include "Input.h"

#include "Application/GameObject/Area/Area.h"

void Player::Initialize(Model* model)
{

	// ワールドトランスフォーム
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0.0f,10.0f,0.0f };
	worldTransform_.rotation_ = { 0.0f,0.0f,0.0f };
	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransform_.UpdateMatrix();

	// モデル
	model_ = model;

	// 速度
	velocity_ = { 0.0f ,0.0f };

	// 加速度
	acceleration_ = { 0.0f ,0.0f };

	// コライダーサイズ
	colliderSize_ = { 2.0f, 2.0f };

	//着地判定
	islanding_ = false;

}

void Player::Update()
{

	Move();
	
	// 着地していない
	if (!islanding_) {
		Falling();
	}

	worldTransform_.UpdateMatrix();

}

void Player::Draw(const ViewProjection& viewProjection)
{

	model_->Draw(worldTransform_, viewProjection);

}

void Player::Move()
{
	
	// 入力デバイスインスタンス取得
	Input* input = Input::GetInstance();

	//キーボード

	//左移動
	if ((input->PressKey(DIK_LEFT) || input->PressKey(DIK_A)) &&
		!(input->PressKey(DIK_RIGHT) || input->PressKey(DIK_D))) {
		velocity_.x = -kMoveVelocityMax_;
	}
	//右移動
	else if ((input->PressKey(DIK_RIGHT) || input->PressKey(DIK_D)) &&
		!(input->PressKey(DIK_LEFT) || input->PressKey(DIK_A))) {
		velocity_.x = kMoveVelocityMax_;
	}
	//移動しない
	else {

		//ゲームパッド

		XINPUT_STATE joyState;

		if (input->GetJoystickState(0, joyState)) {
			//移動
			velocity_.x = (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kMoveVelocityMax_;
		}
		else {
			velocity_.x = 0.0f;
		}

	}

	//ワールドトランスフォーム変更
	worldTransform_.translation_.x += velocity_.x;

	// 移動制限
	worldTransform_.translation_.x = 
		std::clamp(worldTransform_.translation_.x,
		area_->kPositionMin_.x + colliderSize_.x / 2.0f,
		area_->kPositionMax_.x - colliderSize_.x / 2.0f);

}

void Player::Jump()
{
}

void Player::Falling()
{

	velocity_.y += kFallingAcceleration;
		
	//ワールドトランスフォーム変更
	worldTransform_.translation_.y += velocity_.y;

	if (worldTransform_.translation_.y <= area_->kPositionMin_.y + colliderSize_.y / 2.0f) {
		FallToTheBottom();
	}
}

void Player::FallToTheBottom()
{

	islanding_ = true;
	worldTransform_.translation_.y = area_->kPositionMin_.y + colliderSize_.y / 2.0f;
	velocity_.y = 0.0f;

}
