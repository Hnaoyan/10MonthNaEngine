#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"

/// <summary>
/// ゲームエリア
/// </summary>
class Area
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

public: // メンバ定数

	// ドロップライン
	const float dropLine_ = 8.0f;

	//最大位置
	const Vector2 translationMax_ = {10.0f, 20.0f};

	//最小位置
	const Vector2 translationMin_ = { -10.0f, -20.0f };


private: // メンバ変数

	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	// モデル
	Model* model_;

};

