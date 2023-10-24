#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"

class CloudObject
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	void Initialize(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="view"></param>
	void Draw(ViewProjection& view);

private:
	// 消すフラグ
	bool isDead_ = false;

	// 移動速度
	Vector3 velocity_ = {};

	// モデル
	Model* model_ = nullptr;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;

public:	// アクセッサ
	/// <summary>
	/// 設定
	/// </summary>
	/// <param name="dead"></param>
	void SetIsDead(bool dead) { isDead_ = dead; }

	bool GetIsDead() { return isDead_; }

	/// <summary>
	/// 位置の取得・設定
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(const Vector3& position) { worldTransform_.translation_ = position; }

	Vector3 GetPosition() { return worldTransform_.translation_; }

	/// <summary>
	/// スケール
	/// </summary>
	/// <param name="scale"></param>
	void SetScale(const Vector3& scale) { worldTransform_.scale_ = scale; }

	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }

};

