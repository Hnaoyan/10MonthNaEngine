#include "MapSystem.h"
#include <cassert>
#include <json.hpp>
#include <fstream>

//名前空間
using namespace nlohmann;

// 静的メンバ定数
// マップのサイズ
const Vector2 MapSystem::kMapSize_ = { 15.0f, 15.0f };
// マスのサイズ
const Vector2 MapSystem::kSquareSize_ = { 10.0f, 10.0f };
// ステージ数
const uint32_t kMaximumNumberOfStages_ = 2;

MapSystem::~MapSystem()
{
	// マップ
	for (size_t y = 0; y < static_cast<size_t>(kMapSize_.y); y++) {
		delete map_[y];
	}
	delete map_;

	// データマップ


	for (std::map<std::string, Group>::iterator itGroup = stageDatas_.begin(); itGroup != stageDatas_.end();
		++itGroup) {
		
		std::string string = itGroup->first;

		// 指定グループが存在するか
		assert(stageDatas_.find(string) != stageDatas_.end());
		//  グループの参照を取得
		Group& group = stageDatas_[string];
		// 指定グループに指定キーが存在するか
		assert(group.find("map_") != group.end());
		// 指定グループから指定のキーの値を取得
		int** map = std::get<0>(group["map_"]);
		for (size_t y = 0; y < static_cast<size_t>(kMapSize_.y); y++) {
			if (map[y] != nullptr) {
				delete map[y];
			}
		}
		if (map) {
			delete map;
		}

	}

}

void MapSystem::Initialize(int stageNum)
{

	// マップ
	map_ = new int* [static_cast<size_t>(kMapSize_.y)];
	for (size_t y = 0; y < static_cast<size_t>(kMapSize_.y); y++) {
		map_[y] = new int[static_cast<size_t>(kMapSize_.x)];
	}

	// マップデータ
	StagesLoad();

	//設定
	Setting(stageNum);

}

void MapSystem::Update(Command::CommandNumber commandNumber)
{

	// コマンド番号
	comandNumber_ = commandNumber;

	// 移動
	Move(comandNumber_);

	// 音を鳴らす
	if (comandNumber_ == Command::CommandNumber::Button) {
		MakeSound();
	}
	
	// ゲームクリアチェック
	GameClear();
	// ゲームオーバーチェック
	GameOver();

}

void MapSystem::Setting(int stageNum)
{

	// ステージ番号
	stageNum_ = stageNum;

	// グループ名の設定
	std::string groupName = "StageData" + std::to_string(stageNum_);

	// 初期情報
	initialStageData_.map_ = GetMapValue(groupName, "map_");
	initialStageData_.playerPosition_ = GetPositionValue(groupName, "playerPosition_");
	initialStageData_.enemyPosition_ = GetPositionsValue(groupName, "enemyPosition_");
	//for(std::vector<Vector2>::iterator ; )
	initialStageData_.cagePosition_ = GetPositionsValue(groupName, "cagePosition_");
	initialStageData_.startPosition_ = GetPositionValue(groupName, "startPosition_");
	initialStageData_.goalPosition_ = GetPositionValue(groupName, "goalPosition_");

	// マップ
	for (size_t y = 0; y < static_cast<size_t>(kMapSize_.y); y++) {
		for (size_t x = 0; x < static_cast<size_t>(kMapSize_.x); x++) {
			map_[y][x] = initialStageData_.map_[y][x];
		}
	}
	// プレイヤーの位置
	playerPosition_ = initialStageData_.playerPosition_;
	// エネミーの位置
	// エネミーのカウント
	enemyCount_ = 0;
	for (size_t i = 0; i < initialStageData_.enemyPosition_.size(); i++) {
		Vector2 enemyPosition = initialStageData_.enemyPosition_.at(i);
		enemyPosition_.push_back(enemyPosition);
		enemyCount_++;
	}

	// ゴールが開いたか
	goalOpened_ = false;
	// 敵を捕まえた
	// エネミーが起きている時
	for (size_t i = 0; i < enemyCount_; i++) {
		enemyAwake_.push_back(false);
		capturedEnemy_.push_back(false);
	}

	// コマンド番号
	comandNumber_ = Command::CommandNumber::None;

	// ゲームクリアフラグ
	isGameClaer_ = false;

	// ゲームオーバーフラグ
	isGameOver_ = false;

}

