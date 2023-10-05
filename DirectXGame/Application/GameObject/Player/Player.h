#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"

/// <summary>
/// プレイヤー
/// </summary>
class Player
{

public: // メンバ関数

	void Initialize(Model* model);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	void Move();

	void Jump();

private: // メンバ変数

	//ワールドトランスフォーム
	WorldTransform worldTransform_;

	// モデル
	Model* model_;

	// 速度
	Vector2 velocity_;

	// 加速度
	Vector2 acceleration_;

	// コライダーサイズ
	Vector2 colliderSize_;

	//着地判定
	bool islanding_;

private: // メンバ定数

	// ジャンプの初速度
	const float kJumpVelocity_ = 3.0f;

	// 落下加速度
	const float kFallingAcceleration = 0.2f;

};

