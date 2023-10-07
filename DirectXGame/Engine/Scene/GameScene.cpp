#include "GameScene.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include <cassert>

using namespace std;

GameScene::GameScene() {}

GameScene::~GameScene()
{
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	viewProjection_.Initialize();

	baseCamera_ = make_unique<BaseCamera>();
	baseCamera_->Initialize();

	baseCamera_->SetPosition({ 10.0f, 20.0f, -70.0f });
	baseCamera_->SetRotation({ 0.0f, 0.0f, 0.0f });

	collisionManager = make_unique<CollisionManager>();

	// ゲームシーン用

	//エリア
	areaModel_.reset(Model::CreateFromObj("area", true));
	area_ = make_unique<Area>();
	area_->Initialize(areaModel_.get());

	// プレイヤー
	playerModel_.reset(Model::CreateFromObj("player", true));
	player_ = make_unique<Player>();
	player_->Initialize(playerModel_.get());
	player_->SetArea(area_.get());

	//ブロック
	blockModel_.reset(Model::CreateFromObj("block", true));
	uint32_t blockTextureHandleScaffold = TextureManager::Load("./Resources/block/scaffold.png");
	uint32_t blockTextureHandleScaffoldColor = TextureManager::Load("./Resources/block/scaffoldColor.png");
	uint32_t blockTextureHandlePlayerAttack = TextureManager::Load("./Resources/block/playerAttack.png");
	uint32_t blockTextureHandleEnemyAttack = TextureManager::Load("./Resources/block/enemyAttack.png");
	blockTextureHandles_.push_back(blockTextureHandleScaffold);
	blockTextureHandles_.push_back(blockTextureHandleScaffoldColor);
	blockTextureHandles_.push_back(blockTextureHandlePlayerAttack);
	blockTextureHandles_.push_back(blockTextureHandleEnemyAttack);

	blockManager_ = make_unique<BlockManager>();
	blockManager_->Initialize(blockModel_.get(), blockTextureHandles_);
	blockManager_->SetArea(area_.get());

	player_->SetBlockManager(blockManager_.get());
	
}

void GameScene::Update()
{

	/// カメラ関係の更新処理
	CameraUpdate();

	// プレイヤー
	player_->Update();

	//ブロックマネージャー
	blockManager_->Update();

	//衝突判定
	CollisionCheak();

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>


	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	
	area_->Draw(viewProjection_);
	player_->Draw(viewProjection_);
	blockManager_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CameraUpdate()
{
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_L)) {
		if (isDebug_) {
			isDebug_ = false;
		}
		else {
			isDebug_ = true;
		}
	}
#endif // DEBUG

	baseCamera_->Update();

	// デバックカメラか追尾カメラ
	if (isDebug_) {

	}
	else {
		viewProjection_.matView = baseCamera_->GetView().matView;
		viewProjection_.matProjection = baseCamera_->GetView().matProjection;
		viewProjection_.TransferMatrix();
	}


}

void GameScene::CollisionCheak()
{

	// リストをクリア
	collisionManager->ListClear();

	// コライダーをリストに登録
	//プレイヤー
	collisionManager->ListRegister(player_->GetColliderAddress());
	//ブロック
	for (Block* block : blockManager_->GetBlocks()) {
		collisionManager->ListRegister(block->GetColliderAddress());
	}
	//ボスエネミー

	// 当たり判定を取る
	collisionManager->CheakAllCollision();

}