void MapSystem::Move(Command::CommandNumber commandNumber)
{

	// 変数
	int32_t playerPosX = static_cast<int32_t>(playerPosition_.x);
	int32_t playerPosY = static_cast<int32_t>(playerPosition_.y);

	bool haveMoved = false;

	// コマンドによる移動
	switch (commandNumber)
	{
	case Command::Left:
		haveMoved = PlayerMove(playerPosX - 1, playerPosY);
		break;
	case Command::Right:
		haveMoved = PlayerMove(playerPosX + 1, playerPosY);
		break;
	case Command::Up:
		haveMoved = PlayerMove(playerPosX, playerPosY + 1);
		break;
	case Command::Down:
		haveMoved = PlayerMove(playerPosX, playerPosY - 1);
		break;
	default:
		break;
	}

	// 移動できるかチェック
	// 成功
	if (haveMoved) {
		// エネミーの移動
		EnemyMove();
	}
	// 失敗
	else {

	}

}

void MapSystem::BlockFall(int32_t x, int32_t y)
{
	// 道から穴に
	map_[y][x] = Hole;

}

bool MapSystem::PlayerMove(int32_t x, int32_t y)
{

	// 変数
	int32_t playerPosX = static_cast<int32_t>(playerPosition_.x);
	int32_t playerPosY = static_cast<int32_t>(playerPosition_.y);

	// マップはみ出し確認
	if (x < 0 || y < 0 ||
		x >= static_cast<int32_t>(kMapSize_.x) ||
		y >= static_cast<int32_t>(kMapSize_.y)) {
		// 移動できない
		return false;
	}
	
	switch (static_cast<MapNumber>(map_[y][x]))
	{
	// 壁
	case Wall:
		// 移動できない
		return false;
		break;
	
	// 道
	case Road:
		//現在位置のブロックを落とす
		BlockFall(playerPosX, playerPosY);
		// 移動
		playerPosition_.x = static_cast<float>(x);
		playerPosition_.y = static_cast<float>(y);
		// 移動できた
		return true;
		break;
	
	// 穴
	case Hole:
		// 移動できない
		return false;
		break;
	
	default:
		break;
	}

	return false;

}

void MapSystem::EnemyMove()
{

	// エネミーが起きている時
	for (size_t i = 0; i < enemyCount_; i++) {
		if (enemyAwake_.at(i) && !capturedEnemy_.at(i)) {
			// 行動する
			int x = static_cast<int>(std::fabsf(playerPosition_.x - enemyPosition_.at(i).x));
			int y = static_cast<int>(std::fabsf(playerPosition_.y - enemyPosition_.at(i).y));

			// 出来ればyに動く
			if (x < y) {
				if (playerPosition_.y < enemyPosition_.at(i).y) {
					enemyPosition_.at(i).y -= 1.0f;
				}
				else {
					enemyPosition_.at(i).y += 1.0f;
				}
			}
			else {
				if (playerPosition_.x < enemyPosition_.at(i).x) {
					enemyPosition_.at(i).x -= 1.0f;
				}
				else {
					enemyPosition_.at(i).x += 1.0f;
				}
			}

			// 檻に入る
			if (enemyPosition_.at(i).x == initialStageData_.cagePosition_.at(i).x &&
				enemyPosition_.at(i).y == initialStageData_.cagePosition_.at(i).y) {
				capturedEnemy_.at(i) = true;
			}

		}
	}

}

