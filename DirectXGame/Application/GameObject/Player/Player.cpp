#include "Player.h"
#include <algorithm>
#include "Input.h"

#include "Application/GameObject/Area/Area.h"
#include "Application/Others/Math2d/Math2d.h"

void Player::Initialize(Model* model)
{

	// ワールドトランスフォーム
	worldTransform_.Initialize();

	// モデル
	model_ = model;

	Setting();

}

void Player::Update()
{

	Move();
	
	// 着地していない
	if (!islanding_) {
		Falling();
		if (!isMidairJump_) {
			MidairJump();
		}
	}
	else {
		Jump();
	}

	worldTransform_.UpdateMatrix();

	Vector2 position = { worldTransform_.matWorld_.m[3][0],worldTransform_.matWorld_.m[3][1] };
	collider_.Update(position);

}

void Player::Draw(const ViewProjection& viewProjection)
{

	model_->Draw(worldTransform_, viewProjection);

}

void Player::Setting()
{

	// ワールドトランスフォーム
	worldTransform_.translation_ = { 0.0f,10.0f,0.0f };
	worldTransform_.rotation_ = { 0.0f,0.0f,0.0f };
	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransform_.UpdateMatrix();

	// 速度
	velocity_ = { 0.0f ,0.0f };

	// 加速度
	acceleration_ = { 0.0f ,0.0f };

	// コライダー
	Vector2 position = { worldTransform_.matWorld_.m[3][0],worldTransform_.matWorld_.m[3][1] };
	collider_.Initialize(&worldTransform_, Vector2{ 2.0f, 2.0f });
	collider_.SetCollisionAttribute(CollisionAttribute::player);
	collider_.SetCollisionMask(0xffffffff - CollisionAttribute::player);
	//コールバック設定
	std::function<void(uint32_t, WorldTransform*)> f = std::function<void(uint32_t, WorldTransform*)>(std::bind(&Player::OnCollision, this, std::placeholders::_1, std::placeholders::_2));
	collider_.SetFunction(f);

	//着地判定
	islanding_ = false;

	//空中ジャンプしたか
	isMidairJump_ = false;

}

void Player::OnCollision(uint32_t collisonObj, WorldTransform* worldTransform)
{
	collisonObj;
	if (true) {
		OnCollisionScaffold(worldTransform);
	}

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
		area_->kPositionMin_.x + collider_.GetSize().x / 2.0f,
		area_->kPositionMax_.x - collider_.GetSize().x / 2.0f);

}

void Player::Jump()
{

	// 入力デバイスインスタンス取得
	Input* input = Input::GetInstance();

	//ジャンプしたか
	bool isJump = false;

	//キーボード
	if (input->TriggerKey(DIK_SPACE)) {
		isJump = true;
	}
	else {

		//ゲームパッド

		XINPUT_STATE joyState;
		XINPUT_STATE preJoyState;

		if (input->GetJoystickState(0, joyState) && input->GetJoystickStatePrevious(0, preJoyState)) {
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A && !(preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
				isJump = true;
			}
		}

	}

	//ジャンプしていたら
	if (isJump) {

		islanding_ = false;

		velocity_.y = kJumpVelocity_;

		//ワールドトランスフォーム変更
		worldTransform_.translation_.y += velocity_.y;
		if (worldTransform_.translation_.y >= area_->kPositionMax_.y - collider_.GetSize().y / 2.0f) {
			worldTransform_.translation_.y = area_->kPositionMax_.y - collider_.GetSize().y / 2.0f;
		}

	}
	//ブロックマネージャーに発射するよう伝える

}

void Player::MidairJump()
{

	// 入力デバイスインスタンス取得
	Input* input = Input::GetInstance();

	//ジャンプしたか
	bool isJump = false;

	//キーボード
	if (input->TriggerKey(DIK_SPACE)) {
		isJump = true;
	}
	else {

		//ゲームパッド

		XINPUT_STATE joyState;
		XINPUT_STATE preJoyState;

		if (input->GetJoystickState(0, joyState) && input->GetJoystickStatePrevious(0, preJoyState)) {
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A && !(preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
				isJump = true;
			}
		}

	}

	//ジャンプしていたら
	if (isJump) {

		isMidairJump_ = true;

		velocity_.y = kMidairJumpVelocity_;

		//ワールドトランスフォーム変更
		worldTransform_.translation_.y += velocity_.y;
		if (worldTransform_.translation_.y >= area_->kPositionMax_.y - collider_.GetSize().y / 2.0f) {
			worldTransform_.translation_.y = area_->kPositionMax_.y - collider_.GetSize().y / 2.0f;
		}

	}

}

void Player::Falling()
{

	velocity_.y += kFallingAcceleration;
		
	//ワールドトランスフォーム変更
	worldTransform_.translation_.y += velocity_.y;

	if (worldTransform_.translation_.y <= area_->kPositionMin_.y + collider_.GetSize().y / 2.0f) {
		FallToTheBottom();
	}

}

void Player::FallToTheBottom()
{

	islanding_ = true;
	isMidairJump_ = false;
	worldTransform_.translation_.y = area_->kPositionMin_.y + collider_.GetSize().y / 2.0f;
	velocity_.y = 0.0f;

}

void Player::OnCollisionScaffold(WorldTransform* worldTransform)
{

	//どこの面と当たったか

	Vector2 playerPos = { worldTransform_.matWorld_.m[3][0],worldTransform_.matWorld_.m[3][1] };
	Vector2 blockPos = { worldTransform->matWorld_.m[3][0],worldTransform->matWorld_.m[3][1] };

	Vector2 playerLT = { worldTransform_.matWorld_.m[3][0] - collider_.GetSize().x,worldTransform_.matWorld_.m[3][1] + collider_.GetSize().y };
	Vector2 playerLB = { worldTransform_.matWorld_.m[3][0] - collider_.GetSize().x,worldTransform_.matWorld_.m[3][1] - collider_.GetSize().y };
	Vector2 playerRT = { worldTransform_.matWorld_.m[3][0] + collider_.GetSize().x,worldTransform_.matWorld_.m[3][1] + collider_.GetSize().y };
	Vector2 playerRB = { worldTransform_.matWorld_.m[3][0] + collider_.GetSize().x,worldTransform_.matWorld_.m[3][1] - collider_.GetSize().y };

	float move = 2.0f;



	worldTransform_.translation_.y = worldTransform->translation_.y + move;
	worldTransform_.UpdateMatrix();
	Vector2 position = { worldTransform_.matWorld_.m[3][0],worldTransform_.matWorld_.m[3][1] };
	collider_.Update(position);
	islanding_ = true;
	isMidairJump_ = false;
	velocity_.y = 0.0f;

}
