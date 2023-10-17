#pragma once
#include <list>
#include <functional>

/// <summary>
/// アニメーションマネージャー
/// </summary>
class AnimationManager
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 待機時更新処理
	/// </summary>
	void WaitUpdate();

	/// <summary>
	/// 行動時更新処理
	/// </summary>
	void ActionUpdate();

	/// <summary>
	/// 行動開始時設定
	/// </summary>
	void ActionStart();

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// 待機アニメーション関数リスト追加
	/// </summary>
	/// <param name="function">関数</param>
	void SetWaitingAnimation(std::function<void()> function);

	/// <summary>
	/// 行動アニメーション関数リスト追加
	/// </summary>
	/// <param name="function">関数</param>
	void SetActionAnimation(std::function<void()> function);

private: // メンバ変数

	// 待機アニメーション関数リスト
	std::list<std::function<void()>> waitingAnimations_;

	// 行動アニメーション関数リスト
	std::list<std::function<void()>> actionAnimations_;
	
	// 行動アニメーションタイマー
	uint32_t actionAnimationTimer_;

	// 行動アニメーションタイム
	uint32_t actionAnimationTime_;

	// 行動アニメーションしているか
	bool isActionAnimation_;

};

