#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"

#include "Application/Others/RectangleCollider/RectangleCollider.h"

// 前方宣言
class Area;

/// <summary>
/// プレイヤー
/// </summary>
class Player
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize(Model* model);

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
	void Setting();

private: //メンバ関数

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// ジャンプ
	/// </summary>
	void Jump();

	/// <summary>
	/// 空中ジャンプ
	/// </summary>
	void MidairJump();

	/// <summary>
	/// 落下
	/// </summary>
	void Falling();

	/// <summary>
	/// 底に落ちる
	/// </summary>
	void FallToTheBottom();

public: // アクセッサ

	/// <summary>
	/// エリアセッター
	/// </summary>
	void SetArea(Area* area) { area_ = area; }

private: // メンバ変数

	//ワールドトランスフォーム
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// 速度
	Vector2 velocity_;

	// 加速度
	Vector2 acceleration_;

	// コライダー
	RectangleCollider collider_;

	//着地判定
	bool islanding_;

	//エリア
	Area* area_ = nullptr;

	// 空中ジャンプしたか
	bool isMidairJump_;

private: // メンバ定数

	// ジャンプの初速度
	const float kJumpVelocity_ = 1.5f;

	// 空中ジャンプの初速度
	const float kMidairJumpVelocity_ = 1.5f;

	// 最大移動速度
	const float kMoveVelocityMax_ = 0.5f;

	// 落下加速度
	const float kFallingAcceleration = -0.1f;

};