void MapSystem::MakeSound()
{

	for (size_t i = 0; i < enemyCount_; i++) {
		if (!enemyAwake_.at(i) && !capturedEnemy_.at(i)) {
			// 起きる
			int x = static_cast<int>(std::fabsf(playerPosition_.x - enemyPosition_.at(i).x));
			int y = static_cast<int>(std::fabsf(playerPosition_.y - enemyPosition_.at(i).y));
			
			if (x + y < 3) {
				enemyAwake_.at(i) = true;
			}

		}
	}

}

void MapSystem::GameClear()
{
	if (!goalOpened_) {
		goalOpened_ = true;
		for (bool capturedEnemy : capturedEnemy_) {
			if (!capturedEnemy) {
				goalOpened_ = false;
				break;
			}
		}
	}

	if (playerPosition_.x == initialStageData_.goalPosition_.x &&
		playerPosition_.y == initialStageData_.goalPosition_.y &&
		goalOpened_) {
		isGameClaer_ = true;
	}

}

void MapSystem::GameOver()
{

	for (size_t i = 0; i < enemyCount_; i++) {
		// ぶつかった
		if (playerPosition_.x == enemyPosition_.at(i).x && 
			playerPosition_.y == enemyPosition_.at(i).y && 
			enemyAwake_.at(i)) {
			isGameOver_ = true;
			return;
		}
		// 寝ている敵に近づく
		int x = static_cast<int>(std::fabsf(playerPosition_.x - enemyPosition_.at(i).x));
		int y = static_cast<int>(std::fabsf(playerPosition_.y - enemyPosition_.at(i).y));
		if (x + y < 2 && !enemyAwake_.at(i)) {
			isGameOver_ = true;
			return;
		}
	}

}

void MapSystem::Restart()
{

	// マップ
	for (size_t y = 0; y < static_cast<size_t>(kMapSize_.y); y++) {
		for (size_t x = 0; x < static_cast<size_t>(kMapSize_.x); x++) {
			map_[y][x] = initialStageData_.map_[y][x];
		}
	}

	// プレイヤーの位置
	playerPosition_ = initialStageData_.playerPosition_;

	// エネミーの位置
	for (size_t i = 0; initialStageData_.enemyPosition_.size(); i++) {
		enemyPosition_.push_back(initialStageData_.enemyPosition_.at(i));
	}

	// ゴールが開いたか
	goalOpened_ = false;

	// 敵を捕まえた
	for (size_t i = 0; capturedEnemy_.size(); i++) {
		capturedEnemy_.at(i) = false;
	}

	// コマンド番号
	comandNumber_ = Command::CommandNumber::None;

}

void MapSystem::StagesLoad()
{

	std::string saveDirectryPath = kDirectoryPath;
	// ディレクトリがなければスキップする
	if (!std::filesystem::exists(saveDirectryPath)) {
		return;
	}
	std::filesystem::directory_iterator dir_it(saveDirectryPath);
	for (const std::filesystem::directory_entry& entry : dir_it) {
		// ファイルパスを取得
		const std::filesystem::path& filePath = entry.path();

		// ファイル拡張子を取得
		std::string extension = filePath.extension().string();
		// .jsonファイル以外はスキップ
		if (extension.compare(".json") != 0) {
			continue;
		}

		// ファイル読み込み
		for (size_t i = 0; i < static_cast<size_t>(2); i++) {
			StageLoad(filePath.stem().string(), i);
		}
	}

}

