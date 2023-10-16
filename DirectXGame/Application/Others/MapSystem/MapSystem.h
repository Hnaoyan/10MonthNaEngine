#pragma once
#include "StructManager.h"
#include <vector>
#include <map>
#include <string>
#include <variant>

#include "../Application/Others/Command/Command.h"
#include "Application/GameObject/EnemiesManager/EnemiesManager.h"

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
	static uint32_t kMaximumNumberOfStages_;

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

	/// <summary>
	/// 設定
	/// </summary>
	void Setting(int stageNum);

	/// <summary>
	/// マップ読み込み
	/// </summary>
	void StagesLoad();

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
	/// エネミーの移動計画
	/// </summary>
	void EnemyMovePlan();

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
	/// マップ読み込み
	/// </summary>
	void StageLoad(const std::string& groupName);

	/// <summary>
	/// ステージデータ削除
	/// </summary>
	void StageDatasDelete();

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

public: // アクセッサ

	// マップ
	int** GetMap() { return map_; }

	// プレイヤー
	Vector2 GetPlayerPosition() { return playerPosition_; }

	// エネミーズ
	std::vector<Vector2> GetEnemyPosition() { return enemyPosition_; }

	// 初期情報

	// マップ
	int** GetInitialMap() { return initialStageData_.map_; }

	// プレイヤー
	Vector2 GetInitialPlayerPosition() { return initialStageData_.playerPosition_; }

	// エネミーズ
	std::vector<Vector2> GetInitialEnemyPosition() { return initialStageData_.enemyPosition_; }

	// 檻
	std::vector<Vector2> GetInitialCagePosition() { return initialStageData_.cagePosition_; }

	// スタート位置
	Vector2 GetInitialStartPosition() { return initialStageData_.startPosition_; }

	// ゴール位置
	Vector2 GetInitialGoalPosition() { return initialStageData_.goalPosition_; }

	size_t GetEnemyCount() { return enemyCount_; }
	size_t GetCageCount() { return cageCount_; }

	void SetEnemiesManager(EnemiesManager* enemiesManager) { enemiesManager_ = enemiesManager; }

	bool GetIsGameClear() { return isGameClaer_; }

	bool GetIsGameOver() { return isGameOver_; }

	bool GetIsRestart() { return isRestart_; }

	void SetIsRestart(bool isRestart) { isRestart_ = isRestart; }

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
	// エネミーの次の位置
	std::vector<Vector2> nextEnemyPosition_;

	// エネミーのカウント
	size_t enemyCount_;
	// 檻のカウント
	size_t cageCount_;

	// エネミーが起きている時
	std::vector<bool> enemyAwake_;

	// ゴールが開いたか
	bool goalOpened_;

	// 敵を捕まえた
	std::vector<bool> capturedEnemy_;

	// 檻を使った
	std::vector<bool> usedCage_;

	// コマンド番号
	Command::CommandNumber comandNumber_;

	// ゲームクリアフラグ
	bool isGameClaer_;

	// ゲームオーバーフラグ
	bool isGameOver_;

	// エネミーズマネージャー
	EnemiesManager* enemiesManager_;

	// リスタートフラグ
	bool isRestart_;

private: //メンバ変数(データ)

	// ステージデータ(ステージ番号、データ)
	using Item = std::variant<int**, Vector2, std::vector<Vector2>, float*>;
	using Group = std::map<std::string, Item>;
	std::map<std::string, Group> stageDatas_;

	// ステージデータの保存先ファイルパス
	const std::string kDirectoryPath = "Resources/StageData/";

};

