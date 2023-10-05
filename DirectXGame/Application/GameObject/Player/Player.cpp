#include "Player.h"
#include "Input.h"

void Player::Initialize(Model* model)
{

	// ワールドトランスフォーム
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0.0f,0.0f,0.0f };
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
	colliderSize_ = { worldTransform_.scale_.x, worldTransform_.scale_.y };

	//着地判定
	islanding_ = false;

}

void Player::Update()
{

	worldTransform_.UpdateMatrix();

}

void Player::Draw(const ViewProjection& viewProjection)
{

	model_->Draw(worldTransform_, viewProjection);

}

void Player::Move()
{
	
	// 入力デバイスインスタンス取得
	//Input* input = Input::GetInstance();

}

void Player::Jump()
{
}
