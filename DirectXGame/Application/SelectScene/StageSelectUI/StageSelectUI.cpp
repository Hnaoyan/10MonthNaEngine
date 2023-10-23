#include "StageSelectUI.h"
#include "VectorLib.h"
#include <numbers>

void StageSelectUI::Initialize(uint32_t leftTextureHandle, uint32_t rightTextureHandle, uint32_t stageSelectTextureHandle,
	uint32_t stageNumberTextureHandle, uint32_t stageUiTextureHandle)
{

	// スプライト
	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };
	Vector2 anchorPoint = { 0.5f, 0.5f };

	// レフト
	leftTextureHandle_ = leftTextureHandle;
	leftPostion_ = { 178.0f, 360.0f };
	leftSize_ = { 128.0f, 192.0f };
	leftSprite_.reset(Sprite::Create(leftTextureHandle_, leftPostion_, color, anchorPoint, false, false));
	leftSprite_->SetSize(leftSize_);
	leftSprite_->SetSpriteRect(Vector2{ 128.0f, 0.0f }, leftSize_);
	leftSprite_->SetBlendMode(Sprite::BlendMode::kNormal);
	leftSprite_->Update();

	// ライト
	rightTextureHandle_ = rightTextureHandle;
	rightPostion_ = { 1102.0f, 360.0f };
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

	// ステージナンバー
	stageUiTextureHandle_ = stageUiTextureHandle;
	stageUiPostion_ = { 1100.0f, 650.0f };
	stageUiSize_ = { 240.0f, 96.0f };
	stageUiSprite_.reset(Sprite::Create(stageUiTextureHandle_, stageUiPostion_, color, anchorPoint, false, false));
	stageUiSprite_->SetSize(stageUiSize_);
	stageUiSprite_->SetSpriteRect(Vector2{ 0.0f, 0.0f }, Vector2{ 160.0f, 64.0f });
	stageUiSprite_->SetBlendMode(Sprite::BlendMode::kNormal);
	stageUiSprite_->Update();

	stageNum_ = 0;

}

void StageSelectUI::Update()
{
	//waveAnimation_t_ += 0.01f;
	//float amplitude = 0.3f;
	//float fre = 5.0f;
	//waveVelocity_.y = amplitude * std::cosf(1.0f * float(std::numbers::pi) * fre * waveAnimation_t_);

	//// 上のUI
	//stageNumberPostion_ = VectorLib::Add(stageNumberPostion_, waveVelocity_);
	//stageNumberSprite_->SetPosition(stageNumberPostion_);
	//stageNumberSprite_->Update();
	//
	//// 左右の矢印
	//leftPostion_ = VectorLib::Add(leftPostion_, waveVelocity_);
	//rightPostion_ = VectorLib::Add(rightPostion_, waveVelocity_);

	//leftSprite_->SetPosition(leftPostion_);
	//rightSprite_->SetPosition(rightPostion_);

	//leftSprite_->Update();
	//rightSprite_->Update();

	//// 下のUI
	//stageSelectPostion_ = VectorLib::Add(stageSelectPostion_, waveVelocity_);
	//stageSelectSprite_->SetPosition(stageSelectPostion_);
	//stageSelectSprite_->Update();

}

void StageSelectUI::Draw(bool isMove)
{

	if (isMove) {
		leftSprite_->Draw();
		rightSprite_->Draw();
		stageNumberSprite_->Draw();
		stageUiSprite_->Draw();
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
