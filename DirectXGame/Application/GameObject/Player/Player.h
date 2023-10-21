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
		kNone,
		kLeft,
		kRight,
		kUp,
		kDown,
		kLeftError,
		kRightError,
		kUpError,
		kDownError,
		kVibration
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

	void MoveErrorAnimationInitialize(ActionNumber actionNumber);

	void MoveErrorAnimationUpdate();

	void VibrationAnimationInitialize();

	void VibrationAnimationUpdate();

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
	/// tマックス(uint32_t)
	/// </summary>
	/// <returns></returns>
	uint32_t GetAnimationTMax() { return static_cast<uint32_t>(animationTMax_); }

	/// <summary>
	/// ゲットポジション
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldTransformPosition() { return Vector3(worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1], worldTransform_.matWorld_.m[3][2]); }
private: // メンバ変数

	//ワールドトランスフォーム
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// 現在のマス
	Vector2 position_;

	// 行動アニメーションをした
	bool isActionAnimation_;

	// 行動アニメーション番号
	ActionNumber actionNumber_;

	// t
	float animationT_;
	// tマックス
	float animationTMax_;

private: // MoveAnimation
	// スタート角度
	Vector3 moveAnimationStartRotate_;
	// エンド角度
	Vector3 moveAnimationEndRotate_;
	// 親
	WorldTransform moveAnimationWorldTransform_;

	// スタートスケール
	Vector3 moveAnimationStartScale_;
	// 中間スケール
	Vector3 moveAnimationMiddleScale_;

private: // VibrationAnimation
	// スタート位置
	Vector3 vibrationAnimationGroundPostion_;
	// エンド位置
	Vector3 vibrationAnimationHighPostion_;
	// めり込み位置
	Vector3 vibrationAnimationFillPostion_;
	// スタート角度
	Vector3 vibrationAnimationStartRotate_;
	// エンド角度
	Vector3 vibrationAnimationEndRotate_;
	// ジャンプサイズ
	Vector3 vibrationAnimationJumpScale_;
	// めり込みサイズ
	Vector3 vibrationAnimationFillScale_;

};
