#include "Enemy.h"
#include "MathCalc.h"
#include "Others/MapSystem/MapSystem.h"

void Enemy::Initialize(Model* model, const Vector2& position, Model* sleepModel, Model* surprisedModel, Model* shadowModel,
	uint32_t awakeEnemyTextureHandle, uint32_t sleepEnemyTextureHandle)
{

	// ワールドトランスフォーム
	worldTransform_.Initialize();

	sleepWorldTransform_.Initialize();
	surprisedWorldTransform_.Initialize();
	surprisedWorldTransform_.translation_ = Vector3{ 0.0f, 10.0f, 0.0f };
	//surprisedWorldTransform_.parent_ = &worldTransform_;
	surprisedWorldTransform_.rotation_.x = -1.57f * 2.0f / 3.0f;
	surprisedWorldTransform_.scale_ = { surprisedSize_, surprisedSize_, surprisedSize_ };
	surprisedWorldTransform_.UpdateMatrix();

	shadowWorldTransform_.Initialize();
	shadowWorldTransform_.parent_ = &worldTransform_;
	shadowWorldTransform_.UpdateMatrix();

	// モデル
	model_ = model;

	sleepModel_ = sleepModel;

	surprisedModel_ = surprisedModel;

	shadowModel_ = shadowModel;

	// テクスチャハンドル
	awakeEnemyTextureHandle_ = awakeEnemyTextureHandle;
	sleepEnemyTextureHandle_ = sleepEnemyTextureHandle;

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

void Enemy::Update(const Vector2& position, bool enemyAwake, bool captured)
{

#ifdef _DEBUG
	ApplyGlobalVariables();
#endif // _DEBUG

	position_ = position;
	awake_ = enemyAwake;
	captured_ = captured;

	worldTransform_.UpdateMatrix();

	sleepWorldTransform_.translation_ = worldTransform_.translation_ + Vector3{ 5.0f, 0.0f, -10.0f };
	sleepWorldTransform_.rotation_.x = -1.57f * 2.0f / 3.0f;
	sleepWorldTransform_.UpdateMatrix();

}

void Enemy::Draw(const ViewProjection& viewProjection, bool isShadowDraw)
{

	// 本体
	if ((!awake_ || surprisedT_ < 1.0f / 2.0f) && !isGameOverAnimation_) {
		model_->Draw(worldTransform_, viewProjection, sleepEnemyTextureHandle_);
	}
	else {
		model_->Draw(worldTransform_, viewProjection, awakeEnemyTextureHandle_);
	}
	
	
	if (isShadowDraw) {
		shadowModel_->Draw(shadowWorldTransform_, viewProjection);
	}
	if ((!awake_ || surprisedT_ < 1.0f / 2.0f) && !isGameOverAnimation_ ) {
		sleepModel_->Draw(sleepWorldTransform_, viewProjection);
	}
	else if(surprisedT_ < 1.0f && surprisedT_ > 1.0f / 2.0f || isGameOverAnimation_){
		surprisedModel_->Draw(surprisedWorldTransform_,viewProjection);
	}

}

void Enemy::Setting(const Vector2& position)
{

	// ワールドトランスフォーム
	worldTransform_.translation_ = { position.x * MapSystem::kSquareSize_.x, position.y * MapSystem::kSquareSize_.y, -7.5f };
	position_ = { worldTransform_.translation_.x,worldTransform_.translation_.y };
	animationEndPosition_ = position_;
	ActionAnimationInitialize();
	rotate_ = 0.0f;
	worldTransform_.rotation_ = { 0.0f ,0.0f,rotate_ };
	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransform_.UpdateMatrix();

	// アニメーションの開始座標
	shadowAddZ_ = animationStartShadowAddZ_;
	shadowWorldTransform_.translation_ = { 0.0f, 0.0f, shadowAddZ_ };
	shadowWorldTransform_.rotation_ = { 0.0f, 0.0f, 0.0f };
	shadowWorldTransform_.UpdateMatrix();
	shadowModel_->SetAlphaValue(0.85f);

	awake_ = false;

	isGameOverAnimation_ = false;

}

void Enemy::ActionAnimationInitialize()
{
	animationT_ = 0;
	animationStartPosition_ = position_;
}

void Enemy::ActionAnimationUpdate()
{
	if (animationT_ >= 1.0f) {
		animationT_ = 1.0f;
	}
	else {
		animationT_ += (1.0f / 9.0f);
		Vector3 easePosition = MathCalc::EaseOutCubicF(animationT_,
			Vector3(animationStartPosition_.x, animationStartPosition_.y, 0),
			Vector3(position_.x, position_.y, 0));
		easePosition = { easePosition.x * MapSystem::kSquareSize_.x ,easePosition.y * MapSystem::kSquareSize_.y };
		worldTransform_.translation_.x = easePosition.x;
		worldTransform_.translation_.y = easePosition.y;
	}
	
	surprisedStartPosition_.x = worldTransform_.translation_.x;
	surprisedEndPosition_.x = worldTransform_.translation_.x;
	surprisedStartPosition_.y = worldTransform_.translation_.y;
	surprisedEndPosition_.y = worldTransform_.translation_.y;
	worldTransform_.UpdateMatrix();

	shadowWorldTransform_.UpdateMatrix();

}

void Enemy::WaitingAnimationInitialize()
{

	// スリープアニメーション
	// 位置
	Vector3 sleepStartPositionAdd = { 5.0f, 0.0f, -10.0f };
	sleepStartPosition_ = worldTransform_.translation_ + sleepStartPositionAdd;
	// 位置
	Vector3 sleepEndPositionAdd = { 7.5f, 0.0f, -12.5f };
	sleepMiddlePosition_ = worldTransform_.translation_ + sleepEndPositionAdd;
	// アニメーションt
	sleepT_ = 0.0f;
	// アニメーションフレーム
	sleepFrame_ = 40;

	// アニメーションt.x, 
	surprisedT_ = 0.0f;
	// アニメーションフレーム
	surprisedFrame_ = 90;

	surprisedStartPosition_ = worldTransform_.translation_;
	surprisedEndPosition_ = worldTransform_.translation_;
	surprisedEndPosition_.z = surprisedEndPosition_.z - 12.5f;


	// 影最終座標
	animationShadowAddZ_ = animationStartShadowAddZ_ + 12.5f;

	// サウンド再生したか
	wakeupSound_ = false;

	// 基準位置
	pukapukaPositionZ_ = surprisedEndPosition_.z;
	// 追加位置
	pukapukaPositionAddZ_ = 1.5f;
	pukapukaSpeed_ = 0.015f;
	pukapukaT_ = -1.0f;

}

void Enemy::WaitingAnimationUpdate()
{
	// スリープ
	if (!awake_ || surprisedT_ < 1.0f / 2.0f) {
		// アニメーションする
		sleepT_ += 1.0f / static_cast<float>(sleepFrame_);

		if (sleepT_ > 1.0f) {
			sleepT_ -= 1.0f;
		}

		if (sleepT_ < 1.0f / 2.0f) {
			float t = sleepT_ * 2.0f;
			sleepWorldTransform_.translation_ = MathCalc::EaseInCubicF(t, sleepStartPosition_, sleepMiddlePosition_);
		}
		else {
			float t = (sleepT_ - 1.0f / 2.0f) * 2.0f;
			sleepWorldTransform_.translation_ = MathCalc::EaseOutCubicF(t, sleepMiddlePosition_, sleepStartPosition_);
		}
	}

	// ビックリ
	if (animationT_ == 0.0f || animationT_ >= 1.0f) {
		if (surprisedT_ < 1.0f && awake_) {
			// アニメーションする
			surprisedT_ += 1.0f / static_cast<float>(surprisedFrame_);

			if (surprisedT_ > 1.0f) {
				surprisedT_ = 1.0f;
			}

			if (surprisedT_ > 1.0f / 2.0f && surprisedT_ < 3.0f / 4.0f) {
				// サウンド再生
				if (!wakeupSound_) {
					wakeupSound_ = true;
					Audio::GetInstance()->PlayWave(7, false, 0.3f);
				}

				float t = (surprisedT_ - 1.0f / 2.0f) * 4.0f;
				worldTransform_.translation_ = MathCalc::EaseInCubicF(t, surprisedStartPosition_, surprisedEndPosition_);
				worldTransform_.UpdateMatrix();
				surprisedWorldTransform_.scale_ = MathCalc::EaseInCubicF(t, Vector3(0.0f, 0.0f, 0.0f), Vector3(surprisedSize_, surprisedSize_, surprisedSize_));
				// 影
				shadowWorldTransform_.translation_.z = MathCalc::EaseInCubicF(t, animationStartShadowAddZ_, animationShadowAddZ_);
			}

		}
	}
	
	// 回転
	if (surprisedT_ > 1.0f / 2.0f && awake_ && !captured_) {
		worldTransform_.rotation_.z = MathCalc::EaseInCubicF(0.5f, worldTransform_.rotation_.z, rotate_);
	}

	// つかまってる
	if (captured_ && animationT_ >= 1.0f) {
		worldTransform_.translation_ = Vector3(position_.x * MapSystem::kSquareSize_.x,
			position_.y * MapSystem::kSquareSize_.y,
			pukapukaPositionZ_);
		worldTransform_.rotation_.z = rotate_;
	}

	// プカプカ
	float pukapukaAdd = 0.0f;
	if (surprisedT_ >= 1.0f && awake_ && !captured_) {

		pukapukaT_ += pukapukaSpeed_;
		if (pukapukaT_ >= 1.0f) {
			pukapukaT_ -= 2.0f;
		}

		pukapukaAdd = pukapukaPositionAddZ_ * std::cosf(1.0f * float(std::numbers::pi) * pukapukaT_);
		worldTransform_.translation_.z = pukapukaPositionZ_ + pukapukaAdd;
		shadowWorldTransform_.translation_.z = animationShadowAddZ_ - pukapukaAdd;

	}

	surprisedWorldTransform_.translation_.x = worldTransform_.translation_.x;
	surprisedWorldTransform_.translation_.y = worldTransform_.translation_.y;
	surprisedWorldTransform_.translation_.z = worldTransform_.translation_.z + surprisedZ_;

	worldTransform_.UpdateMatrix();
	sleepWorldTransform_.UpdateMatrix();
	surprisedWorldTransform_.UpdateMatrix();
	shadowWorldTransform_.UpdateMatrix();

}

void Enemy::GameOverAnimationInitialize(const Vector2& playerPosition)
{


	// ゲームオーバーアニメーション
	gameOverPositionStart_ = worldTransform_.translation_;
	gameOverPositionMiddle_ = { playerPosition.x , playerPosition.y, worldTransform_.translation_.z, };
	
	Vector2 distance = { playerPosition.x - worldTransform_.translation_.x, playerPosition.y - worldTransform_.translation_.y };

	if (distance.x > 0.0f) {
		worldTransform_.rotation_.z = 1.57f;
	}
	else if (distance.x < 0.0f) {
		worldTransform_.rotation_.z = -1.57f;
	}
	else if (distance.y > 0.0f) {
		worldTransform_.rotation_.z = 3.14f;
	}
	else if (distance.y < 0.0f) {
		worldTransform_.rotation_.z = 0.0f;
	}

	worldTransform_.UpdateMatrix();

	gameOverT_ = 0.0f;

	gameOverFrame_ = 40;

	isGameOverAnimation_ = true;

}

void Enemy::GameOverAnimationUpdate()
{

	if (isGameOverAnimation_) {
		gameOverT_ += 1.0f / static_cast<float>(gameOverFrame_);
		if (gameOverT_ > 1.0f) {
			gameOverT_ = 1.0f;
			isGameOverAnimation_ = false;
		}

		if (gameOverT_ < 1.0f / 5.0f) {
			float t = gameOverT_ * 5.0f;
			worldTransform_.translation_ = MathCalc::EaseInCubicF(t, gameOverPositionStart_, gameOverPositionMiddle_);
		}
		else {
			float t = (gameOverT_ - 1.0f / 5.0f) * 5.0f / 4.0f;
			worldTransform_.translation_ = MathCalc::EaseOutCubicF(t, gameOverPositionMiddle_, gameOverPositionStart_);
		}

		surprisedWorldTransform_.translation_.x = worldTransform_.translation_.x;
		surprisedWorldTransform_.translation_.y = worldTransform_.translation_.y;
		surprisedWorldTransform_.translation_.z = worldTransform_.translation_.z + surprisedZ_;

		worldTransform_.UpdateMatrix();
		shadowWorldTransform_.UpdateMatrix();
		surprisedWorldTransform_.UpdateMatrix();
	}

}

void Enemy::ApplyGlobalVariables()
{
}
