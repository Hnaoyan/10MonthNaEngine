#include "StageSelectUI.h"

void StageSelectUI::Initialize(uint32_t leftTextureHandle, uint32_t rightTextureHandle, uint32_t stageSelectTextureHandle, uint32_t stageNumberTextureHandle)
{

	// スプライト
	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };
	Vector2 anchorPoint = { 0.5f, 0.5f };

	// レフト
	leftTextureHandle_ = leftTextureHandle;
	leftPostion_ = { 158.0f, 360.0f };
	leftSize_ = { 128.0f, 192.0f };
	leftSprite_.reset(Sprite::Create(leftTextureHandle_, leftPostion_, color, anchorPoint, false, false));
	leftSprite_->SetSize(leftSize_);
	leftSprite_->SetSpriteRect(Vector2{ 128.0f, 0.0f }, leftSize_);
	leftSprite_->SetBlendMode(Sprite::BlendMode::kNormal);
	leftSprite_->Update();

	// ライト
	rightTextureHandle_ = rightTextureHandle;
	rightPostion_ = { 1122.0f, 360.0f };
	rightSize_ = { 128.0f, 192.0f };
	rightSprite_.reset(Sprite::Create(rightTextureHandle_, rightPostion_, color, anchorPoint, false, false));
	rightSprite_->SetSize(rightSize_);
	rightSprite_->SetSpriteRect(Vector2{ 128.0f, 0.0f }, rightSize_);
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

	// ステージナンバー
	stageNumberTextureHandle_ = stageNumberTextureHandle;
	stageNumberPostion_ = { 640.0f, 64.0f };
	stageNumberSize_ = { 300.0f, 128.0f };
	stageNumberSprite_.reset(Sprite::Create(stageNumberTextureHandle_, stageNumberPostion_, color, anchorPoint, false, false));
	stageNumberSprite_->SetSize(stageNumberSize_);
	stageNumberSprite_->SetSpriteRect(Vector2{ 0.0f, 0.0f }, Vector2{ 300.0f, 128.0f });
	stageNumberSprite_->SetBlendMode(Sprite::BlendMode::kNormal);
	stageNumberSprite_->Update();

	stageNum_ = 0;

}

void StageSelectUI::Update()
{

}

void StageSelectUI::Draw(bool isMove)
{

	if (isMove) {
		leftSprite_->Draw();
		rightSprite_->Draw();
		stageNumberSprite_->Draw();
	}
	stageSelectSprite_->Draw();

}

void StageSelectUI::Setting()
{

}

void StageSelectUI::SetStageNum(uint32_t stageNum)
{
	
	stageNum_ = stageNum;
	stageNumberSprite_->SetSpriteRect(Vector2{ 300.0f * stageNum, 0.0f }, Vector2{ 300.0f, 128.0f });
	stageNumberSprite_->Update();


}
