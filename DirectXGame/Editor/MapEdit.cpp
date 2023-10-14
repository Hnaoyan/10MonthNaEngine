#include "MapEdit.h"
#include <cassert>
#include <json.hpp>
#include <fstream>

//名前空間
using namespace nlohmann;

// 静的メンバ定数
// マップのサイズ
const Vector2 MapEdit::kMapSize_ = { 15.0f, 15.0f };

void MapEdit::Initialize()
{
}

void MapEdit::Update()
{
}

void MapEdit::Draw()
{
}

void MapEdit::StagesLoad()
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

void MapEdit::StageLoad(const std::string& groupName, size_t num)
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
			std::list<Vector2> values;
			for (size_t k = 0; k < itItem->size(); k += 2) {
				Vector2 value = { itItem->at(k), itItem->at(k + 1) };
				values.push_back(value);
			}
			SetValue(name, itemName, values);
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
