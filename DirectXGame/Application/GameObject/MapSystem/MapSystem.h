#pragma once
#include "StructManager.h"
#include <vector>

/// <summary>
/// マップシステムクラス
/// </summary>
class MapSystem
{

public: // サブクラス
	// マップ番号
	enum MapNumber {
		Wall,
		Road,
		Hole,
	};

public: // 静的メンバ定数

	// マップのサイズ
	static const Vector2 kMapSize_;

	// マスのサイズ
	static const Vector2 kSquareSize_;

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

private: // メンバ関数

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// ブロックが落ちる
	/// </summary>
	void BlockFall();

	/// <summary>
	/// プレイヤーの移動
	/// </summary>
	void PlayerMove();

	/// <summary>
	/// エネミーの移動
	/// </summary>
	void EnemyMove();

	/// <summary>
	/// 音を鳴らす
	/// </summary>
	void MakeSound();

	/// <summary>
	/// ゲームクリア(ゴールについたか)
	/// </summary>
	void GameClear();

	/// <summary>
	/// ゲームオーバー(敵にぶつかってしまった)
	/// </summary>
	void GameOver();
	
	/// <summary>
	/// リスタート
	/// </summary>
	void Restart();

	/// <summary>
	/// 設定
	/// </summary>
	void Setting();

public: // メンバ定数

private: //メンバ変数

	// ステージ番号
	int stageNum_;

	// マップ
	int** map_;

	// 初期マップ
	int** InitialMap_;

	// プレイヤーの位置
	Vector2 playerPosition_;

	// エネミーの位置
	std::vector<Vector2> enemyPosition_;

	// 檻の位置
	std::vector<Vector2> cagePosition_;

	// スタートの位置
	Vector2 startPosition_;

	// ゴールの位置
	Vector2 goalPosition_;

	// ゴールが開いたか
	bool goalOpened_;
	
	// 敵を捕まえた
	std::vector<bool> capturedEnemy_;

};

