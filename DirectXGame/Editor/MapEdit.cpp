#include "MapEdit.h"
#include <cassert>
#include <json.hpp>
#include <fstream>
#include <WinApp.h>
#include <MatLib.h>
#include <imgui.h>

//名前空間
using namespace nlohmann;

// 静的メンバ定数
// マップのサイズ
const Vector2 MapEdit::kMapSize_ = { 15.0f, 15.0f };
// マスのサイズ
const Vector2 MapEdit::kSquareSize_ = { 10.0f, 10.0f };
// ステージ数
uint32_t MapEdit::kMaximumNumberOfStages_ = 0;

MapEdit::~MapEdit()
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
}

void MapEdit::Initialize(Model* enemymodel,	Model* cagemodel, Model* startmodel, Model* goalmodel,	Model* blockmodel)
{

	stageNum_ = 0;

	// スタート位置
	startWT_.Initialize();
	// ゴール位置
	goalWT_.Initialize();
	// ブロック
	for (size_t y = 0; y < static_cast<size_t>(kMapSize_.y); y++) {
		for (size_t x = 0; x < static_cast<size_t>(kMapSize_.x); x++) {
			WorldTransform worldTransform;
			worldTransform.Initialize();
			worldTransform.translation_ = { static_cast<float>(x) * kSquareSize_.x , static_cast<float>(y) * kSquareSize_.y, 0.0f };
			worldTransform.UpdateMatrix();
			blockWT_.push_back(worldTransform);
		}
	}

	enemyModel_ = enemymodel;
	cageModel_ = cagemodel;
	startModel_ = startmodel;
	goalModel_ = goalmodel;
	blockModel_ = blockmodel;

	// 敵の数
	enemyNum_ = 0;
	// 檻の数
	cageNum_ = 0;

	StagesLoad();

	Setting(0);

	input_ = Input::GetInstance();

}

