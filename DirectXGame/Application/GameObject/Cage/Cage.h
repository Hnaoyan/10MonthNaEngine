#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"

#include "../Effect/EffectManager.h"

/// <summary>
/// 檻
/// </summary>
class Cage
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize(Model* model, Model* shadowModel,
		const Vector2& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 設定
	/// </summary>
	void Setting(const Vector2& position);

	/// <summary>
	/// 
	/// </summary>
	void ActionAnimationInitialize();

	/// <summary>
	/// 
	/// </summary>
	void ActionAnimationUpdate();

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

private: // メンバ変数

	//ワールドトランスフォーム
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// 現在のマス
	Vector2 position_;

	
	float positionZ_ = -20.0f;

	// アニメーション
	// するか
	bool isAnimation_;
	// 初期スケール
	Vector3 initScale_;
	// スケール中間
	Vector3 middleScale_;
	// t
	float animationT_;
	// フレーム
	uint32_t animationFrame_;

	Vector3 scaleAdd_ = { 4.0f,4.0f,4.0f };

	// 影
	WorldTransform shadowWorldTransform_;
	// モデル
	Model* shadowModel_ = nullptr;

	float shadowAddZ_;

};

