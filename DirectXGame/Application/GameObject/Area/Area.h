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

public: // メンバ定数

	// ドロップライン
	const float kDropLine_ = 8.0f;

	//最大位置
	const Vector2 kPositionMax_ = {20.0f, 40.0f};

	//最小位置
	const Vector2 kPositionMin_ = {0.0f, 0.0f};


private: // メンバ変数

	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	// モデル
	Model* model_;

};

