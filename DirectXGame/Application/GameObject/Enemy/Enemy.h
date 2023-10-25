#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"

#include "../Effect/EffectManager.h"

/// <summary>
/// エネミー
/// </summary>
class Enemy
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize(Model* model, const Vector2& position, Model* sleepModel, Model* surprisedModel, Model* shadowModel,
		uint32_t awakeEnemyTextureHandle, uint32_t sleepEnemyTextureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const Vector2& position, bool enemyAwake, bool captured);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection, bool isShadowDraw);

	/// <summary>
	/// 設定
	/// </summary>
	void Setting(const Vector2& position);

	void ActionAnimationInitialize();

	void ActionAnimationUpdate();

	// 待機アニメーション
	void WaitingAnimationInitialize();

	void WaitingAnimationUpdate();

	// ゲームオーバーアニメーション
	void GameOverAnimationInitialize(const Vector2& playerPosition);

	void GameOverAnimationUpdate();

private: //メンバ関数

	/// <summary>
	/// 調整項目適用関数
	/// </summary>
	void ApplyGlobalVariables();

public: // アクセッサ

	/// <summary>
	/// セットポジション
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(const Vector2& position) { position_ = position; }

	/// <summary>
	/// ゲットポジション
	/// </summary>
	/// <returns></returns>
	Vector2 GetPosition() { return position_; }

	void SetRotate(float rotate) { rotate_ = rotate; }

	void SetEndPosition(const Vector2& position) { animationEndPosition_ = position; }

	void SetScale(const Vector3& scale) { worldTransform_.scale_ = scale; }

private: // メンバ変数

	//ワールドトランスフォーム
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// 現在のマス
	Vector2 position_;

	// 起きているか
	bool awake_;

	// つかまってるか
	bool captured_;

	// 回転
	float rotate_;

	// テクスチャハンドル
	uint32_t awakeEnemyTextureHandle_ = 0u;
	uint32_t sleepEnemyTextureHandle_ = 0u;

	// アニメーションの開始座標
	Vector2 animationStartPosition_;
	// 最終座標
	Vector2 animationEndPosition_;
	// アニメーションの補間レート
	float animationT_;

	// スリープ
	//ワールドトランスフォーム
	WorldTransform sleepWorldTransform_;
	// モデル
	Model* sleepModel_ = nullptr;

	// スリープアニメーション
	// 位置
	Vector3 sleepStartPosition_;
	// 位置
	Vector3 sleepMiddlePosition_;
	// アニメーションt
	float sleepT_;
	// アニメーションフレーム
	uint32_t sleepFrame_;
	
	// ビックリ
	// ワールドトランスフォーム
	WorldTransform surprisedWorldTransform_;
	// モデル
	Model* surprisedModel_ = nullptr;
	// アニメーションt
	float surprisedT_;
	// アニメーションフレーム
	uint32_t surprisedFrame_;

	Vector3 surprisedStartPosition_;
	Vector3 surprisedEndPosition_;

	float surprisedZ_ = -10.0f;

	float surprisedSize_ = 0.7f;

	// 影
	WorldTransform shadowWorldTransform_;
	// モデル
	Model* shadowModel_ = nullptr;

	float shadowAddZ_;
	// アニメーションの開始座標
	float animationStartShadowAddZ_ = 2.4f;
	// 最終座標
	float animationShadowAddZ_;

	// ゲームオーバーアニメーション
	Vector3 gameOverPositionStart_;
	Vector3 gameOverPositionMiddle_;
	float gameOverT_;
	uint32_t gameOverFrame_;
	bool isGameOverAnimation_ = false;

};