void MapSystem::StageLoad(const std::string& groupName , size_t num)
{

	// 読み込むJSONファイルのフルパスを合成する
	std::string filePath = kDirectoryPath + groupName + ".json";
	// 読み込み用ファイルストリーム
	std::ifstream ifs;
	// ファイルを読み込み用に開く
	ifs.open(filePath);
	// ファイルオープン失敗
	if (!std::filesystem::exists(filePath)) {
		assert(0);
	}
	json root;

	// json文字列からjsonのデータ構造に展開
	ifs >> root;
	// ファイルを閉じる
	ifs.close();

	// グループを検索
	std::string name = groupName + std::to_string(num);

	json::iterator itGroup = root.find(name);

	// 未登録チェック
	assert(itGroup != root.end());

	// 各アイテムについて
	for (json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem) {
		// アイテム名を取得
		const std::string& itemName = itItem.key();

		// int32_t型
		if (itemName == "map_") {
			// int型の値を登録
			int** values;
			values = new int* [static_cast<size_t>(kMapSize_.y)];
			for (size_t y = 0; y < static_cast<size_t>(kMapSize_.y); y++) {
				values[y] = new int[static_cast<size_t>(kMapSize_.x)];
			}
			size_t i = 0;
			// マップ
			for (size_t y = 0; y < static_cast<size_t>(kMapSize_.y); y++) {
				for (size_t x = 0; x < static_cast<size_t>(kMapSize_.x); x++) {
					values[y][x] = itItem->at(i);
					i++;
				}
			}
			SetValue(name, itemName, values);
		}
		// 要素数が2の配列であれば
		else if (itemName == "playerPosition_" || 
				itemName == "startPosition_" || 
			itemName == "goalPosition_") {
			// float型のjson配列登録
			Vector2 value = { itItem->at(0), itItem->at(1) };
			SetValue(name, itemName, value);
		}
		// 要素数が多い配列であれば
		else {
			// float型のjson配列登録
			std::vector<Vector2> values;
			for (size_t k = 0; k < itItem->size(); k += 2) {
				Vector2 value = { itItem->at(k), itItem->at(k + 1) };
				values.push_back(value);
			}
			SetValue(name, itemName, values);
		}

	}

}

void MapSystem::SetValue(const std::string& groupName, const std::string& key, int** value)
{

	// グループの参照を取得
	Group& group = stageDatas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem = value;
	// 設定した項目をstd::mapに追加
	group[key] = newItem;

}

void MapSystem::SetValue(const std::string& groupName, const std::string& key, const Vector2& value) {

	// グループの参照を取得
	Group& group = stageDatas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem = value;
	// 設定した項目をstd::mapに追加
	group[key] = newItem;

}
void MapSystem::SetValue(const std::string& groupName, const std::string& key, std::vector<Vector2>& value) {

	// グループの参照を取得
	Group& group = stageDatas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem = value;
	// 設定した項目をstd::mapに追加
	group[key] = newItem;

}

int** MapSystem::GetMapValue(const std::string& groupName, const std::string& key)
{
	// 指定グループが存在するか
	assert(stageDatas_.find(groupName) != stageDatas_.end());
	//  グループの参照を取得
	Group& group = stageDatas_[groupName];
	// 指定グループに指定キーが存在するか
	assert(group.find(key) != group.end());
	// 指定グループから指定のキーの値を取得
	return std::get<0>(group[key]);
}

Vector2 MapSystem::GetPositionValue(const std::string& groupName, const std::string& key)
{
	// 指定グループが存在するか
	assert(stageDatas_.find(groupName) != stageDatas_.end());
	//  グループの参照を取得
	Group& group = stageDatas_[groupName];
	// 指定グループに指定キーが存在するか
	assert(group.find(key) != group.end());
	// 指定グループから指定のキーの値を取得
	return std::get<1>(group[key]);
}

std::vector<Vector2> MapSystem::GetPositionsValue(const std::string& groupName, const std::string& key)
{
	// 指定グループが存在するか
	assert(stageDatas_.find(groupName) != stageDatas_.end());
	//  グループの参照を取得
	Group& group = stageDatas_[groupName];
	// 指定グループに指定キーが存在するか
	assert(group.find(key) != group.end());
	// 指定グループから指定のキーの値を取得
	return std::get<2>(group[key]);

}