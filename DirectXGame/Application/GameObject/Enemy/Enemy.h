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
	void Initialize(Model* model, const Vector2& position, Model* sleepModel);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const Vector2& position, bool enemyAwake);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 設定
	/// </summary>
	void Setting(const Vector2& position);

	void ActionAnimationInitialize();

	void ActionAnimationUpdate();

	// 待機アニメーション
	void WaitingAnimationInitialize();

	void WaitingAnimationUpdate();

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

private: // メンバ変数

	//ワールドトランスフォーム
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// 現在のマス
	Vector2 position_;

	// 起きているか
	bool awake_;

	float rotate_;

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
	
	// 行動アニメーションをしたか
	bool isActionAnimation_;

};

