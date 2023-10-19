#include "MapSystem.h"
#include <cassert>
#include <json.hpp>
#include <fstream>

#include "Application/GameObject/EnemiesManager/EnemiesManager.h"
#include "Application/GameObject/BlockManager/BlockManager.h"
#include "Application/GameObject/Player/Player.h"
#include "Application/GameObject/Start/Start.h"
#include "Application/GameObject/Goal/Goal.h"

#include "Application/UI/CaptureEnemyUI/CaptureEnemyUI.h"

//名前空間
using namespace nlohmann;

// 静的メンバ定数
// マップのサイズ
const Vector2 MapSystem::kMapSize_ = { 15.0f, 15.0f };
// マスのサイズ
const Vector2 MapSystem::kSquareSize_ = { 10.0f, 10.0f };
// ステージ数
uint32_t MapSystem::kMaximumNumberOfStages_ = 0;

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
	else if (comandNumber_ == Command::CommandNumber::Restart) {
		Restart();
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

	initialStageData_.enemyPosition_.clear();
	initialStageData_.cagePosition_.clear();

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
	// エネミーのカウント
	enemyCount_ = 0;
	enemyPosition_.clear();
	nextEnemyPosition_.clear();
	for (size_t i = 0; i < initialStageData_.enemyPosition_.size(); i++) {
		Vector2 enemyPosition = initialStageData_.enemyPosition_.at(i);
		enemyPosition_.push_back(enemyPosition);
		nextEnemyPosition_.push_back(enemyPosition);
		enemyCount_++;
	}

	//檻のカウント
	cageCount_ = initialStageData_.cagePosition_.size();

	// ゴールが開いたか
	goalOpened_ = false;
	particleManager_->SetOpen(false);
	// 敵を捕まえた
	// エネミーが起きている時
	// 檻を使った
	enemyAwake_.clear();
	capturedEnemy_.clear();
	usedCage_.clear();
	for (size_t i = 0; i < enemyCount_; i++) {
		enemyAwake_.push_back(false);
		capturedEnemy_.push_back(false);
	}
	for (size_t i = 0; i < cageCount_; i++) {
		usedCage_.push_back(false);
	}

	// コマンド番号
	comandNumber_ = Command::CommandNumber::None;

	// ゲームクリアフラグ
	isGameClaer_ = false;

	// ゲームオーバーフラグ
	isGameOver_ = false;

	// リスタートフラグ
	isRestart_ = false;

}

void MapSystem::StagesLoad()
{

	StageDatasDelete();

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

		StageLoad(filePath.stem().string());

	}

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
		// 移動成功アニメーション
		player_->ActionAnimationInitialize(commandNumber);
	}
	// 失敗
	else {
		// 移動失敗アニメーション
		player_->ActionAnimationInitialize(Command::CommandNumber::None);

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

	// 移動
	for (size_t i = 0; i < enemyCount_; i++) {
		enemyPosition_.at(i) = nextEnemyPosition_.at(i);
		// 檻に入る
		int k = 0;
		for (Vector2 cagePosition : initialStageData_.cagePosition_) {
			if (enemyPosition_.at(i).x == cagePosition.x &&
				enemyPosition_.at(i).y == cagePosition.y &&
				!usedCage_.at(k)) {
				capturedEnemy_.at(i) = true;
				usedCage_.at(k) = true;


				int num = 0;
				// 捕まえた敵の数取得、UIにおくる
				for (bool capturedEnemy : capturedEnemy_) {
					if (capturedEnemy) {
						num++;
					}
				}
				captureEnemyUI_->EnemyCountUpdate(num);

				break;
			}
			k++;
		}
	}

	EnemyMovePlan();

}

