#include "GameScene.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include <cassert>
#include <functional>
#include "VectorLib.h"
#include "EffectManager.h"

using namespace std;

GameScene::GameScene()
{
	// プレイヤー
	playerModel_.reset(Model::CreateFromObj("player", true));
	blockModel_.reset(Model::CreateFromObj("block", true));
	enemyModel_.reset(Model::CreateFromObj("enemy", true));
	enemyMovePlanModel_.reset(Model::CreateFromObj("Arrow", true));
	cageModel_.reset(Model::CreateFromObj("Cage", true));
	startModel_.reset(Model::CreateFromObj("start", true));
	goalModel_.reset(Model::CreateFromObj("Goal", true));
	skyDomeModel_.reset(Model::CreateFromObj("tenkyu", true));
	enemyDagerModel_.reset(Model::CreateFromObj("danger", true));

	stageNumberTextureHandle_ = TextureManager::Load("Image/stageNumber.png");
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	viewProjection_.Initialize();

	skyDomeWorldTransform_.Initialize();
	skyDomeWorldTransform_.translation_ = { 70.0f,10.0f, -40.0f};
	skyDomeWorldTransform_.scale_ = { 50.0f,50.0f,50.0f };

	baseCamera_ = make_unique<BaseCamera>();
	baseCamera_->Initialize();

	baseCamera_->SetPosition({ 10.0f, 20.0f, -70.0f });
	baseCamera_->SetRotation({ 0.0f, 0.0f, 0.0f });

	// エフェクト
	effectManager_ = make_unique<EffectManager>();
	effectManager_->Initialize();
	// パーティクルエフェクト
	particleManager_ = make_unique<ParticleManager>();
	particleManager_->Initialize(baseCamera_->GetViewPlayer());

	// ゲームシーン用

	// コマンド
	command_ = make_unique<Command>();
	command_->Initialize();

	// マップ
	mapSystem_ = make_unique<MapSystem>();
	mapSystem_->SetParticleManager(particleManager_.get());
	mapSystem_->Initialize(stageNum);

	player_ = make_unique<Player>();
	player_->Initialize(playerModel_.get(), mapSystem_->GetInitialPlayerPosition());
	player_->SetPosition(mapSystem_->GetPlayerPosition());
	// マップシステム
	mapSystem_->SetPlayer(player_.get());

	// ブロック
	uint32_t blockWalltextureHandle = TextureManager::Load("block/block_00.png");
	uint32_t blockLoadtextureHandle = TextureManager::Load("block/block_00.png");
	uint32_t blockHoletextureHandle = TextureManager::Load("block/block_01.png");
	blocktextureHandles_.push_back(blockWalltextureHandle);
	blocktextureHandles_.push_back(blockLoadtextureHandle);
	blocktextureHandles_.push_back(blockHoletextureHandle);
	blockManager_ = make_unique<BlockManager>();
	blockManager_->Initialize(blockModel_.get(), blocktextureHandles_, mapSystem_->GetMap());
	// マップシステム
	mapSystem_->SetBlockManager(blockManager_.get());

	// エネミー
	enemiesManager_ = make_unique<EnemiesManager>();
	enemiesManager_->Iintialize(mapSystem_.get(), enemyModel_.get(), enemyMovePlanModel_.get(), cageModel_.get(), enemyDagerModel_.get(), mapSystem_->GetEnemyCount(), mapSystem_->GetCageCount());
	// マップシステム
	mapSystem_->SetEnemiesManager(enemiesManager_.get());

	// スタート
	start_ = make_unique<Start>();
	start_->Initialize(startModel_.get(), mapSystem_->GetInitialStartPosition());
	// マップシステム
	mapSystem_->SetStart(start_.get());

	// ゴール
	goal_ = make_unique<Goal>();
	goal_->Initialize(goalModel_.get(), mapSystem_->GetInitialGoalPosition());
	// マップシステム
	mapSystem_->SetGoal(goal_.get());

	// アニメーションマネージャー
	animationManager_ = make_unique<AnimationManager>();
	animationManager_->Initialize();
	// 待機アニメーションを設定していく
	SetWaitingAnimation();

	

	// 敵の数カウント
	// テクスチャ
	// EnemyKazu
	enmyKazuTextureHandle_ = TextureManager::Load("gameSceneUI/enemyKazu.png");
	// num
	numTextureHandle_ = TextureManager::Load("gameSceneUI/enemyNumber.png");
	// slash
	slashTextureHandle_ = TextureManager::Load("gameSceneUI/enemyNaname.png");
	captureEnemyUI_ = make_unique<CaptureEnemyUI>();
	captureEnemyUI_->Initialize(enmyKazuTextureHandle_, numTextureHandle_, slashTextureHandle_);
	// マップシステム
	mapSystem_->SetCaptureEnemyUI(captureEnemyUI_.get());
	// ステージ番号
	stageNumberUI_ = make_unique<StageNumberUI>();
	stageNumberUI_->Initialize(stageNumberTextureHandle_);
	stageNumberUI_->Setting(stageNum);

	// マネージャーの設定
	//player_->SetEffectManager(effectManager_.get());

}

