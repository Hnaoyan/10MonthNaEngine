#pragma once
#include "StructManager.h"
#include <vector>
#include <map>
#include <string>
#include <variant>

#include "../Application/Others/Command/Command.h"

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
	void Update(Command::CommandNumber commandNumber);

private: // メンバ関数

	/// <summary>
	/// 移動
	/// </summary>
	void Move(Command::CommandNumber commandNumber);

	/// <summary>
	/// ブロックが落ちる
	/// </summary>
	void BlockFall(int32_t x, int32_t y);

	/// <summary>
	/// プレイヤーの移動
	/// </summary>
	/// <param name="x">x座標</param>
	/// <param name="y">y座標</param>
	/// <returns>移動できたか</returns>
	bool PlayerMove(int32_t x, int32_t y);

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
	void StageLoad(const std::string& groupName, size_t num);

	/// <summary>
	/// 値のセット
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <param name="value"></param>
	void SetValue(const std::string& groupName, const std::string& key, int** value);
	void SetValue(const std::string& groupName, const std::string& key, const Vector2& value);
	void SetValue(const std::string& groupName, const std::string& key, std::vector<Vector2>& value);

	/// <summary>
	/// 値の取得
	/// </summary>
	/// <returns></returns>
	int** GetMapValue(const std::string& groupName, const std::string& key);
	Vector2 GetPositionValue(const std::string& groupName, const std::string& key);
	std::vector<Vector2> GetPositionsValue(const std::string& groupName, const std::string& key);

public: // メンバ定数

private: //メンバ変数

	// ステージ番号
	int stageNum_;

	// ステージデータ
	StageData initialStageData_;

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

	// コマンド番号
	Command::CommandNumber comandNumber_;

	// ゲームクリアフラグ
	bool isGameClaer_;

	// ゲームオーバーフラグ
	bool isGameOver_;

private: //メンバ変数(データ)

	// ステージデータ(ステージ番号、データ)
	using Item = std::variant<int**, Vector2, std::vector<Vector2>, float*>;
	using Group = std::map<std::string, Item>;
	std::map<std::string, Group> stageDatas_;

	// ステージデータの保存先ファイルパス
	const std::string kDirectoryPath = "Resources/StageData/";

};

