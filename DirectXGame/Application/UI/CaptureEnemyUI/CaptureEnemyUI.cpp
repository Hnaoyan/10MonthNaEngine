#include "CaptureEnemyUI.h"

void CaptureEnemyUI::Initialize(uint32_t enemyCountTextureHandle, uint32_t numTextureHandle, uint32_t slashTextureHandle)
{

	// 色
	Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	Vector2 anchorpoint = { 0.5f,0.5f };

	// 説明(上のやつ)
	enemyCountTextureHandle_ = enemyCountTextureHandle;
	enemyCountSize_ = {224.0f, 80.0f};
	enemyCountPosition_ = { 1056.0f,  544.0f };
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

	// エネミー数
	enemyCount_ = 0;

}

void CaptureEnemyUI::Update()
{
}

void CaptureEnemyUI::Draw()
{

	enemyCountSprite_->Draw();
	denominatorSprite_->Draw();
	numeratorSprite_->Draw();
	slashSprite_->Draw();

}

void CaptureEnemyUI::Setting(uint32_t enemyMax)
{

	// エネミー総数
	enemyMax_ = enemyMax;

	Vector2 base = { numSize_.x * enemyMax_, 0.0f };
	Vector2 size = { numSize_.x , numSize_.y};

	denominatorSprite_->SetSpriteRect(base, size);
	denominatorSprite_->Update();
	
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
