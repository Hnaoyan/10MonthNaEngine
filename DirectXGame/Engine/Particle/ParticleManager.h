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

	void Test(const Vector3& point);

public:

	enum class PatternNum {
		kNone,	// 通常
		kMove,	// 移動時
		kVibration,	// 振動時
		kExplosion,	// 破裂
	};

public:

	void SetRequest(PatternNum number) { patternRequest_ = number; }

	void SetOpen(bool isOpen) { goalOpen_.isNow = isOpen; }

private: // enumクラスの管理系
	/// <summary>
	/// 分岐変数
	/// </summary>
	PatternNum pattern_ = PatternNum::kNone;
	/// <summary>
	/// リクエスト
	/// </summary>
	std::optional<PatternNum> patternRequest_ = std::nullopt;

	struct PatternControl {
		int frameCount, endCount;
		bool isNow;
	};

	PatternControl waveTimer_;

private: // パターンの更新と初期化関数
#pragma region パターンそれぞれの関数
	/// <summary>
	/// 波エフェクトの初期化
	/// </summary>
	void WaveInitialize();

	/// <summary>
	/// 波エフェクトの更新
	/// </summary>
	void WaveUpdate();

	/// <summary>
	/// 爆発エフェクトの初期化
	/// </summary>
	void ExplosionInitialize();

	/// <summary>
	/// 爆発エフェクトの更新
	/// </summary>
	void ExplosionUpdate();

public:
	/// <summary>
	/// ゴール解放時の設定
	/// </summary>
	void GoalEffectSetting(const Vector3& pos);

	/// <summary>
	/// ゴールが解放されてる時のエフェクト
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="scale"></param>
	void GoalEffectUpdate();

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

	Vector3 goalPosition_ = {};
	PatternControl goalOpen_ = { 0,30,false };
};

