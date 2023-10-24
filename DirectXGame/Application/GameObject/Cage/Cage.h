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
	void Initialize(Model* model, const Vector2& position);

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


};

