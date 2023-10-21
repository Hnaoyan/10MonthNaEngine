#include "ParticleExplosion.h"
#include "VectorLib.h"

void ParticleExplosion::Initialize(Model* model, uint32_t texture)
{
	Particle::Initialize(model, texture);
	model_->SetAlphaValue(0.3f);
	model_->SetBlendMode(Model::BlendMode::kScreen);
	fadeTimer_ = 60;
}

void ParticleExplosion::Update()
{
	this->timeElapsed_++;
	if (timeElapsed_ >= fadeTimer_) {
		isDead_ = true;
	}

	worldTransform_.scale_ = VectorLib::Subtract(worldTransform_.scale_, Vector3(0.01f, 0.01f, 0.01f));
	velocity_.z += 0.1f;
	velocity_ = VectorLib::Scaler(velocity_, 0.95f);
	worldTransform_.translation_ = VectorLib::Add(worldTransform_.translation_, velocity_);
	worldTransform_.UpdateMatrix();
	//Particle::Update();
}

void ParticleExplosion::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

void ParticleExplosion::SetInitialSpeed(float speed)
{
	speed;
}
