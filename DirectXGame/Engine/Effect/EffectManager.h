#pragma once
#include "ViewProjection.h"

class EffectManager
{
public:
	/// <summary>
	/// インスタンス
	/// </summary>
	/// <returns></returns>
	static EffectManager* GetInstance();

	void Initialize();

	void Update();

private:
	// ビュー
	ViewProjection* view_ = nullptr;

	// エフェクト管理の構造体
	struct EffectParameters {
		bool isFlag;
		int countTimer;
		int endTimer;
	};
	/// <summary>
	/// エフェクト管理構造体を使った更新処理
	/// </summary>
	/// <param name="type"></param>
	void Update(EffectParameters type);

public:

	/// <summary>
	/// ヒットストップ更新
	/// </summary>
	void HitStopUpdate();

	/// <summary>
	/// シェイクの更新
	/// </summary>
	void ShakeUpdate();

	/// <summary>
	/// シェイクの座標計算
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="rangeMax"></param>
	/// <param name="rangeMin"></param>
	/// <returns></returns>
	static Vector3 ShakeUpdate(const Vector3& pos, int rangeMax, int rangeMin);

	/// <summary>
	/// シェイクの縦横の幅の差
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="rangeMax"></param>
	/// <param name="rangeMin"></param>
	/// <returns></returns>
	static Vector3 ShakeUpdate(const Vector3& pos, const Vector2& rangeMax, const Vector2& rangeMin);
#pragma region ヒットストップ系

private: // ヒットストップ系
	EffectParameters hitStop_;

public:
	/// <summary>
	/// ヒットストップ設定
	/// </summary>
	/// <param name="flag"></param>
	void SetIsStop(bool flag) { hitStop_.isFlag = flag; }

	/// <summary>
	/// ヒットストップ取得
	/// </summary>
	/// <returns></returns>
	bool IsStop() { return hitStop_.isFlag; }

#pragma endregion
#pragma region シェイク系

private: // シェイク関係
	EffectParameters shake_;
	Vector3 initPoint_{};
public:
	/// <summary>
	/// シェイク設定
	/// </summary>
	/// <param name="flag"></param>
	void SetIsShake(bool flag) { shake_.isFlag = flag; }

	/// <summary>
	/// シェイク取得
	/// </summary>
	/// <returns></returns>
	bool IsShake() { return shake_.isFlag; }

#pragma endregion

	/// <summary>
	/// 調整項目適用関数
	/// </summary>
	void ApplyGlobalVariables();

};

