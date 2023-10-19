#include "StageSelectUI.h"

void StageSelectUI::Initialize(uint32_t leftTextureHandle, uint32_t rightTextureHandle, uint32_t stageSelectTextureHandle)
{

	// スプライト
	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };
	Vector2 anchorPoint = { 0.5f, 0.5f };

	// レフト
	leftTextureHandle_ = leftTextureHandle;
	leftPostion_ = { 158.0f, 410.0f };
	leftSize_ = { 256.0f, 192.0f };
	leftSprite_.reset(Sprite::Create(leftTextureHandle_, leftPostion_, color, anchorPoint, false, false));
	leftSprite_->SetSize(leftSize_);
	leftSprite_->SetBlendMode(Sprite::BlendMode::kNormal);
	leftSprite_->Update();

	// ライト
	rightTextureHandle_ = rightTextureHandle;
	rightPostion_ = { 1122.0f, 410.0f };
	rightSize_ = { 256.0f, 192.0f };
	rightSprite_.reset(Sprite::Create(rightTextureHandle_, rightPostion_, color, anchorPoint, false, false));
	rightSprite_->SetSize(rightSize_);
	rightSprite_->SetBlendMode(Sprite::BlendMode::kNormal);
	rightSprite_->Update();

	// ステージセレクト
	stageSelectTextureHandle_ = stageSelectTextureHandle;
	stageSelectPostion_ = { 640.0f, 650.0f };
	stageSelectSize_ = { 480.0f, 128.0f };
	stageSelectSprite_.reset(Sprite::Create(stageSelectTextureHandle_, stageSelectPostion_, color, anchorPoint, false, false));
	stageSelectSprite_->SetSize(stageSelectSize_);
	stageSelectSprite_->SetBlendMode(Sprite::BlendMode::kNormal);
	stageSelectSprite_->Update();

}

void StageSelectUI::Update()
{

}

void StageSelectUI::Draw()
{

	leftSprite_->Draw();
	rightSprite_->Draw();
	stageSelectSprite_->Draw();

}

void StageSelectUI::Setting()
{

}
