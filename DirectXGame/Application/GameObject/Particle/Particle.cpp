#include "Particle.h"
#include "VectorLib.h"

void Particle::Initialize(Model* model, uint32_t texture) 
{
	// モデル設定
	model_ = model;
	model_->SetAlphaValue(1.0f);
	// テクスチャ設定
	texture_ = texture;
	// 座標系設定
	worldTransform_.Initialize();
	scale_ = { 1.0f,1.0f,1.0f };
}

void Particle::Update() 
{
	timeElapsed_++;
	if (timeElapsed_ > fadeTimer_) {
		isDead_ = true;
	}
	worldTransform_.scale_.x += 0.04f;
	worldTransform_.scale_.y += 0.04f;
	float alpha = model_->GetAlphaValue() - 0.05f;
	model_->SetAlphaValue(alpha);
	worldTransform_.translation_ = VectorLib::Add(worldTransform_.translation_, velocity_);
	worldTransform_.UpdateMatrix();
}


void Particle::Draw(ViewProjection& viewProjection) 
{
	model_->Draw(worldTransform_, viewProjection, texture_);
}