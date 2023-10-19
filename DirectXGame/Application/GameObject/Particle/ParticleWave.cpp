#include "ParticleWave.h"
#include "MathCalc.h"

void ParticleWave::Initialize(Model* model, uint32_t texture)
{

	Particle::Initialize(model, texture);
	wave_t_ = 0;
	isWave_ = false;
}

void ParticleWave::Update()
{
	if (isWave_) {
		wave_t_ += 0.01f;
		float amp = 0.015f;
		float fre = 5.0f;
		velocity_.y = amp * std::cosf(1.0f * float(std::numbers::pi) * fre * wave_t_);
	}
	Particle::Update();
	worldTransform_.scale_.x += 0.02f;
	worldTransform_.scale_.y += 0.02f;

}

void ParticleWave::Draw(ViewProjection& viewProjection)
{
	Particle::Draw(viewProjection);
}
