#include "BlockManager.h"

#include "Application/Others/MapSystem/MapSystem.h"
#include <GlobalVariables.h>

BlockManager::~BlockManager()
{

	//block_の解放
	for (Block* block : blocks_) {
		delete block;
	}

}

void BlockManager::Initialize(Model* model, std::vector<uint32_t> textureHandles, int** map)
{

	// モデル
	model_ = model;

	// テクスチャハンドル
	textureHandles_ = textureHandles;

	// マップ
	map_ = map;

	// ブロック
	for (size_t y = 0; y < static_cast<size_t>(MapSystem::kMapSize_.y); y++) {
		for (size_t x = 0; x < static_cast<size_t>(MapSystem::kMapSize_.x); x++) {
			Block* block = new Block();
			textureHandles_ = textureHandles;
			int num = map_[y][x];
			block->Initialize(model_, textureHandles_[num], Vector2(static_cast<float>(x), static_cast<float>(y)), static_cast<MapSystem::MapNumber>(num));
			blocks_.push_back(block);
		}
	}

	Update();

#pragma region 調整項目クラス
	// 調整項目クラスのインスタンス取得
	//GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名設定
	//const char* groupName = "Block";
	// 指定した名前でグループ追加
	//globalVariables->CreateGroup(groupName);

	// メンバ変数の調整したい項目をグローバル変数に追加

	//ApplyGlobalVariables();

#pragma endregion

}

void BlockManager::Update()
{

#ifdef _DEBUG
	ApplyGlobalVariables();

#endif // _DEBUG

	for (Block* block : blocks_) {
		int x = static_cast<int>(block->GetPosition().x);
		int y = static_cast<int>(block->GetPosition().y);
		if (map_[y][x] == MapSystem::MapNumber::Hole &&
			block->GetMapNum() != MapSystem::MapNumber::Hole) {
			block->Fall(textureHandles_[MapSystem::MapNumber::Hole]);
		}
		block->Update();
	}

}

void BlockManager::Draw(const ViewProjection& viewProjection)
{

	for (Block* block : blocks_) {
		block->Draw(viewProjection);
	}

}

void BlockManager::Setting()
{

	//blockの設定
	for (Block* block : blocks_) {
		Vector2 pos = block->GetPosition();
		int num = map_[static_cast<int>(pos.y)][static_cast<int>(pos.x)];
		block->Setting(textureHandles_[num], static_cast<MapSystem::MapNumber>(num));
	}

}

void BlockManager::ActionAnimationUpdate()
{
	for (Block* block : blocks_) {
		if (block->GetFallNow()) {
			block->FallAnimation(easeStartPoint_, easeEndPoint_);
		}
	}
}

void BlockManager::ApplyGlobalVariables()
{

	// 調整項目クラスのインスタンス取得
	//GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名の設定
	//const char* groupName = "Block";

}