void MapEdit::Update(const ViewProjection& viewProjection)
{

	Vector2 pos = { -1.0f, -1.0f};

	// ステージ変更
	ImGui::Begin("Editor");
	ImGui::Text("Stage%d", stageNum_);
	if (ImGui::Button("nextStage")) {
		// グループ名の設定
		std::string groupName = "StageData" + std::to_string(stageNum_);
		SaveData(groupName);
		stageNum_++;
		Setting(stageNum_);
	}
	if (ImGui::Button("preStage") && stageNum_ > 0) {
		// グループ名の設定
		std::string groupName = "StageData" + std::to_string(stageNum_);
		SaveData(groupName);
		stageNum_--;
		Setting(stageNum_);
	}
	if (ImGui::Button("Save")) {
		// グループ名の設定
		std::string groupName = "StageData" + std::to_string(stageNum_);
		SaveData(groupName);
		groupName = "StageData";
		SaveFile(groupName);
	}
	ImGui::End();

	// クリックされた(map変更)
	if (input_->TriggerKey(DIK_SPACE)) {
		pos = BlockFind(viewProjection);
		if (pos.x >= 0.0f) {
			if (stageData_.map_[static_cast<size_t>(pos.y)][static_cast<size_t>(pos.x)] == MapNumber::Road) {
				stageData_.map_[static_cast<size_t>(pos.y)][static_cast<size_t>(pos.x)] = MapNumber::Hole;
			}
			else {
				stageData_.map_[static_cast<size_t>(pos.y)][static_cast<size_t>(pos.x)] = MapNumber::Road;
			}
		}
	}
	//敵
	if (input_->TriggerKey(DIK_E)) {
		pos = BlockFind(viewProjection);
		if (pos.x >= 0.0f) {
			bool find = false;
			stageData_.enemyPosition_.remove_if([pos, &find](Vector2 enemyPosition) {
				if (enemyPosition.x == pos.x && enemyPosition.y == pos.y) {
					find = true;
					return true;
				}
				return false;
			});
			if (find) {
				enemyWT_.remove_if([pos, &find](WorldTransform worldTransform) {
					if (worldTransform.translation_.x == pos.x * kSquareSize_.x && worldTransform.translation_.y == pos.y * kSquareSize_.y) {
						return true;
					}
					return false;
				});
			}
			else {
				DuplicateConfirmation(pos);
				if ( !(stageData_.startPosition_.x == pos.x && stageData_.startPosition_.y == pos.y) &&
					!(stageData_.goalPosition_.x == pos.x && stageData_.goalPosition_.y == pos.y)) {
					stageData_.enemyPosition_.push_back(pos);
					WorldTransform worldTransform;
					worldTransform.Initialize();
					worldTransform.translation_ = { pos.x * kSquareSize_.x , pos.y * kSquareSize_.y, -2.0f };
					worldTransform.UpdateMatrix();
					enemyWT_.push_back(worldTransform);
				}
			}
		}

	}	
	//檻
	if (input_->TriggerKey(DIK_C)) {
		pos = BlockFind(viewProjection);
		if (pos.x >= 0.0f) {
			bool find = false;
			stageData_.cagePosition_.remove_if([pos, &find](Vector2 cagePosition) {
				if (cagePosition.x == pos.x && cagePosition.y == pos.y) {
					find = true;
					return true;
				}
				return false;
				});
			if (find) {
				cageWT_.remove_if([pos, &find](WorldTransform worldTransform) {
					if (worldTransform.translation_.x == pos.x * kSquareSize_.x && worldTransform.translation_.y == pos.y * kSquareSize_.y) {
						return true;
					}
					return false;
					});
			}
			else {
				DuplicateConfirmation(pos);
				if (!(stageData_.startPosition_.x == pos.x && stageData_.startPosition_.y == pos.y) &&
					!(stageData_.goalPosition_.x == pos.x && stageData_.goalPosition_.y == pos.y)) {
					stageData_.cagePosition_.push_back(pos);
					WorldTransform worldTransform;
					worldTransform.Initialize();
					worldTransform.translation_ = { pos.x * kSquareSize_.x , pos.y * kSquareSize_.y, -2.0f };
					worldTransform.UpdateMatrix();
					cageWT_.push_back(worldTransform);
				}
			}
		}

	}
	//スタート
	if (input_->TriggerKey(DIK_S)) {
		pos = BlockFind(viewProjection);
		if (pos.x >= 0.0f) {
			if (!(stageData_.goalPosition_.x == pos.x && stageData_.goalPosition_.y == pos.y)) {
				DuplicateConfirmation(pos);
				stageData_.startPosition_ = pos;
				stageData_.playerPosition_ = pos;
				startWT_.translation_ = { pos.x * kSquareSize_.x , pos.y * kSquareSize_.y, -2.0f };
				startWT_.UpdateMatrix();
			}
		}

	}
	//ゴール
	if (input_->TriggerKey(DIK_G)) {
		pos = BlockFind(viewProjection);
		if (pos.x >= 0.0f) {
			if (!(stageData_.startPosition_.x == pos.x && stageData_.startPosition_.y == pos.y)) {
				DuplicateConfirmation(pos);
				stageData_.goalPosition_ = pos;
				goalWT_.translation_ = { pos.x * kSquareSize_.x , pos.y * kSquareSize_.y, -2.0f };
				goalWT_.UpdateMatrix();
			}
		}

	}

}

void MapEdit::Draw(const ViewProjection& viewProjection)
{

	// エネミーの位置
	for (WorldTransform worldTransform : enemyWT_) {
		enemyModel_->Draw(worldTransform, viewProjection);
	}
	// 檻の位置
	for (WorldTransform worldTransform : cageWT_) {
		cageModel_->Draw(worldTransform, viewProjection);
	}
	// スタート位置
	startModel_->Draw(startWT_, viewProjection);
	// ゴール位置
	goalModel_->Draw(goalWT_, viewProjection);
	// ブロック
	for (WorldTransform worldTransform : blockWT_) {
		Vector2 result = { worldTransform.translation_.x / kSquareSize_.x , worldTransform.translation_.y / kSquareSize_.y };
		if (stageData_.map_[static_cast<size_t>(result.y)][static_cast<size_t>(result.x)] == MapNumber::Road) {
			blockModel_->Draw(worldTransform, viewProjection);
		}
	}

}

