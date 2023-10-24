#include "CaptureEnemyUI.h"
#include <GlobalVariables.h>
#include <Math/MathCalc.h>

void CaptureEnemyUI::Initialize(uint32_t enemyCountTextureHandle, uint32_t numTextureHandle, uint32_t slashTextureHandle, uint32_t goGoalTextureHandle)
{

	// 色
	Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	Vector2 anchorpoint = { 0.5f,0.5f };

	// 説明(上のやつ)
	enemyCountTextureHandle_ = enemyCountTextureHandle;
	enemyCountSize_ = {224.0f, 80.0f};
	enemyCountPosition_ = { 1056.0f,  524.0f };
	enemyCountSprite_.reset(Sprite::Create(enemyCountTextureHandle_, enemyCountPosition_, color, anchorpoint, false, false));
	enemyCountSprite_->SetSize(enemyCountSize_);
	enemyCountSprite_->Update();

	// 数字*2
	numTextureHandle_ = numTextureHandle;
	numSize_ = { 96.0f, 96.0f };
	// 分母
	denominatorPosition_ = { 1152.0f, 624.0f };
	denominatorSprite_.reset(Sprite::Create(numTextureHandle_, denominatorPosition_, color, anchorpoint, false, false));
	denominatorSprite_->SetSize(numSize_);
	// 分子
	numeratorPosition_ = { 960.0f , 624.0f };
	numeratorSprite_.reset(Sprite::Create(numTextureHandle_, numeratorPosition_, color, anchorpoint, false, false));
	numeratorSprite_->SetSize(numSize_);

	Setting(0);

	// ／
	slashTextureHandle_ = slashTextureHandle;
	slashSize_ = { 96.0f, 96.0f };
	slashPosition_ = { 1056.0f, 624.0f };
	slashSprite_.reset(Sprite::Create(slashTextureHandle_, slashPosition_, color, anchorpoint, false, false));
	slashSprite_->SetSize(slashSize_);
	slashSprite_->Update();

	// goGoal
	goGoalTextureHandle_ = goGoalTextureHandle;
	goGoalSize_ = { 400.0f, 160.0f };
	goGoalPosition_ = { 1056.0f,  574.0f };
	goGoalSprite_.reset(Sprite::Create(goGoalTextureHandle_, goGoalPosition_, color, anchorpoint, false, false));
	goGoalSprite_->SetSize(goGoalSize_);
	goGoalSprite_->Update();

	// エネミー数
	enemyCount_ = 0;

#pragma region 調整項目クラス
	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名設定
	const char* groupName = "CaptureEnemyUI";
	// 指定した名前でグループ追加
	globalVariables->CreateGroup(groupName);

	// メンバ変数の調整したい項目をグローバル変数に追加

	globalVariables->AddItem(groupName, "enemyCountSize", enemyCountSize_);
	globalVariables->AddItem(groupName, "enemyCountPosition", enemyCountPosition_);

	globalVariables->AddItem(groupName, "numSize", numSize_);
	globalVariables->AddItem(groupName, "denominatorPosition", denominatorPosition_);
	globalVariables->AddItem(groupName, "numeratorPosition", numeratorPosition_);

	globalVariables->AddItem(groupName, "slashSize", slashSize_);
	globalVariables->AddItem(groupName, "slashPosition", slashPosition_);

	globalVariables->AddItem(groupName, "goGoalSize", goGoalSize_);
	globalVariables->AddItem(groupName, "goGoalPosition", goGoalPosition_);

	globalVariables->AddItem(groupName, "sizeMultiply", sizeMultiply_);

	ApplyGlobalVariables();

#pragma endregion

}

void CaptureEnemyUI::Update()
{

#ifdef _DEBUG
	ApplyGlobalVariables();

	enemyCountSprite_->SetPosition(enemyCountPosition_);
	//enemyCountSprite_->SetSize(enemyCountSize_);
	enemyCountSprite_->Update();

	denominatorSprite_->SetPosition(denominatorPosition_);
	//denominatorSprite_->SetSize(numSize_);
	denominatorSprite_->Update();

	numeratorSprite_->SetPosition(numeratorPosition_);
	//numeratorSprite_->SetSize(numSize_);
	numeratorSprite_->Update();

	slashSprite_->SetPosition(slashPosition_);
	//slashSprite_->SetSize(slashSize_);
	slashSprite_->Update();

	goGoalSprite_->SetPosition(goGoalPosition_);
	//goGoalSprite_->SetSize(goGoalSize_);
	goGoalSprite_->Update();

#endif // _DEBUG


}

void CaptureEnemyUI::Draw()
{

	if (enemyCount_ != enemyMax_) {
		enemyCountSprite_->Draw();
		denominatorSprite_->Draw();
		numeratorSprite_->Draw();
		slashSprite_->Draw();
	}
	else {
		goGoalSprite_->Draw();
	}

}

