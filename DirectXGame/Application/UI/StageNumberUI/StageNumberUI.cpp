#include "StageNumberUI.h"

void StageNumberUI::Initialize(uint32_t texture)
{
	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };
	Vector2 anchorPoint = { 0,0 };
	position_ = { 950.0f,10.0f };
	size_ = { 300.0f,128.0f };

	textureHandle_ = texture;
	stageNumberSprite_.reset(Sprite::Create(textureHandle_, position_, color, anchorPoint, false, false));
	stageNumberSprite_->SetSize(size_);
	stageNumberSprite_->SetSpriteRect(Vector2{ 0,0 }, size_);
	stageNumberSprite_->SetBlendMode(Sprite::BlendMode::kNormal);
	stageNumberSprite_->Update();
}

void StageNumberUI::Setting(int stageNum)
{
	// 現在のステージ番号取得
	stageNumberSprite_->SetSpriteRect(Vector2{ size_.x * (float)stageNum, 0.0f }, size_);
	stageNumberSprite_->Update();

}

void StageNumberUI::Draw()
{
	stageNumberSprite_->Draw();
}
