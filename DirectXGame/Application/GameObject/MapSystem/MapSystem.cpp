#include "MapSystem.h"
#include <cassert>
#include <json.hpp>
#include <fstream>
#include <WinUser.h>

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
	map_ = new int*[static_cast<size_t>(kMapSize_.y)];
	for (size_t y = 0; y < static_cast<size_t>(kMapSize_.y); y++) {
		map_[y] = new int[static_cast<size_t>(kMapSize_.x)];
	}

	// 初期マップ
	initialData_.map_ = new int* [static_cast<size_t>(kMapSize_.y)];
	for (size_t y = 0; y < static_cast<size_t>(kMapSize_.y); y++) {
		initialData_.map_[y] = new int[static_cast<size_t>(kMapSize_.x)];
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
			map_[y][x] = initialData_.map_[y][x];
		}
	}

	// プレイヤーの位置
	playerPosition_ = initialData_.playerPosition_;

	// エネミーの位置
	for (size_t i = 0; initialData_.enemyPosition_.size(); i++) {
		enemyPosition_.push_back(initialData_.enemyPosition_.at(i));
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

	// 初期情報
	for (std::map<std::string, Group>::iterator itGroup = stageDatas_.begin(); itGroup != stageDatas_.end();
		++itGroup) {
		// グループの参照を取得
		Group& group = itGroup->second;
		// ステージ番号確認
		for (std::map<std::string, StageData>::iterator itData = group.begin();
			itData != group.end(); ++itData) {
			// 項目名を取得
			const std::string& itemName = itData->first;
			if (itemName == std::to_string(stageNum_)) {
				// 項目を取得
				initialData_ = itData->second;
				break;
			}
		}
	}

	// マップ
	for (size_t y = 0; y < static_cast<size_t>(kMapSize_.y); y++) {
		for (size_t x = 0; x < static_cast<size_t>(kMapSize_.x); x++) {
			map_[y][x] = initialData_.map_[y][x];
		}
	}
	// プレイヤーの位置
	playerPosition_ = initialData_.playerPosition_;
	// エネミーの位置
	for (size_t i = 0; initialData_.enemyPosition_.size(); i++) {
		enemyPosition_.push_back(initialData_.enemyPosition_.at(i));
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
	/*

	// 読み込むJSONファイルのフルパスを合成する
	std::string filePath = kDirectoryPath + groupName + ".json";
	// 読み込み用ファイルストリーム
	std::ifstream ifs;
	// ファイルを読み込み用に開く
	ifs.open(filePath);
	// ファイルオープン失敗
	if (!std::filesystem::exists(filePath)) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
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
		
		StageData stageData;
		uint32_t i = 0;

		// マップ
		for (size_t y = 0; y < static_cast<size_t>(kMapSize_.y); y++) {
			for (size_t x = 0; x < static_cast<size_t>(kMapSize_.x); x++) {
				stageData.map_[y][x] = itItem->at(i);
				i++;
			}
		}
		// プレイヤーの位置
		stageData.playerPosition_.x = itItem->at(i);
		i++;
		stageData.playerPosition_.y = itItem->at(i);
		i++;
		// エネミーの位置
		std::vector<Vector2> enemyPosition_;
		for (size_t k = 0; initialData_.enemyPosition_.size(); k++) {
			stageData.enemyPosition_.at(k).x = itItem->at(i);
			i++;
			stageData.enemyPosition_.at(k).y = itItem->at(i);
			i++;
		}

		// 檻の位置
		std::vector<Vector2> cagePosition_;
		// スタートの位置
		Vector2 startPosition_;
		// ゴールの位置
		Vector2 goalPosition_;

	}

	*/

}
