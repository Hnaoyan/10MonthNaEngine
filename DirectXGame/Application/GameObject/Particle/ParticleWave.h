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

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(ViewProjection& viewProjection) override;

	void SetIsWave(bool flag) { isWave_ = flag; }

private:
	float wave_t_;
	bool isWave_ = false;

};

