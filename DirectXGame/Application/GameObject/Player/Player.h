#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"

#include "Application/Others/RectangleCollider/RectangleCollider.h"

// 前方宣言
class Area;
class BlockManager;

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

	/// <summary>
	/// 衝突を検出したら呼び出されるコールバック関数
	/// </summary>
	/// <param name="collisonObj">衝突したOBJ</param>
	void OnCollision(uint32_t collisonObj, WorldTransform* worldTransform);

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

	/// <summary>
	/// 足場ブロックと衝突
	/// </summary>
	void OnCollisionBlock(WorldTransform* worldTransform);

	/// <summary>
	/// 調整項目適用関数
	/// </summary>
	void ApplyGlobalVariables();

public: // アクセッサ

	/// <summary>
	/// エリアセッター
	/// </summary>
	void SetArea(Area* area) { area_ = area; }

	/// <summary>
	/// ブロックマネージャーセッター
	/// </summary>
	void SetBlockManager(BlockManager* blockManager) { blockManager_ = blockManager; }

	/// <summary>
	/// コライダーアドレスゲッター
	/// </summary>
	/// <returns></returns>
	RectangleCollider* GetColliderAddress() { return &collider_; }

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

	// 空中ジャンプしたか
	bool isMidairJump_;

private: // ポインタ

	//エリア
	Area* area_ = nullptr;
	
	// ブロックマネージャー
	BlockManager* blockManager_ = nullptr;

private: // メンバ定数

	// ジャンプの初速度
	float kJumpVelocity_ = 1.5f;

	// 空中ジャンプの初速度
	float kMidairJumpVelocity_ = 1.5f;

	// 最大移動速度
	float kMoveVelocityMax_ = 0.5f;

	// 落下加速度
	float kFallingAcceleration_ = -0.1f;

};

