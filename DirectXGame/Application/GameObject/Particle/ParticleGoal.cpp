#include "ParticleGoal.h"
#include "VectorLib.h"

void ParticleGoal::Initialize(Model* model, uint32_t texture)
{
	Particle::Initialize(model, texture);
}

void ParticleGoal::Update()
{
	float decrement = 0.2f;
	worldTransform_.scale_ = VectorLib::Subtract(worldTransform_.scale_, Vector3(decrement, decrement, decrement));
	Particle::Update();
}

void ParticleGoal::Draw(ViewProjection& viewProjection)
{
	Particle::Draw(viewProjection);
}
