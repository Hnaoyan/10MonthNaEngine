#include "MapEdit.h"
#include <cassert>
#include <json.hpp>
#include <fstream>
#include <WinApp.h>
#include <MatLib.h>

//名前空間
using namespace nlohmann;

// 静的メンバ定数
// マップのサイズ
const Vector2 MapEdit::kMapSize_ = { 15.0f, 15.0f };
// マスのサイズ
const Vector2 MapEdit::kSquareSize_ = { 10.0f, 10.0f };
// ステージ数
uint32_t MapEdit::kMaximumNumberOfStages_ = 2;

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
		BlockFind(viewProjection);
		if (pos.x >= 0.0f) {

		}

	}	
	//檻
	if (input_->TriggerKey(DIK_C)) {
		BlockFind(viewProjection);
		if (pos.x >= 0.0f) {

		}

	}
	//スタート
	if (input_->TriggerKey(DIK_S)) {
		BlockFind(viewProjection);
		if (pos.x >= 0.0f) {

		}

	}
	//ゴール
	if (input_->TriggerKey(DIK_G)) {
		BlockFind(viewProjection);
		if (pos.x >= 0.0f) {

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

	// 初期情報
	stageData_.map_ = GetMapValue(groupName, "map_");
	stageData_.playerPosition_ = GetPositionValue(groupName, "playerPosition_");
	stageData_.enemyPosition_ = GetPositionsValue(groupName, "enemyPosition_");
	stageData_.cagePosition_ = GetPositionsValue(groupName, "cagePosition_");
	stageData_.startPosition_ = GetPositionValue(groupName, "startPosition_");
	stageData_.goalPosition_ = GetPositionValue(groupName, "goalPosition_");

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
	//クライアントエリア座標に変換する
	//HWND hwnd = WinApp::GetInstance()->GetHwnd();
	//ScreenToClient(hwnd, &mousePos);
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
			
			//	mousePosVec.y > positionM.y && mousePosVec.y < positionP.y

 			Vector2 result = { worldTransform.translation_.x / kSquareSize_.x , worldTransform.translation_.y / kSquareSize_.y };
			return result;
		}

	}

	return Vector2( -1.0f, -1.0f);

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

		StageLoad(filePath.stem().string(), static_cast<size_t>(MapEdit::kMaximumNumberOfStages_));

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

	// ファイル読み込み
	for (size_t s = 0; s < num; s++) {

		// グループを検索
		std::string name = groupName + std::to_string(s);

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
