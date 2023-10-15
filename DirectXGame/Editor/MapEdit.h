#pragma once
#include <variant>
#include <map>
#include <string>
#include <StructManager.h>
#include <vector>
#include <list>
#include <WorldTransform.h>
#include <ViewProjection.h>
#include <Model.h>
#include <Input.h>

/// <summary>
/// マップエディタクラス
/// </summary>
class MapEdit
{

public: // サブクラス
	// マップ番号
	enum MapNumber {
		Wall, // 壁 (使わない)
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
		std::list<Vector2> enemyPosition_;
		// 檻の位置
		std::list<Vector2> cagePosition_;
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

public: // 静的メンバ変数

	// ステージ数
	static uint32_t kMaximumNumberOfStages_;

public: // メンバ関数(edit部分)

	~MapEdit();

	void Initialize(Model* enemymodel, Model* cagemodel, Model* startmodel, Model* goalmodel, Model* blockmodel);

	void Update(const ViewProjection& viewProjection);

	void Draw(const ViewProjection& viewProjection);

	void Setting(size_t stageNum);

private: // メンバ関数

	Vector2 BlockFind(const ViewProjection& viewProjection);

	void DuplicateConfirmation(Vector2 pos);

	void StageDatasDelete();

	void AddStage(std::string groupName);


private: // メンバ変数

	// ステージ番号
	size_t stageNum_;

	// ステージデータ
	StageData stageData_;

	// 敵の数
	size_t enemyNum_;
	// 檻の数
	size_t cageNum_;

	Input* input_ = nullptr;

private: // モデルとか

	// エネミーの位置
	std::list<WorldTransform> enemyWT_;
	// 檻の位置
	std::list<WorldTransform> cageWT_;
	// スタート位置
	WorldTransform startWT_;
	// ゴール位置
	WorldTransform goalWT_;
	// ブロック
	std::vector<WorldTransform> blockWT_;

	// モデル
	Model* enemyModel_ = nullptr;
	Model* cageModel_ = nullptr;
	Model* startModel_ = nullptr;
	Model* goalModel_ = nullptr;
	Model* blockModel_ = nullptr;

public: // メンバ関数(読み込みなど)

	/// <summary>
	/// マップ読み込み
	/// </summary>
	void StagesLoad();

	/// <summary>
	/// マップ読み込み
	/// </summary>
	void StageLoad(const std::string& groupName);

	/// <summary>
	/// 値のセット
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <param name="value"></param>
	void SetValue(const std::string& groupName, const std::string& key, int** value);
	void SetValue(const std::string& groupName, const std::string& key, const Vector2& value);
	void SetValue(const std::string& groupName, const std::string& key, std::list<Vector2>& value);

	/// <summary>
	/// 値の取得
	/// </summary>
	/// <returns></returns>
	int** GetMapValue(const std::string& groupName, const std::string& key);
	Vector2 GetPositionValue(const std::string& groupName, const std::string& key);
	std::list<Vector2> GetPositionsValue(const std::string& groupName, const std::string& key);

private: //メンバ変数(データ)

	// ステージデータ(ステージ番号、データ)
	using Item = std::variant<int**, Vector2, std::list<Vector2>, float*>;
	using Group = std::map<std::string, Item>;
	std::map<std::string, Group> stageDatas_;

	// ステージデータの保存先ファイルパス
	const std::string kDirectoryPath = "Resources/StageData/";

};

