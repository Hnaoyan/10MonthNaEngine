#pragma once
#include "Particle.h"

class ParticleExplosion : public Particle
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

	/// <summary>
	/// 初速度設定
	/// </summary>
	/// <param name="speed"></param>
	void SetInitialSpeed(float speed);

private:
	float initialSpeedY_ = 3.0f;

};