void CaptureEnemyUI::Setting(uint32_t enemyMax)
{

	// エネミー総数
	enemyMax_ = enemyMax;

	Vector2 base = { numSize_.x * enemyMax_, 0.0f };
	Vector2 size = { numSize_.x , numSize_.y};

	denominatorSprite_->SetSpriteRect(base, size);
	denominatorSprite_->Update();
	
	isAnimation_ = false;
	
	EnemyCountUpdate(0);

}

void CaptureEnemyUI::EnemyCountUpdate(uint32_t enemyCount)
{

	enemyCount_ = enemyCount;

	Vector2 base = { numSize_.x * enemyCount_, 0.0f };
	Vector2 size = { numSize_.x , numSize_.y };

	numeratorSprite_->SetSpriteRect(base, size);
	numeratorSprite_->Update();

}

void CaptureEnemyUI::ActionAnimationInitialize()
{

	// アニメーション
	// 初期スケール
	initEnemyCountSize_ = enemyCountSize_;
	// スケール中間
	middleEnemyCountSize_ = { enemyCountSize_.x * sizeMultiply_.x , enemyCountSize_.y * sizeMultiply_.y };

	// 初期スケール
	initNumSize_ = numSize_;
	// スケール中間
	middleNumCountSize_ = { numSize_.x * sizeMultiply_.x , numSize_.y * sizeMultiply_.y };

	// 初期スケール
	initSlashSize_ = slashSize_;
	// スケール中間
	middleSlashSize_ = { slashSize_.x * sizeMultiply_.x , slashSize_.y * sizeMultiply_.y };

	// 初期スケール
	initGoGoalSize_ = goGoalSize_;
	// スケール中間
	middleGoGoalSize_ = { goGoalSize_.x * sizeMultiply_.x , goGoalSize_.y * sizeMultiply_.y };

	// t
	animationT_ = 0.0f;
	// フレーム
	animationFrame_ = 10;

	isAnimation_ = true;

}

void CaptureEnemyUI::ActionAnimationUpdate()
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

			// サイズ変更
			enemyCountSize_ = MathCalc::EaseInCubicF(t, initEnemyCountSize_, middleEnemyCountSize_);
			numSize_ = MathCalc::EaseInCubicF(t, initNumSize_, middleNumCountSize_);
			slashSize_ = MathCalc::EaseInCubicF(t, initSlashSize_, middleSlashSize_);
			goGoalSize_ = MathCalc::EaseInCubicF(t, initGoGoalSize_, middleGoGoalSize_);
		}
		//  1.0f / 2.0f
		else {
			float t = (animationT_ - 1.0f / 2.0f) * 2.0f;

			// サイズ変更
			enemyCountSize_ = MathCalc::EaseInCubicF(t, middleEnemyCountSize_, initEnemyCountSize_);
			numSize_ = MathCalc::EaseInCubicF(t, middleNumCountSize_, initNumSize_);
			slashSize_ = MathCalc::EaseInCubicF(t, middleSlashSize_, initSlashSize_);
			goGoalSize_ = MathCalc::EaseInCubicF(t, middleGoGoalSize_, initGoGoalSize_);
		}

		// スプライト更新
		enemyCountSprite_->SetSize(enemyCountSize_);
		enemyCountSprite_->Update();
		denominatorSprite_->SetSize(numSize_);
		denominatorSprite_->Update();
		numeratorSprite_->SetSize(numSize_);
		numeratorSprite_->Update();
		slashSprite_->SetSize(slashSize_);
		slashSprite_->Update();
		goGoalSprite_->SetSize(goGoalSize_);
		goGoalSprite_->Update();
	}

}

void CaptureEnemyUI::ApplyGlobalVariables()
{

	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "CaptureEnemyUI";

	enemyCountSize_ = globalVariables->GetVector2Value(groupName, "enemyCountSize");
	enemyCountPosition_ = globalVariables->GetVector2Value(groupName, "enemyCountPosition");

	numSize_ = globalVariables->GetVector2Value(groupName, "numSize");
	denominatorPosition_ = globalVariables->GetVector2Value(groupName, "denominatorPosition");
	numeratorPosition_ = globalVariables->GetVector2Value(groupName, "numeratorPosition");

	slashSize_ = globalVariables->GetVector2Value(groupName, "slashSize");
	slashPosition_ = globalVariables->GetVector2Value(groupName, "slashPosition");

	goGoalSize_ = globalVariables->GetVector2Value(groupName, "goGoalSize");
	goGoalPosition_ = globalVariables->GetVector2Value(groupName, "goGoalPosition");

	sizeMultiply_ = globalVariables->GetVector2Value(groupName, "sizeMultiply");

}
