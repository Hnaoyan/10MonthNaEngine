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
	void Initialize(Model* model, Model* shadowModel, const Vector2& position);

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

	// 待機
	void WaitingAnimationInitialize();

	void WaitingAnimationUpdate();

	// 行動
	void ActionAnimationInitialize(uint32_t num);

	void ActionAnimationUpdate();

	// 移動
	void MoveAnimationInitialize(ActionNumber actionNumber);

	void MoveAnimationUpdate();

	// 移動ミス
	void MoveErrorAnimationInitialize(ActionNumber actionNumber);

	void MoveErrorAnimationUpdate();

	// 振動
	void VibrationAnimationInitialize();

	void VibrationAnimationUpdate();

	// クリア
	void ClearAnimationInitialize();

	void ClearAnimationUpdate();

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
	/// tマックス
	/// </summary>
	/// <returns></returns>
	uint32_t GetAnimationFrame() { return animationFrame_; }

	/// <summary>
	/// ゲットポジション
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldTransformPosition() { return Vector3(worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1], worldTransform_.matWorld_.m[3][2]); }

	/// <summary>
	/// 表示するか
	/// </summary>
	/// <param name="isDraw"></param>
	void SetIsDraw(bool isDraw) { isDraw_ = isDraw; }

private: // メンバ変数

	//ワールドトランスフォーム
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// 現在のマス
	Vector2 position_;

	// 表示するか
	bool isDraw_;

	// 影
	WorldTransform shadowWorldTransform_;
	// モデル
	Model* shadowModel_ = nullptr;

	// 行動アニメーションをした
	bool isActionAnimation_;

	// 行動アニメーション番号
	ActionNumber actionNumber_;

	// t
	float animationT_;
	// tマックス
	uint32_t animationFrame_;

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

	//移動中間スケール
	Vector3 moveAnimationMiddleScaleAdd_ = { 0.2f, 0.2f, 0.2f };

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
	Vector3 vibrationAnimationJumpScale_ = { 0.6f, 0.6f, 1.4f };
	// めり込みサイズ
	Vector3 vibrationAnimationFillScale_ = { 1.4f, 1.4f, 0.6f };

	// 振動エンド位置
	Vector3 vibrationAnimationHighPostionAdd_ = { 0.0f,0.0f, -15.0f };
	// 振動めり込み位置
	Vector3 vibrationAnimationFillPostionAdd_ = { 0.0f,0.0f, 2.0f };

private: // ClearAnimation

	// スタート位置
	Vector3 clearAnimationStartPostion_;
	// エンド位置
	Vector3 clearAnimationEndPostion_;
	// スタート角度
	Vector3 clearArnimationStartRotate_;
	// エンド角度
	Vector3 clearAnimationEndRotate_;

private: 

	// 移動T
	uint32_t moveAnimationT_ = 10;
	// 移動ミスT
	uint32_t moveErrorAnimationT_ = 20;
	// 振動T
	uint32_t vibrationAnimationT_ = 60;
	// クリアT
	uint32_t clearAnimationT_ = 60;


};
