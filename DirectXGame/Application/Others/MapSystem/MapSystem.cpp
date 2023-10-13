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
const Vector2 MapSystem::kSquareSize_ = { 10.0f,10.0f };
// ステージ数
const uint32_t kMaximumNumberOfStages_ = 1;

MapSystem::~MapSystem()
{
}

void MapSystem::Initialize(int stageNum)
{

	// マップ
	map_ = new int* [static_cast<size_t>(kMapSize_.y)];
	for (size_t y = 0; y < static_cast<size_t>(kMapSize_.y); y++) {
		map_[y] = new int[static_cast<size_t>(kMapSize_.x)];
	}

	// 初期マップ
	initialStageData_.map_ = new int* [static_cast<size_t>(kMapSize_.y)];
	for (size_t y = 0; y < static_cast<size_t>(kMapSize_.y); y++) {
		initialStageData_.map_[y] = new int[static_cast<size_t>(kMapSize_.x)];
	}

	// マップデータ
	StagesLoad();

	//設定
	Setting(stageNum);

}

void MapSystem::Update()
{
}

void MapSystem::Move()
{
}

void MapSystem::BlockFall()
{
}

void MapSystem::PlayerMove()
{
}

void MapSystem::EnemyMove()
{
}

void MapSystem::MakeSound()
{
}

void MapSystem::GameClear()
{
}

void MapSystem::GameOver()
{
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

}

void MapSystem::Setting(int stageNum)
{

	// ステージ番号
	stageNum_ = stageNum;

	// グループ名の設定
	std::string groupName = std::to_string(stageNum_);

	// 初期情報
	initialStageData_.map_ = GetMapValue(groupName, "map_");
	initialStageData_.playerPosition_ = GetPositionValue(groupName, "playerPosition_");
	initialStageData_.enemyPosition_ = GetPositionsValue(groupName, "enemyPosition_");
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
	for (size_t i = 0; initialStageData_.enemyPosition_.size(); i++) {
		enemyPosition_.push_back(initialStageData_.enemyPosition_.at(i));
	}
	// ゴールが開いたか
	goalOpened_ = false;
	// 敵を捕まえた
	for (size_t i = 0; enemyPosition_.size(); i++) {
		bool capturedEnemy = false;
		capturedEnemy_.push_back(capturedEnemy);
	}

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
		StageLoad(filePath.stem().string());
	}

}

void MapSystem::StageLoad(const std::string& groupName)
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
	json::iterator itGroup = root.find(groupName);

	// 未登録チェック
	assert(itGroup != root.end());

	// 各アイテムについて
	for (json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem) {
		// アイテム名を取得
		const std::string& itemName = itItem.key();

		// int32_t型
		if (itItem->is_array() && itItem->size() == static_cast<size_t>(kMapSize_.x) * static_cast<size_t>(kMapSize_.y)) {
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
			SetValue(groupName, itemName, values);
		}
		// 要素数が2の配列であれば
		else if (itItem->is_array() && itItem->size() == 2) {
			// float型のjson配列登録
			Vector2 value = { itItem->at(0), itItem->at(1) };
			SetValue(groupName, itemName, value);
		}
		// 要素数が多い配列であれば
		else if (itItem->is_array() && itItem->size() > 2) {
			// float型のjson配列登録
			std::vector<Vector2> values;
			size_t i = 0;
			for (size_t k = 0; k < itItem->size(); k++) {
				Vector2 value = { itItem->at(i), itItem->at(i + 1) };
				values.push_back(value);
				i += 2;
			}
			SetValue(groupName, itemName, values);
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