void MapSystem::EnemyMovePlan()
{

	// エネミーが起きている時
	for (size_t i = 0; i < enemyCount_; i++) {
		if (enemyAwake_.at(i) && !capturedEnemy_.at(i)) {
			// 行動する
			int distanceX = static_cast<int>(std::fabsf(playerPosition_.x - enemyPosition_.at(i).x));
			int distanceY = static_cast<int>(std::fabsf(playerPosition_.y - enemyPosition_.at(i).y));

			// 出来ればyに動く
			if (distanceX < distanceY) {
				if (playerPosition_.y < enemyPosition_.at(i).y) {
					nextEnemyPosition_.at(i).y = enemyPosition_.at(i).y - 1.0f;
				}
				else {
					nextEnemyPosition_.at(i).y = enemyPosition_.at(i).y + 1.0f;
				}
			}
			else {
				if (playerPosition_.x < enemyPosition_.at(i).x) {
					nextEnemyPosition_.at(i).x = enemyPosition_.at(i).x - 1.0f;
				}
				else {
					nextEnemyPosition_.at(i).x = enemyPosition_.at(i).x + 1.0f;
				}
			}

		}
	}

	// 重なり判定
	for (size_t i = 0; i < enemyCount_ - 1; i++) {
		for (size_t j = i + 1; j < enemyCount_; j++) {
			// 重なっている
			if (nextEnemyPosition_.at(i).x == nextEnemyPosition_.at(j).x &&
				nextEnemyPosition_.at(i).y == nextEnemyPosition_.at(j).y) {
				if (capturedEnemy_.at(j) || !enemyAwake_.at(j)) {
					// iを動かす
					nextEnemyPosition_.at(i) = enemyPosition_.at(i);
				}
				else {
					// jを動かす
					nextEnemyPosition_.at(j) = enemyPosition_.at(j);
				}
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

	EnemyMovePlan();

}

void MapSystem::GameClear()
{
	if (!goalOpened_) {
		// ゴールエフェクトの準備
		goalOpened_ = true;
		for (bool capturedEnemy : capturedEnemy_) {
			if (!capturedEnemy) {
				goalOpened_ = false;
				break;
			}
		}
	}
	if(goalOpened_)
	{
		particleManager_->GoalEffectSetting(Vector3(MapSystem::kSquareSize_.x * initialStageData_.goalPosition_.x,
			MapSystem::kSquareSize_.y * initialStageData_.goalPosition_.y, -10.0f));

		//particleManager_->CatchEnemyGenerate(Vector3(MapSystem::kSquareSize_.x * initialStageData_.goalPosition_.x,
		//	MapSystem::kSquareSize_.y * initialStageData_.goalPosition_.y, -10.0f));
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
	enemyPosition_.clear();
	nextEnemyPosition_.clear();
	for (size_t i = 0; i < initialStageData_.enemyPosition_.size(); i++) {
		enemyPosition_.push_back(initialStageData_.enemyPosition_.at(i));
		nextEnemyPosition_.push_back(initialStageData_.enemyPosition_.at(i));
	}

	// ゴールが開いたか
	goalOpened_ = false;

	// 敵を捕まえた
	capturedEnemy_.clear();
	enemyAwake_.clear();
	usedCage_.clear();
	for (size_t i = 0; i < enemyCount_; i++) {
		enemyAwake_.push_back(false);
		capturedEnemy_.push_back(false);
	}
	for (size_t i = 0; i < cageCount_; i++){
		usedCage_.push_back(false);
	}

	// コマンド番号
	comandNumber_ = Command::CommandNumber::None;

	isGameClaer_ = false;

	isGameOver_ = false;

	isRestart_ = true;

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

	// マップ数
	kMaximumNumberOfStages_ = 0;

	// ファイル読み込み
	while (1) {

		// グループを検索
		std::string name = groupName + std::to_string(kMaximumNumberOfStages_);

		json::iterator itGroup = root.find(name);

		// 未登録チェック
		if (itGroup == root.end()) {
			break;
		}
		else {
			kMaximumNumberOfStages_++;
		}

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

}

void MapSystem::StageDatasDelete()
{

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

	stageDatas_.clear();

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
	if (group.find(key) != group.end()) {
		// 指定グループから指定のキーの値を取得
		return std::get<2>(group[key]);
	}
	std::vector<Vector2> result;
	return result;

}