void MapEdit::Setting(size_t stageNum)
{

	// 解放とかクリアとか
	// エネミーの位置
	enemyWT_.clear();
	// 檻の位置
	cageWT_.clear();

	// ステージ番号
	stageNum_ = stageNum;

	// グループ名の設定
	std::string groupName = "StageData" + std::to_string(stageNum_);

	if (stageNum_ >= kMaximumNumberOfStages_) {
		AddStage(groupName);
	}
	else {
		// 初期情報
		stageData_.map_ = GetMapValue(groupName, "map_");
		stageData_.playerPosition_ = GetPositionValue(groupName, "playerPosition_");
		stageData_.enemyPosition_ = GetPositionsValue(groupName, "enemyPosition_");
		stageData_.cagePosition_ = GetPositionsValue(groupName, "cagePosition_");
		stageData_.startPosition_ = GetPositionValue(groupName, "startPosition_");
		stageData_.goalPosition_ = GetPositionValue(groupName, "goalPosition_");
	}


	// エネミーの位置
	for (Vector2 pos : stageData_.enemyPosition_) {
		WorldTransform worldTransform;
		worldTransform.Initialize();
		worldTransform.translation_ = { pos.x * kSquareSize_.x , pos.y * kSquareSize_.y, -2.0f };
		worldTransform.UpdateMatrix();
		enemyWT_.push_back(worldTransform);
	}
	// 檻の位置
	for (Vector2 pos : stageData_.cagePosition_) {
		WorldTransform worldTransform;
		worldTransform.Initialize();
		worldTransform.translation_ = { pos.x * kSquareSize_.x , pos.y * kSquareSize_.y, -2.0f };
		worldTransform.UpdateMatrix();
		cageWT_.push_back(worldTransform);
	}
	// スタート位置
	startWT_.Initialize();
	startWT_.translation_ = { stageData_.startPosition_.x * kSquareSize_.x , stageData_.startPosition_.y * kSquareSize_.y, -2.0f };
	startWT_.UpdateMatrix();
	// ゴール位置
	goalWT_.Initialize();
	goalWT_.translation_ = { stageData_.goalPosition_.x * kSquareSize_.x , stageData_.goalPosition_.y * kSquareSize_.y, -2.0f };
	goalWT_.UpdateMatrix();

}

Vector2 MapEdit::BlockFind(const ViewProjection& viewProjection)
{

	POINT mousePos = { static_cast<LONG>(input_->GetMousePosition().x), static_cast<LONG>(input_->GetMousePosition().y) };
	Vector2 mousePosVec = { static_cast<float>(mousePos.x) , static_cast<float>(mousePos.y) };

	ViewProjection viewProjection_ = viewProjection;
	//ビューポート行列
	Matrix4x4 matViewport = MatLib::MakeViewportMatrix(0, 0, WinApp::kClientWidth, WinApp::kClientHeight, 0, 1);

	//ビュー行列とプロジェクション行列,ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport =
		MatLib::Multiply(viewProjection.matView, MatLib::Multiply(viewProjection.matProjection, matViewport));

	// ブロック
	for (WorldTransform worldTransform : blockWT_) {
		Vector3 positionM = { worldTransform.matWorld_.m[3][0] - kSquareSize_.x / 2.0f , worldTransform.matWorld_.m[3][1] + kSquareSize_.y / 2.0f , worldTransform.matWorld_.m[3][2] };
		Vector3 positionP = { worldTransform.matWorld_.m[3][0] + kSquareSize_.x / 2.0f , worldTransform.matWorld_.m[3][1] - kSquareSize_.y / 2.0f , worldTransform.matWorld_.m[3][2] };
		positionM = MatLib::Transform(positionM, matViewProjectionViewport);
		positionP = MatLib::Transform(positionP, matViewProjectionViewport);

		if (mousePosVec.x > positionM.x && mousePosVec.x < positionP.x &&
			mousePosVec.y > positionM.y && mousePosVec.y < positionP.y) {
			
			Vector2 result = { worldTransform.translation_.x / kSquareSize_.x , worldTransform.translation_.y / kSquareSize_.y };
			return result;
		}

	}

	return Vector2( -1.0f, -1.0f);

}

