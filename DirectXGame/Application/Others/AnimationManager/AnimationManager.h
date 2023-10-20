#pragma once
#include <list>
#include <functional>

/// <summary>
/// アニメーションマネージャー
/// </summary>
class AnimationManager
{
public: // サブクラス
	// アニメーション構造体
	struct AnimationStruct
	{
		// 行動アニメーション関数リスト
		std::list<std::function<void()>> animations_;

		// 行動アニメーションタイマー
		uint32_t animationTimer_;

		// 行動アニメーションタイム
		uint32_t animationTime_;

		// 行動アニメーションしているか
		bool isAnimation_;

		// 初期化
		void Initialize();
		
		// リセット関数
		void Reset();
	};


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
	void ActionStart(uint32_t animationTime);

	/// <summary>
	/// ゲームクリア時更新処理
	/// </summary>
	void GameClearUpdate();

	/// <summary>
	/// ゲームクリア開始時設定
	/// </summary>
	void GameClearStart();

	/// <summary>
	/// ゲームオーバー時更新処理
	/// </summary>
	void GameOverUpdate();

	/// <summary>
	/// ゲームオーバー開始時設定
	/// </summary>
	void GameOverStart();

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

	/// <summary>
	/// ゲームクリアアニメーション関数リスト追加
	/// </summary>
	/// <param name="function">関数</param>
	void SetGameClearAnimation(std::function<void()> function);

	/// <summary>
	/// ゲームオーバーアニメーション関数リスト追加
	/// </summary>
	/// <param name="function">関数</param>
	void SetGameOverAnimation(std::function<void()> function);

public: // アクセッサ

	// 行動アニメーションしているか
	bool GetIsActionAnimation() { return actionAnimation_.isAnimation_; }

	// ゲームクリアアニメーションしているか
	bool GetIsGameClearAnimation() { return gameClearAnimation_.isAnimation_; }

	// ゲームオーバーアニメーションしているか
	bool GetIsGameOverAnimation() { return gameOverAnimation_.isAnimation_; }

	// 行動アニメーションタイム設定
	void SetActionAnimationTime(uint32_t animationTime) { actionAnimation_.animationTime_ = animationTime; }

private: // メンバ変数

	// 待機アニメーション関数リスト
	std::list<std::function<void()>> waitingAnimations_;

	// 行動アニメーション
	AnimationStruct actionAnimation_;

	// ゲームクリアアニメーション
	AnimationStruct gameClearAnimation_;

	// ゲームオーバーアニメーション
	AnimationStruct gameOverAnimation_;

};

