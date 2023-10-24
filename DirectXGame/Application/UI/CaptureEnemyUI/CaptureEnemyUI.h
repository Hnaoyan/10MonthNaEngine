#pragma once
#include <cstdint>
#include <memory>
#include <Sprite.h>

/// <summary>
/// 捕まえた敵
/// </summary>
class CaptureEnemyUI
{

public: // メンバ関数


	void Initialize(uint32_t enemyCountTextureHandle, uint32_t numTextureHandle, uint32_t slashTextureHandle, uint32_t goGoalTextureHandle);

	void Update();

	void Draw();

	void Setting(uint32_t enemyMax);

	void EnemyCountUpdate(uint32_t enemyCount);

	void ActionAnimationInitialize();

	void ActionAnimationUpdate();

private: //メンバ関数

	/// <summary>
	/// 調整項目適用関数
	/// </summary>
	void ApplyGlobalVariables();

private: // メンバ変数

	// 説明(上のやつ)
	uint32_t enemyCountTextureHandle_;
	Vector2 enemyCountSize_;
	Vector2 enemyCountPosition_;
	std::unique_ptr<Sprite> enemyCountSprite_;

	// 数字*2
	uint32_t numTextureHandle_;
	Vector2 numSize_;
	// 分母
	Vector2 denominatorPosition_;
	std::unique_ptr<Sprite> denominatorSprite_;
	// 分子
	Vector2 numeratorPosition_;
	std::unique_ptr<Sprite> numeratorSprite_;

	// ／
	uint32_t slashTextureHandle_;
	Vector2 slashSize_;
	Vector2 slashPosition_;
	std::unique_ptr<Sprite> slashSprite_;

	// goGoal
	uint32_t goGoalTextureHandle_;
	Vector2 goGoalSize_;
	Vector2 goGoalPosition_;
	std::unique_ptr<Sprite> goGoalSprite_;

	// エネミー数
	uint32_t enemyCount_;
	// エネミー総数
	uint32_t enemyMax_;

	// アニメーション
	// 初期スケール
	Vector2 initEnemyCountSize_;
	// スケール中間
	Vector2 middleEnemyCountSize_;
	
	// 初期スケール
	Vector2 initNumSize_;
	// スケール中間
	Vector2 middleNumCountSize_;
	
	// 初期スケール
	Vector2 initSlashSize_;
	// スケール中間
	Vector2 middleSlashSize_;
	
	// 初期スケール
	Vector2 initGoGoalSize_;
	// スケール中間
	Vector2 middleGoGoalSize_;
	
	// t
	float animationT_;
	// フレーム
	uint32_t animationFrame_;

	// 倍率
	Vector2 sizeMultiply_ = { 1.5f,1.5f };
	// するか
	bool isAnimation_;

};
