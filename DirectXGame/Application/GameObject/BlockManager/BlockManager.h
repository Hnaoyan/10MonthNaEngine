#pragma once
#include <list>
#include <vector>
#include "Application/GameObject/Block/Block.h"
#include "Application/Others/TimedCall/TimedCall.h"

//前方宣言
class Area;

/// <summary>
/// ブロックマネージャー
/// </summary>
class BlockManager
{

public: // メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	~BlockManager();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, std::vector<uint32_t> textureHandles);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 設定
	/// </summary>
	void Setting();

	/// <summary>
	/// ブロック削除
	/// </summary>
	void DeleteBlock();

	/// <summary>
	/// ブロック発射
	/// </summary>
	void BlockFiring();

private: // メンバ関数

	/// <summary>
	/// 足場ブロック生成
	/// </summary>
	void ScaffoldBlockGenerate();

	/// <summary>
	/// 足場ブロック生成セット
	/// </summary>
	void SetScaffoldBlockGenerateTimer();

	/// <summary>
	/// エネミーの攻撃ブロック生成
	/// </summary>
	void EnemyAttackBlockGenerate();

public: // アクセッサ

	/// <summary>
	/// エリアセッター
	/// </summary>
	/// <param name="area"></param>
	void SetArea(Area* area) { area_ = area; }

	/// <summary>
	/// ブロックゲッター
	/// </summary>
	std::list<Block*> GetBlocks() { return blocks_; }

	/// <summary>
	/// テクスチャハンドルゲッター
	/// </summary>
	/// <returns></returns>
	std::vector<uint32_t> GetTextureHandles() { return textureHandles_; }

	/// <summary>
	/// コライダーサイズゲッター
	/// </summary>
	/// <returns></returns>
	Vector2 GetColliderSize() { return colliderSize_; }

private: // メンバ変数

	// ブロック
	std::list<Block*> blocks_;

	//モデル
	Model* model_ = nullptr;

	//時限発動のリスト
	std::list<TimedCall*> timedCalls_;

	// エリア
	Area* area_ = nullptr;

	// コライダーサイズ
	Vector2 colliderSize_;

	// 足場ブロック生成インターバル
	int scaffoldBlockGenerateInterval_;

	// テクスチャハンドル
	std::vector<uint32_t> textureHandles_;

};

