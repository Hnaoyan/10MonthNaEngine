#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
#include <Application/Others/TimedCall/TimedCall.h>
#include <Application/Others/RectangleCollider/RectangleCollider.h>
#include <functional>

// 前方宣言
class BlockManager;

/// <summary>
/// ボスエネミー
/// </summary>
class BossEnemy
{

public: // メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	~BossEnemy();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize(Model* model, BlockManager* blockManager);

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

private: // メンバ関数

	/// <summary>
	/// ブロック落とす
	/// </summary>
	void DropBlock();

	/// <summary>
	/// ブロック落とすのタイマー設定
	/// </summary>
	void SetDropBlockTimer();

	/// <summary>
	/// ダメージ
	/// </summary>
	void Damage();

public: // アクセッサ

	/// <summary>
	/// ブロックマネージャーゲッター
	/// </summary>
	/// <returns></returns>
	BlockManager* GetBlockManager() { return blockManager_; }

	/// <summary>
	/// 死んでるか
	/// </summary>
	/// <returns></returns>
	bool IsDead() { return isDead_; }

	/// <summary>
	/// 死んでるかセッター
	/// </summary>
	/// <param name="isDead"></param>
	void SetIsDead(bool isDead) { isDead_ = isDead; }

	/// <summary>
	/// コライダーゲッター
	/// </summary>
	/// <returns></returns>
	RectangleCollider GetCollider() { return collider_; }

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

	// コライダー
	RectangleCollider collider_;

	// HP
	int32_t hp_;

	//時限発動のリスト
	std::list<TimedCall*> timedCalls_;

	// エネミーの攻撃関数
	std::function<void()> enemyAttackFunc_;

	// ブロックマネージャー
	BlockManager* blockManager_ = nullptr;

	// 死亡フラグ
	bool isDead_;

private: // メンバ定数

	// ブロック落とすインターバルベース
	int kDropBlockInterval_ = 300;

	// 初期Hp
	uint32_t kInitialHp_ = 10;

};