void GameScene::Update()
{
	//if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
	//	sceneNum = TITLE;
	//}
	/// カメラ関係の更新処理
	CameraUpdate();

	skyDomeWorldTransform_.UpdateMatrix();
	effectManager_->Update();
	particleManager_->Update();

	if(effectManager_->IsStop()){
		// ヒットストップ関係の時間処理
		effectManager_->HitStopUpdate();
	}
	// ヒットストップしていない
	else {

		//コマンド待ち
		if (command_->GetAcceptingInput()) {
			WaitingCommand();
		}

		//アニメーション
		if (command_->GetAcceptingInput()) {
			WaitingAnimation();
		}
		else {
			ActionAnimation();
		}

	}

	// ゲームオーバーか
	ImGui::Begin("State");
	if (mapSystem_->GetIsGameClear()) {
		ImGui::Text("GAMECLEAR");

		// 最終ステージじゃない
		if (stageNum != stageMax - 1) {
			stageNum++;
			stageNumberUI_->Setting(stageNum);
			Setting(GAMESCENE);
		}
	}
	if (mapSystem_->GetIsGameOver()) {
		ImGui::Text("GAMEOVER");
		mapSystem_->Restart();
		Reset();
	}
	ImGui::End();

	// ESCでゲームセレクトへ
	if (input_->TriggerKey(DIK_ESCAPE)) {
		sceneNum = STAGESELECT;
	}


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


	//uiManager_->Draw();

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

	//skyDomeModel_->Draw(skyDomeWorldTransform_, viewProjection_);
	particleManager_->Draw(viewProjection_);
	player_->Draw(viewProjection_);
	blockManager_->Draw(viewProjection_);
	enemiesManager_->Draw(viewProjection_);
	start_->Draw(viewProjection_);
	goal_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	captureEnemyUI_->Draw();
	stageNumberUI_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::Setting(Scene preScene)
{

	if (preScene == Scene::EDITOR) {
		mapSystem_->StagesLoad();
	}

	mapSystem_->Setting(stageNum);

	Reset();

}

void GameScene::CameraUpdate()
{
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_L)) {
		//if (isDebug_) {
		//	isDebug_ = false;
		//}
		//else {
		//	isDebug_ = true;
		//}
		//cameraVect_ = baseCamera_->GetView().translate_;
		effectManager_->SetIsShake(true);
	}
	if (input_->TriggerKey(DIK_J)) {
		effectManager_->SetIsStop(true);
	}
#endif // DEBUG

	if (effectManager_->IsShake()) {
		effectManager_->ShakeUpdate();
		baseCamera_->SetPosition(EffectManager::ShakeUpdate(baseCamera_->GetInitPosition(), kFloatType));
	}
	else {
		baseCamera_->ResetPosition();
	}

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

void GameScene::WaitingCommand()
{

	// コマンド待ち
	command_->Update();
	// コマンドが入力された
	if (!command_->GetAcceptingInput()) {
		//マップシステムを動かす
		mapSystem_->Update(command_->GetCommandNumber());
		// リセット
		if (mapSystem_->GetIsRestart()) {
			Reset();
		}
		// マップシステムクラスからの更新情報取得
		player_->Update(mapSystem_->GetPlayerPosition());
		blockManager_->Update();
		enemiesManager_->Update();
		start_->Update();
		goal_->Update();
		// アニメーションマネージャーアクションスタート
		animationManager_->ActionStart(player_->GetAnimationTMax());
		//プレイヤー
		animationManager_->SetActionAnimation(std::bind(&Player::ActionAnimationUpdate, player_.get()));
		// ブロックマネージャー
		animationManager_->SetActionAnimation(std::bind(&BlockManager::ActionAnimationUpdate, blockManager_.get()));
	}

}

void GameScene::WaitingAnimation()
{

	// 待機アニメーションする
	animationManager_->WaitUpdate();

}

void GameScene::ActionAnimation()
{

	// 行動アニメーションする
	animationManager_->ActionUpdate();

	// 行動アニメーションカウントがマックスならコマンド待ち
	if (!animationManager_->GetIsActionAnimation()) {
		// コマンド待機状態へ
		command_->SetAcceptingInput(true);
	}

}

void GameScene::Reset()
{

	player_->Setting(mapSystem_->GetInitialPlayerPosition());
	blockManager_->Setting();
	enemiesManager_->Setting(mapSystem_->GetEnemyCount(), mapSystem_->GetCageCount());
	start_->Setting(mapSystem_->GetInitialStartPosition());
	goal_->Setting(mapSystem_->GetInitialGoalPosition());

	mapSystem_->SetIsRestart(false);
	
	captureEnemyUI_->Setting(static_cast<uint32_t>(mapSystem_->GetEnemyCount()));

	//アニメーション関数
	animationManager_->Reset();
	// 待機アニメーションを設定していく
	SetWaitingAnimation();

	// マップシステムクラスからの更新情報取得
	player_->Update(mapSystem_->GetPlayerPosition());
	blockManager_->Update();
	enemiesManager_->Update();
	start_->Update();
	goal_->Update();

}

void GameScene::SetWaitingAnimation()
{
	// プレイヤー
	//player_->WaitingAnimationInitialize();
	//animationManager_->SetWaitingAnimation(std::bind(&Player::WaitingAnimationUpdate, player_.get()));

}

//void GameScene::SetActionAnimation()
//{
//
//}

void GameScene::ModelSetting()
{

}
