#include "ParticleWave.h"

void ParticleWave::Initialize(Model* model, uint32_t texture)
{

	Particle::Initialize(model, texture);

}

void ParticleWave::Update()
{

	Particle::Update();
	worldTransform_.scale_.x += 0.02f;
	worldTransform_.scale_.y += 0.02f;
	//float alpha = model_->GetAlphaValue() - 0.02f;
	//model_->SetAlphaValue(alpha);

}

void ParticleWave::Draw(ViewProjection& viewProjection)
{
	Particle::Draw(viewProjection);
}
