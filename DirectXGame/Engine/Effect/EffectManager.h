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

	/// <summary>
	/// ヒットストップ更新
	/// </summary>
	void HitStopUpdate();

	void ShakeUpdate() {};

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

private:

	ViewProjection* view_ = nullptr;

private: // ヒットストップ系
	// ストップフラグ
	bool isStop_ = false;
	// ストップカウントタイマー
	int stopTimer_ = 0;
	// 終了タイム
	const int kHitStopTimer = 20;
public:
	/// <summary>
	/// ヒットストップ設定
	/// </summary>
	/// <param name="flag"></param>
	void SetIsStop(bool flag) { isStop_ = flag; }

	/// <summary>
	/// ヒットストップ取得
	/// </summary>
	/// <returns></returns>
	bool IsStop() { return isStop_; }

};