void MapEdit::DuplicateConfirmation(Vector2 pos)
{

	bool find = false;
	// 敵
	stageData_.enemyPosition_.remove_if([pos, &find](Vector2 enemyPosition) {
		if (enemyPosition.x == pos.x && enemyPosition.y == pos.y) {
			find = true;
			return true;
		}
		return false;
		});
	if (find) {
		enemyWT_.remove_if([pos, &find](WorldTransform worldTransform) {
			if (worldTransform.translation_.x == pos.x * kSquareSize_.x && worldTransform.translation_.y == pos.y * kSquareSize_.y) {
				return true;
			}
			return false;
			});
		return;
	}
	// 檻
	stageData_.cagePosition_.remove_if([pos, &find](Vector2 cagePosition) {
		if (cagePosition.x == pos.x && cagePosition.y == pos.y) {
			find = true;
			return true;
		}
		return false;
		});
	if (find) {
		cageWT_.remove_if([pos, &find](WorldTransform worldTransform) {
			if (worldTransform.translation_.x == pos.x * kSquareSize_.x && worldTransform.translation_.y == pos.y * kSquareSize_.y) {
				return true;
			}
			return false;
			});
		return;
	}

}

void MapEdit::StageDatasDelete()
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

void MapEdit::AddStage(const std::string& groupName)
{

	kMaximumNumberOfStages_++;
	stageDatas_[groupName];

	// 初期情報
	int** map;
	map = new int* [static_cast<size_t>(kMapSize_.y)];
	for (size_t y = 0; y < static_cast<size_t>(kMapSize_.y); y++) {
		map[y] = new int[static_cast<size_t>(kMapSize_.x)];
	}
	for (size_t y = 0; y < static_cast<size_t>(kMapSize_.y); y++) {
		for (size_t x = 0; x < static_cast<size_t>(kMapSize_.x); x++) {
			map[y][x] = MapNumber::Road;
		}
	}

	SetValue(groupName, "map_", map);
	stageData_.map_ = GetMapValue(groupName, "map_");

	std::list<Vector2> enemyPosition;
	SetValue(groupName, "enemyPosition_", enemyPosition);
	stageData_.enemyPosition_ = GetPositionsValue(groupName, "enemyPosition_");

	std::list<Vector2> cagePosition;
	SetValue(groupName, "cagePosition_", cagePosition);
	stageData_.cagePosition_ = GetPositionsValue(groupName, "cagePosition_");

	Vector2 startPosition = {0.0f, 0.0f};
	SetValue(groupName, "startPosition_", startPosition);
	stageData_.startPosition_ = GetPositionValue(groupName, "startPosition_");
	SetValue(groupName, "playerPosition_", startPosition);
	stageData_.playerPosition_ = GetPositionValue(groupName, "playerPosition_");

	Vector2 goalPosition = { 14.0f, 14.0f };
	SetValue(groupName, "goalPosition_", goalPosition);
	stageData_.goalPosition_ = GetPositionValue(groupName, "goalPosition_");

}

void MapEdit::StagesLoad()
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

void MapEdit::StageLoad(const std::string& groupName)
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
				std::list<Vector2> values;
				for (size_t k = 0; k < itItem->size(); k += 2) {
					Vector2 value = { itItem->at(k), itItem->at(k + 1) };
					values.push_back(value);
				}
				SetValue(name, itemName, values);
			}

		}
	}

}

void MapEdit::SetValue(const std::string& groupName, const std::string& key, int** value)
{

	// グループの参照を取得
	Group& group = stageDatas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem = value;
	// 設定した項目をstd::mapに追加
	group[key] = newItem;

}

