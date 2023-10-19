#pragma once
#include "StructManager.h"
#include "ViewProjection.h"
#include "Particle.h"
#include <list>
#include <vector>
#include <optional>

class ParticleManager
{
public: // 基本
	ParticleManager();
	~ParticleManager() = default;

	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns></returns>
	static ParticleManager* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(ViewProjection* view);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// ランダムエフェクト
	/// </summary>
	/// <param name="point"></param>
	void RandomRespown(const Vector3& point);

public:

	void SetOpen(bool isOpen) { goalOpenParameters_.isNow = isOpen; }

private: // それぞれの管理変数
	// 管理用構造体
	struct ParticleEffectParameter {
		int frameCount, endCount;
		int addInterval_;
		bool isNow;
	};

	// 波
	ParticleEffectParameter waveParameters_ = { 0,0,0,false };

	Vector3 waveRespawnPosition_;

	uint32_t waveEffectTexture_ = 0u;

	// ゴール
	ParticleEffectParameter goalOpenParameters_ = { 0,0,0,false };

	Vector3 goalPosition_ = {};

	uint32_t goalEffectTexture_ = 0u;

	// 死亡

private: // パターンの更新
#pragma region パターンそれぞれの関数
	/// <summary>
	/// 波エフェクトの更新
	/// </summary>
	void WaveUpdate();
	/// <summary>
	/// ゴールが解放されてる時のエフェクト
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="scale"></param>
	void GoalEffectUpdate();

	/// <summary>
	/// 爆発エフェクトの初期化
	/// </summary>
	void ExplosionSetting();

	/// <summary>
	/// 爆発エフェクトの更新
	/// </summary>
	void ExplosionUpdate();

public: // 外部で呼び出す準備関数
	/// <summary>
	/// 波エフェクトの初期化
	/// </summary>
	void WaveSetting(const Vector3& position);

	/// <summary>
	/// ゴール解放時の設定
	/// </summary>
	void GoalEffectSetting(const Vector3& pos);

	/// <summary>
	/// 捕まえた際のエフェクト
	/// </summary>
	void CatchEnemyGenerate(const Vector3& pos);

#pragma endregion
private:
	/// <summary>
	/// パーティクルの更新処理
	/// </summary>
	void ParticleUpdate();

	/// <summary>
	/// 様々なパターンの処理
	/// </summary>
	void ParticleProcess();

	/// <summary>
	/// 波エフェクトの追加
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="velo"></param>
	void AddWave(const Vector3& pos, const Vector3& scale);
	/// <summary>
	/// 爆発エフェクトの追加
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="velo"></param>
	void AddExplosion(const Vector3& pos, const Vector3& velo);

	/// <summary>
	/// ゴール用エフェクトの追加
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="velo"></param>
	void AddGoalParticle(const Vector3& pos, const Vector3& velo, const Vector3& scale);

	/// <summary>
	/// かごに入った時のエフェクト
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="velo"></param>
	/// <param name="scale"></param>
	void AddSmokeParticle(const Vector3& pos, const Vector3& scale);

	/// <summary>
	/// json管理
	/// </summary>
	void ApplyGlobalVariables();

private:
	std::list<Particle*> particles_;

	uint32_t texture_ = 0u;

	std::unique_ptr<Model> planeModel_;
	std::unique_ptr<Model> cubeModel_;

	ViewProjection* view_ = nullptr;

};

