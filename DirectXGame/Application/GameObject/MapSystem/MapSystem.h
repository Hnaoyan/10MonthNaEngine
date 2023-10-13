#pragma once
#include "StructManager.h"
#include <vector>
#include <map>
#include <string>
#include <variant>

/// <summary>
/// マップシステムクラス
/// </summary>
class MapSystem
{

public: // サブクラス
	// マップ番号
	enum MapNumber {
		Wall, // 壁
		Road, // 道
		Hole, // 穴
	};

	// ステージデータ
	struct StageData
	{
		// マップ
		int** map_;
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
	};

public: // 静的メンバ定数

	// マップのサイズ
	static const Vector2 kMapSize_;

	// マスのサイズ
	static const Vector2 kSquareSize_;

	// ステージ数
	static const uint32_t kMaximumNumberOfStages_;

public: // メンバ関数
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~MapSystem();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(int stageNum);

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
	void Setting(int stageNum);

	/// <summary>
	/// マップ読み込み
	/// </summary>
	void StagesLoad();

	/// <summary>
	/// マップ読み込み
	/// </summary>
	void StageLoad(const std::string& groupName);

public: // メンバ定数

private: //メンバ変数

	// ステージ番号
	int stageNum_;

	// ステージデータ
	StageData initialData_;

	// マップ
	int** map_;

	// プレイヤーの位置
	Vector2 playerPosition_;

	// エネミーの位置
	std::vector<Vector2> enemyPosition_;

	// ゴールが開いたか
	bool goalOpened_;
	
	// 敵を捕まえた
	std::vector<bool> capturedEnemy_;

	// ステージデータ(ステージ番号、データ)
	using Group = std::map<std::string, StageData>;
	std::map<std::string, Group> stageDatas_;

	// ステージデータの保存先ファイルパス
	const std::string kDirectoryPath = "Resources/StageData";

};