void MapEdit::SetValue(const std::string& groupName, const std::string& key, const Vector2& value) {

	// グループの参照を取得
	Group& group = stageDatas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem = value;
	// 設定した項目をstd::mapに追加
	group[key] = newItem;

}
void MapEdit::SetValue(const std::string& groupName, const std::string& key, std::list<Vector2>& value) {

	// グループの参照を取得
	Group& group = stageDatas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem = value;
	// 設定した項目をstd::mapに追加
	group[key] = newItem;

}

int** MapEdit::GetMapValue(const std::string& groupName, const std::string& key)
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

Vector2 MapEdit::GetPositionValue(const std::string& groupName, const std::string& key)
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

std::list<Vector2> MapEdit::GetPositionsValue(const std::string& groupName, const std::string& key)
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

void MapEdit::SaveData(const std::string& groupName)
{

	for (std::map<std::string, Group>::iterator itGroup = stageDatas_.begin(); itGroup != stageDatas_.end();
		++itGroup) {

		std::string string = itGroup->first;
		if (string == groupName) {

			// 指定グループが存在するか
			assert(stageDatas_.find(string) != stageDatas_.end());
			//  グループの参照を取得
			Group& group = stageDatas_[string];

			group["enemyPosition_"] = stageData_.enemyPosition_;
			group["cagePosition_"] = stageData_.cagePosition_;
			group["startPosition_"] = stageData_.startPosition_;
			group["playerPosition_"] = stageData_.playerPosition_;
			group["goalPosition_"] = stageData_.goalPosition_;
		}

	}


}

void MapEdit::SaveFile(const std::string& groupName)
{

	json root;
	root = json::object();

	for (size_t i = 0; i < kMaximumNumberOfStages_; i++) {


		std::string name = groupName + std::to_string(i);
		// グループを検索
		std::map<std::string, Group>::iterator itGroup = stageDatas_.find(name);

		// 未登録チェック
		assert(itGroup != stageDatas_.end());

		// jsonオブジェクト登録
		root[name] = json::object();

		// 各項目について
		for (std::map<std::string, Item>::iterator itItem = itGroup->second.begin();
			itItem != itGroup->second.end(); ++itItem) {

			// 項目名を取得
			const std::string& itemName = itItem->first;
			// 項目の参照を取得
			Item& item = itItem->second;

			// int32_t型の値を保持していれば
			if (std::holds_alternative<int**>(item)) {
				// int32_t型の値を登録
				int** values = std::get<int**>(item);
				for (size_t y = 0; y < static_cast<size_t>(kMapSize_.y); y++) {
					for (size_t x = 0; x < static_cast<size_t>(kMapSize_.x); x++) {
						root[name][itemName] += values[y][x];
					}
				}

			}
			else if (std::holds_alternative<Vector2>(item)) {
				Vector2 value = std::get<Vector2>(item);
				root[name][itemName] = json::array({ value.x, value.y });
			}
			else if (std::holds_alternative<std::list<Vector2>>(item)) {
				// float型のjson配列登録
				std::list<Vector2> values = std::get<std::list<Vector2>>(item);
				for (Vector2 value : values) {
					root[name][itemName] += value.x;
					root[name][itemName] += value.y;
				}
			}
		}

	}

	// ディレクトリがなければ作成する
	std::filesystem::path dir(kDirectoryPath);
	if (!std::filesystem::exists(kDirectoryPath)) {
		std::filesystem::create_directories(kDirectoryPath);
	}
	// 書き込むJSONファイルのフルパスを合成する
	std::string filePath = kDirectoryPath + groupName + ".json";
	// 書き込み用ファイルストリーム
	std::ofstream ofs;
	// ファイルを書き込み用に開く
	ofs.open(filePath);

	// ファイルオープン失敗？
	if (ofs.fail()) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);
		return;
	}
	// ファイルにjson文字列を書き込む(インデント幅4)
	ofs << std::setw(4) << root << std::endl;
	// ファイルを閉じる
	ofs.close();

}
