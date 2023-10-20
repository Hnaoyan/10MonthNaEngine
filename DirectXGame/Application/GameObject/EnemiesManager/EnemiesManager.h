#pragma once
#include "Application/GameObject/Enemy/Enemy.h"
#include "Application/GameObject/EnemyMovePlan/EnemyMovePlan.h"
#include "Application/GameObject/Cage/Cage.h"

#include <vector>

// 前方宣言
class MapSystem;

/// <summary>
/// エネミーズマネージャー
/// </summary>
class EnemiesManager
{

public: // メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemiesManager();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="mapSystem">マップシステム</param>
	/// <param name="enemyModel">エネミーモデル</param>
	/// <param name="cageModel">ケージモデル</param>
	void Iintialize(MapSystem* mapSystem, Model* enemyModel, Model* enemyMovePlanModel, Model* cageModel, Model* enemyDangerModel, size_t enemyCount, size_t cageCount);

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
	void Setting(size_t enemyCount, size_t cageCount);

	/// <summary>
	/// エネミー追加
	/// </summary>
	/// <param name="num">番号</param>
	void AddEnemy(size_t num);

	/// <summary>
	/// 檻追加
	/// </summary>
	/// <param name="num">番号</param>
	void AddCage(size_t num);

	/// <summary>
	/// エネミー情報取得
	/// </summary>
	/// <param name="posision">位置</param>
	/// <returns></returns>
	Enemy* GetEnemy(Vector2 posision);

	/// <summary>
	/// エネミー行動計画情報取得
	/// </summary>
	/// <param name="posision">位置</param>
	/// <returns></returns>
	EnemyMovePlan* GetEnemyMovePlan(Vector2 posision);

	/// <summary>
	/// 檻情報取得
	/// </summary>
	/// <param name="posision">位置</param>
	/// <returns></returns>
	Cage* GetCage(Vector2 posision);

private: //メンバ関数

	/// <summary>
	///  エネミー危険範囲更新
	/// </summary>
	void EnemyDangerUpdate(int x, int y);

private: // メンバ変数

	// エネミーズ
	std::vector<Enemy*> enemies_;
	// モデル
	Model* enemyModel_ = nullptr;

	// エネミーの移動計画
	std::vector<EnemyMovePlan*> enemyMovePlans_;
	// モデル
	Model* enemyMovePlanModel_;

	// 檻
	std::vector<Cage*> cages_;
	// モデル
	Model* cageModel_ = nullptr;

	// エネミー危険範囲
	std::list<WorldTransform> enemyDangerPositions_;
	// モデル
	Model* enemyDangerModel_;

	// マップシステム
	MapSystem* mapSystem_;

};

