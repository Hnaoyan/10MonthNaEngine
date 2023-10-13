#include "ParticleWave.h"

void ParticleWave::Initialize(Model* model, uint32_t texture)
{

	Particle::Initialize(model, texture);

}

void ParticleWave::Update()
{

	Particle::Update();

}

void ParticleWave::Draw(ViewProjection& viewProjection)
{
	Particle::Draw(viewProjection);
}
