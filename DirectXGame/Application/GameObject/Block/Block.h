#pragma once

#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"

#include "Application/Others/RectangleCollider/RectangleCollider.h"

//前方宣言
class Block;

/// <summary>
/// ブロックの状態
/// </summary>
enum BlockState
{
	kScaffold, //足場
	kScaffoldColor, //色のついた足場
	kPlayerAttack, //プレイヤーの攻撃
	kEnemyAttack // エネミーの攻撃
};

/// <summary>
/// ブロックの状態
/// </summary>
class BaseBlockState
{

public: // メンバ関数
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BaseBlockState() {}

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="pBlock">ポインタ</param>
	virtual void Initialize(Block* pBlock) = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 衝突を検出したら呼び出されるコールバック関数
	/// </summary>
	/// <param name="collisonObj">衝突したOBJ</param>
	virtual void OnCollision(uint32_t collisonObj) = 0;

protected: // 継承メンバ変数

	//ポインタ
	Block* pBlock_ = nullptr;

};

/// <summary>
/// ブロックの状態(足場)
/// </summary>
class BlockStateScaffold : public BaseBlockState
{

public: // メンバ関数
	void Initialize(Block* pBlock) override;
	void Update()  override;
	void OnCollision(uint32_t collisonObj) override;

};

/// <summary>
/// ブロックの状態(足場)
/// </summary>
class BlockStateScaffoldColor : public BaseBlockState
{

public: // メンバ関数
	void Initialize(Block* pBlock) override;
	void Update()  override;
	void OnCollision(uint32_t collisonObj) override;

};

/// <summary>
/// ブロックの状態(プレイヤーの攻撃)
/// </summary>
class BlockStatePlayerAttack : public BaseBlockState
{

public: // メンバ関数
	void Initialize(Block* pBlock) override;
	void Update()  override;
	void OnCollision(uint32_t collisonObj) override;

};

/// <summary>
/// ブロックの状態(エネミーの攻撃)
/// </summary>
class BlockStateEnemyAttack : public BaseBlockState
{

public: // メンバ関数
	void Initialize(Block* pBlock) override;
	void Update()  override;
	void OnCollision(uint32_t collisonObj) override;

};

class Block
{

public: // メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Block();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize(Model* model, BlockState blockstate, const Vector3& transfrom, const Vector2& colliderSize);

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
	/// 状態変更
	/// </summary>
	/// <param name="newState">新しい状態</param>
	void ChangeState(BaseBlockState* newState);

	/// <summary>
	/// 足場上昇
	/// </summary>
	void ScaffoldRise();

	/// <summary>
	/// 衝突を検出したら呼び出されるコールバック関数
	/// </summary>
	/// <param name="collisonObj">衝突したOBJ</param>
	void OnCollision(uint32_t collisonObj, WorldTransform* worldTransform);

public: // アクセッサ

	/// <summary>
	/// 死んでるか
	/// </summary>
	/// <returns></returns>
	bool IsDead() { return isDead_; }

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

	/// <summary>
	/// 衝突コールバックセッター
	/// </summary>
	/// <param name="function">関数</param>
	void SetCollisionFunction(std::function<void(uint32_t, WorldTransform*)> collisionFunction) { collisionFunction_ = collisionFunction; }

	/// <summary>
	/// 衝突コールバックゲッター
	/// </summary>
	/// <param name="function">関数</param>
	std::function<void(uint32_t, WorldTransform*)> GetCollisionFunction() { return collisionFunction_; }

private: // メンバ変数

	//ワールドトランスフォーム
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// 速度
	Vector2 velocity_;

	//コライダー
	RectangleCollider collider_;

	// 状態
	BaseBlockState* state_;

	// 状態名
	BlockState stateName_;

	// 死亡フラグ
	bool isDead_;

	// 衝突コールバック
	std::function<void(uint32_t, WorldTransform*)> collisionFunction_;

};

