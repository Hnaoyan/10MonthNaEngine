#include "StagePhot.h"
#include "../Engine/Math/MathCalc.h"

void StagePhot::Initialize(std::vector<uint32_t> textureHandles, size_t stageMax)
{

	// テクスチャハンドル
	textureHandles_ = textureHandles;

	// 固定位置
	positions_[0] = { -960.0f, 432.0f };
	positions_[1] = { -160.0f ,  432.0f };
	positions_[2] = { 640.0f, 360.0f }; //中央
	positions_[3] = { 1440.0f,  432.0f };
	positions_[4] = { 2240.0f,  432.0f };

	// サイズ
	sizes_[0] = { 512.0f, 360.f };
	sizes_[1] = { 512.0f, 360.f };
	sizes_[2] = { 768.0f, 432.0f }; //中央
	sizes_[3] = { 512.0f, 360.f };
	sizes_[4] = { 512.0f, 360.f };

	// スプライト
	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };
	Vector2 anchorPoint = { 0.5f, 0.5f };
	for (size_t i = 0; i < 5; i++) {
		sprite_[i].reset(Sprite::Create(textureHandles_[0], positions_[i], color, anchorPoint, false, false));
		sprite_[i]->SetSize(sizes_[i]);
		sprite_[i]->Update();
	}

	// ステージナンバー
	stageMax_ = stageMax;

	Setting();

}

void StagePhot::Update()
{


}

void StagePhot::Draw()
{

	for (size_t i = 0; i < 5; i++) {
		sprite_[i]->Draw();
	}

}

void StagePhot::Setting()
{
	// スプライトのテクスチャ
	sprite_[0]->SetTextureHandle(textureHandles_[stageMax_ - 2]);
	sprite_[1]->SetTextureHandle(textureHandles_[stageMax_ - 1]);
	sprite_[2]->SetTextureHandle(textureHandles_[0]);
	sprite_[3]->SetTextureHandle(textureHandles_[1]);
	sprite_[4]->SetTextureHandle(textureHandles_[2]);

	// ステージナンバー
	stageNum_ = 0;

}

void StagePhot::MoveRight(float t)
{

	for (size_t i = 1; i < 5; i++) {
		sprite_[i]->SetPosition(MathCalc::EaseInQuadF(t, positions_[i], positions_[i - 1]));
		sprite_[i]->SetSize(MathCalc::EaseInQuadF(t, sizes_[i], sizes_[i - 1]));
		sprite_[i]->Update();
	}
	if (t >= 1.0f) {
		stageNum_++;
		if (stageMax_ == stageNum_) {
			stageNum_ = 0;
		}
		TextureHandleChange();
	}

}

void StagePhot::MoveLeft(float t)
{

	for (size_t i = 0; i < 4; i++) {
		sprite_[i]->SetPosition(MathCalc::EaseInQuadF(t, positions_[i], positions_[i + 1]));
		sprite_[i]->SetSize(MathCalc::EaseInQuadF(t, sizes_[i], sizes_[i + 1]));
		sprite_[i]->Update();
	}
	if (t >= 1.0f) {
		stageNum_--;
		if (-1 == stageNum_) {
			stageNum_ = stageMax_ - 1;
		}
		TextureHandleChange();
	}

}

void StagePhot::TextureHandleChange()
{

	// stageNum_ == 0
	if (stageNum_ == 0) {
		sprite_[0]->SetTextureHandle(textureHandles_[stageMax_ - 2]);
		sprite_[1]->SetTextureHandle(textureHandles_[stageMax_ - 1]);
		sprite_[2]->SetTextureHandle(textureHandles_[stageNum_]);
		sprite_[3]->SetTextureHandle(textureHandles_[stageNum_ + 1]);
		sprite_[4]->SetTextureHandle(textureHandles_[stageNum_ + 2]);
	}
	// stageNum_ == 1
	else if (stageNum_ == 1) {
		sprite_[0]->SetTextureHandle(textureHandles_[stageMax_ - 1]);
		sprite_[1]->SetTextureHandle(textureHandles_[stageNum_ - 1]);
		sprite_[2]->SetTextureHandle(textureHandles_[stageNum_]);
		sprite_[3]->SetTextureHandle(textureHandles_[stageNum_ + 1]);
		sprite_[4]->SetTextureHandle(textureHandles_[stageNum_ + 2]);
	}
	// stageNum_ == stageMax_- 2
	else if (stageNum_ == stageMax_ - 2) {
		sprite_[0]->SetTextureHandle(textureHandles_[stageNum_ - 2]);
		sprite_[1]->SetTextureHandle(textureHandles_[stageNum_ - 1]);
		sprite_[2]->SetTextureHandle(textureHandles_[stageNum_]);
		sprite_[3]->SetTextureHandle(textureHandles_[stageNum_ + 1]);
		sprite_[4]->SetTextureHandle(textureHandles_[0]);
	}
	// stageNum_ == stageMax_- 1
	else if (stageNum_ == stageMax_ - 1) {
		sprite_[0]->SetTextureHandle(textureHandles_[stageNum_ - 2]);
		sprite_[1]->SetTextureHandle(textureHandles_[stageNum_ - 1]);
		sprite_[2]->SetTextureHandle(textureHandles_[stageNum_]);
		sprite_[3]->SetTextureHandle(textureHandles_[0]);
		sprite_[4]->SetTextureHandle(textureHandles_[1]);
	}
	// それ以外
	else {
		sprite_[0]->SetTextureHandle(textureHandles_[stageNum_ - 2]);
		sprite_[1]->SetTextureHandle(textureHandles_[stageNum_ - 1]);
		sprite_[2]->SetTextureHandle(textureHandles_[stageNum_]);
		sprite_[3]->SetTextureHandle(textureHandles_[stageNum_ + 1]);
		sprite_[4]->SetTextureHandle(textureHandles_[stageNum_ + 2]);
	}

	for (size_t i = 0; i < 5; i++) {
		sprite_[i]->SetPosition(positions_[i]);
		sprite_[i]->SetSize(sizes_[i]);
		sprite_[i]->Update();
	}

}
