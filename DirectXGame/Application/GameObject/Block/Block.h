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
	kScaffold,
	kScaffoldColor,
	kPlayerAttack,
	kEnemyAttack
};

/// <summary>
/// ブロックの状態
/// </summary>
class BaseBlockState
{

public: // メンバ関数
	virtual ~BaseBlockState() {}
	virtual void Initialize(Block* pBlock) = 0;
	virtual void Update() = 0;

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
	void OnCollision(uint32_t collisonObj);

};

/// <summary>
/// ブロックの状態(足場)
/// </summary>
class BlockStateScaffoldColor : public BaseBlockState
{

public: // メンバ関数
	void Initialize(Block* pBlock) override;
	void Update()  override;
	void OnCollision(uint32_t collisonObj);

};

/// <summary>
/// ブロックの状態(プレイヤーの攻撃)
/// </summary>
class BlockStatePlayerAttack : public BaseBlockState
{

public: // メンバ関数
	void Initialize(Block* pBlock) override;
	void Update()  override;
	void OnCollision(uint32_t collisonObj);

};

/// <summary>
/// ブロックの状態(エネミーの攻撃)
/// </summary>
class BlockStateEnemyAttack : public BaseBlockState
{

public: // メンバ関数
	void Initialize(Block* pBlock) override;
	void Update()  override;
	void OnCollision(uint32_t collisonObj);

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

public: // アクセッサ

	bool IsDead() { return isDead_; }

	RectangleCollider GetCollider() { return collider_; }

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

};

