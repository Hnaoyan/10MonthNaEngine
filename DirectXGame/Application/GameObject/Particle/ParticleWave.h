#pragma once
#include "Particle.h"


class ParticleWave : public Particle
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="texture"></param>
	void Initialize(Model* model, uint32_t texture) override;

	void Update() override;

	void Draw(ViewProjection& viewProjection) override;

};

