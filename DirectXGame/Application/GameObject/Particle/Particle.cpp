#include "Particle.h"
#include "VectorLib.h"

void Particle::Initialize(Model* model, uint32_t texture) 
{
	// モデル設定
	model_ = model;
	// テクスチャ設定
	texture_ = texture;
	// 座標系設定
	worldTransform_.Initialize();
}

void Particle::Update() 
{
	timeElapsed_++;
	if (timeElapsed_ > fadeTimer_) {
		isDead_ = true;
	}
	worldTransform_.translation_ = VectorLib::Add(worldTransform_.translation_, velocity_);
	worldTransform_.UpdateMatrix();
}


void Particle::Draw(ViewProjection& viewProjection) 
{
	model_->Draw(worldTransform_, viewProjection, texture_);
}