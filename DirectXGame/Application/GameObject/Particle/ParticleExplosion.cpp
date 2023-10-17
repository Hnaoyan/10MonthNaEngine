#include "ParticleExplosion.h"
#include "VectorLib.h"

void ParticleExplosion::Initialize(Model* model, uint32_t texture)
{
	Particle::Initialize(model, texture);

}

void ParticleExplosion::Update()
{
	worldTransform_.scale_ = VectorLib::Subtract(worldTransform_.scale_, Vector3(0.01f, 0.01f, 0.01f));
	Particle::Update();
}

void ParticleExplosion::Draw(ViewProjection& viewProjection)
{
	//Particle::Draw(viewProjection);
	model_->Draw(worldTransform_, viewProjection);
}
