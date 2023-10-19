#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"

#include "../Effect/EffectManager.h"

// 前方宣言
class Area;
class BlockManager;

/// <summary>
/// プレイヤー
/// </summary>
class Player
{

public: // サブクラス

	// 移動番号
	enum class ActionNumber{
		None,
		kLeft,
		kRight,
		kTop,
		kBottom
	};

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize(Model* model, const Vector2& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const Vector2& position);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 設定
	/// </summary>
	void Setting(const Vector2& position);

public: // アニメーション

	void WaitingAnimationInitialize();

	void WaitingAnimationUpdate();

	void ActionAnimationInitialize(uint32_t num);

	void ActionAnimationUpdate();

	void MoveAnimationInitialize(ActionNumber actionNumber);

	void MoveAnimationUpdate();


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

private: // メンバ変数

	//ワールドトランスフォーム
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// 現在のマス
	Vector2 position_;

	// MoveAnimation
	// スタート角度
	Vector3 moveAnimationStartRotate_;
	// エンド角度
	Vector3 moveAnimationEndRotate_;
	// t
	float moveAnimationT_;
	// tマックス
	float moveAnimationTMax_;
	// 親
	WorldTransform moveAnimationWorldTransform_;

	// 行動アニメーションをした
	bool isActionAnimation_;

	// 行動アニメーション番号
	ActionNumber actionNumber_;

};

