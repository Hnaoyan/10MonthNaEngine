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
	sleepModel_.reset(Model::CreateFromObj("Sleep", true));
	enemyMovePlanModel_.reset(Model::CreateFromObj("Arrow", true));
	cageModel_.reset(Model::CreateFromObj("Cage", true));
	goalModel_.reset(Model::CreateFromObj("Goal", true));
	skyDomeModel_.reset(Model::CreateFromObj("tenkyu", true));
	enemyDagerModel_.reset(Model::CreateFromObj("danger", true));

	stageNumberTextureHandle_ = TextureManager::Load("Image/stageNumber.png");

}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();


#pragma region オーディオリソース
	this->clearSEHandle_ = audio_->LoadWave("SE/clear.wav");
	this->deathSEHandle_ = audio_->LoadWave("SE/death.wav");
	this->dontMoveSEHandle_ = audio_->LoadWave("SE/DontMove.wav");
	this->dropSEHandle_ = audio_->LoadWave("SE/drop.wav");
	this->enemyGetSEHandle_ = audio_->LoadWave("SE/enemyGet.wav");
	this->enemyWakeUpSEHandle_ = audio_->LoadWave("SE/enemyWakeUp.wav");
	this->jumpSEHandle_ = audio_->LoadWave("SE/jump.wav");
	this->walkSEHandle_ = audio_->LoadWave("SE/walk.wav");
#pragma endregion


	viewProjection_.Initialize();

	skyDomeWorldTransform_.Initialize();
	skyDomeWorldTransform_.translation_ = { 70.0f,35.0f, -60.0f};
	skyDomeWorldTransform_.scale_ = { 60.0f,60.0f,60.0f };

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
	enemiesManager_->Iintialize(mapSystem_.get(), enemyModel_.get(), sleepModel_.get(), enemyMovePlanModel_.get(), cageModel_.get(), enemyDagerModel_.get(), mapSystem_->GetEnemyCount(), mapSystem_->GetCageCount());
	// マップシステム
	mapSystem_->SetEnemiesManager(enemiesManager_.get());

	// ゴール
	goalRedTextureHandle_ = TextureManager::Load("goal/Goal_00.png");
	goalBlueTextureHandle_ = TextureManager::Load("goal/Goal_01.png");
	goal_ = make_unique<Goal>();
	goal_->Initialize(goalModel_.get(), mapSystem_->GetInitialGoalPosition(), goalRedTextureHandle_, goalBlueTextureHandle_);
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
	// goGoal
	goGoalTextureHandle_ = TextureManager::Load("gameSceneUI/goGoal.png");
	captureEnemyUI_ = make_unique<CaptureEnemyUI>();
	captureEnemyUI_->Initialize(enmyKazuTextureHandle_, numTextureHandle_, slashTextureHandle_, goGoalTextureHandle_);
	// マップシステム
	mapSystem_->SetCaptureEnemyUI(captureEnemyUI_.get());
	// ステージ番号
	stageNumberUI_ = make_unique<StageNumberUI>();
	stageNumberUI_->Initialize(stageNumberTextureHandle_);
	stageNumberUI_->Setting(stageNum);

	// 操作方法
	uint32_t moveTextureHandle;
	moveTextureHandle = TextureManager::Load("stageUi/keyW.png");
	moveTextureHandles_.push_back(moveTextureHandle);
	moveTextureHandle = TextureManager::Load("stageUi/keyA.png");
	moveTextureHandles_.push_back(moveTextureHandle);
	moveTextureHandle = TextureManager::Load("stageUi/keyS.png");
	moveTextureHandles_.push_back(moveTextureHandle);
	moveTextureHandle = TextureManager::Load("stageUi/keyD.png");
	moveTextureHandles_.push_back(moveTextureHandle);
	vibrationTextureHandle_ = TextureManager::Load("stageUi/sindoUi.png");
	resetTextureHandle_ = TextureManager::Load("stageUi/retryUi.png");
	stageSelectTextureHandle_ = TextureManager::Load("stageUi/stageSelectBackUi.png");

	methodOfOperationUI_ = make_unique<MethodOfOperationUI>();
	methodOfOperationUI_->Initialize(moveTextureHandles_, vibrationTextureHandle_, resetTextureHandle_, stageSelectTextureHandle_);

	// マネージャーの設定
	//player_->SetEffectManager(effectManager_.get());
	this->skyDomeWorldTransform_.translation_ = viewProjection_.translate_;

}

void GameScene::Update()
{
	/// カメラ関係の更新処理
	CameraUpdate();

	skyDomeWorldTransform_.UpdateMatrix();
	effectManager_->Update();
	particleManager_->Update();

	if (Input::GetInstance()->TriggerKey(DIK_V)) {
		particleManager_->ExplosionUpdate(player_->GetWorldTransformPosition());
	}

	if(effectManager_->IsStop()){
		// ヒットストップ関係の時間処理
		effectManager_->HitStopUpdate();
	}
	// ヒットストップしていない
	else {

		// オープニングアニメーション
		if (animationManager_->GetIsOpeningAnimation()) {
			OpeningAnimation();
		}
		else if (animationManager_->GetIsGameClearAnimation()) {
			GameClearAnimation();
		}
		else if (animationManager_->GetIsGameOverAnimation()) {
			GameOverAnimation();
		}
		// ゲーム中
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

	}

	// ゲームオーバーか
	ImGui::Begin("State");
	if (mapSystem_->GetIsGameClear() && 
		!animationManager_->GetIsActionAnimation() &&
		!animationManager_->GetIsGameClearAnimation()) {

		ImGui::Text("GAMECLEAR");
		// クリアアニメーション
		animationManager_->GameClearInitialize();
		// プレイヤー
		player_->ClearAnimationInitialize();
		animationManager_->SetGameClearAnimation(std::bind(&Player::ClearAnimationUpdate, player_.get()));
		animationManager_->SetGameClearAnimationTime(player_->GetAnimationFrame());

	}
	if (mapSystem_->GetIsGameOver() &&
		!animationManager_->GetIsGameOverAnimation()) {
		animationManager_->GameOverInitialize();
		animationManager_->SetGameOverAnimationTime(10);
		particleManager_->ExplosionUpdate(player_->GetWorldTransformPosition());
		//ImGui::Text("GAMEOVER");
		//mapSystem_->Restart();
		//Reset();
	}
	ImGui::End();

	// ESCでゲームセレクトへ
	if (input_->TriggerKey(DIK_ESCAPE)) {
		sceneNum = STAGESELECT;
	}

	//UI更新
	methodOfOperationUI_->Update();
	stageNumberUI_->Update();
	captureEnemyUI_->Update();

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

	skyDomeModel_->Draw(skyDomeWorldTransform_, viewProjection_);
	particleManager_->Draw(viewProjection_);
	player_->Draw(viewProjection_);
	blockManager_->Draw(viewProjection_, 
		static_cast<int>(mapSystem_->GetPositionMax().x),
		static_cast<int>(mapSystem_->GetPositionMax().y));
	enemiesManager_->Draw(viewProjection_);
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
	methodOfOperationUI_->Draw();

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
	// ステージ番号UIの更新
	stageNumberUI_->Setting(stageNum);

	Reset();

	// オープニングアニメーション
	animationManager_->OpeningInitialize();
	animationManager_->SetOpeningAnimation(std::bind(&BaseCamera::OpeningAnimationUpdate, baseCamera_.get()));
	baseCamera_->OpeningAnimationInitialize();
	animationManager_->SetOpeningAnimationTime(baseCamera_->GetOpeningFrame());
	command_->SetAcceptingInput(false);

}

void GameScene::CameraUpdate()
{

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
		else {
			// マップシステムクラスからの更新情報取得
			player_->Update(mapSystem_->GetPlayerPosition());
			blockManager_->Update();
			enemiesManager_->Update();
			goal_->Update();
			// アニメーションマネージャーアクションスタート
			animationManager_->ActionInitialize(player_->GetAnimationFrame());
			//プレイヤー
			animationManager_->SetActionAnimation(std::bind(&Player::ActionAnimationUpdate, player_.get()));
			// ブロックマネージャー
			animationManager_->SetActionAnimation(std::bind(&BlockManager::ActionAnimationUpdate, blockManager_.get()));
			// エネミーマネージャー
			animationManager_->SetActionAnimation(std::bind(&EnemiesManager::ActionAnimationUpdate, enemiesManager_.get()));
		}

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

void GameScene::GameClearAnimation()
{

	// 行動アニメーションする
	animationManager_->GameClearUpdate();

	// 行動アニメーションカウントがマックスならコマンド待ち
	if (!animationManager_->GetIsGameClearAnimation()) {
		// 最終ステージじゃない
		if (stageNum != stageMax - 1) {
			stageNum++;
			stageNumberUI_->Setting(stageNum);
			Setting(GAMESCENE);
		}
		else {
			sceneNum = STAGESELECT;
		}
	}

}

void GameScene::GameOverAnimation()
{

	animationManager_->GameOverUpdate();

	if (!animationManager_->GetIsGameOverAnimation()) {
		ImGui::Text("GAMEOVER");
		mapSystem_->Restart();
		Reset();
	}
}

void GameScene::OpeningAnimation()
{

	// 行動アニメーションする
	animationManager_->OpeningUpdate();

	// 行動アニメーションカウントがマックスならコマンド待ち
	if (!animationManager_->GetIsOpeningAnimation()) {
		// コマンド待機状態へ
		command_->SetAcceptingInput(true);
	}

}

void GameScene::Reset()
{

	player_->Setting(mapSystem_->GetInitialPlayerPosition());
	blockManager_->Setting();
	enemiesManager_->Setting(mapSystem_->GetEnemyCount(), mapSystem_->GetCageCount());
	goal_->Setting(mapSystem_->GetInitialGoalPosition());
	Vector3 cameraNewPosition = baseCamera_->GetInitPosition();
	Vector2 mapMaxSize = { mapSystem_->GetPositionMax().x * MapSystem::kSquareSize_.x ,mapSystem_->GetPositionMax().y * MapSystem::kSquareSize_.y};

	float offset_Z = -155.0f;
	float offset_Y = 100.0f;
	baseCamera_->SetBasePosition({ mapMaxSize.x / 2,mapMaxSize.y / 2 - offset_Y,offset_Z });

	particleManager_->GoalEffectReset();

	mapSystem_->SetIsRestart(false);
	
	captureEnemyUI_->Setting(static_cast<uint32_t>(mapSystem_->GetEnemyCount()));

	//アニメーション関数
	animationManager_->Reset();
	// 待機アニメーションを設定していく
	SetWaitingAnimation();

	// マップシステムクラスからの更新情報取得
	player_->Update(mapSystem_->GetPlayerPosition());
	blockManager_->Update();
	enemiesManager_->SetDirect(0);
	enemiesManager_->Update();
	enemiesManager_->ActionAnimationInitialize();
	goal_->Update();

}

void GameScene::SetWaitingAnimation()
{
	// プレイヤー
	//player_->WaitingAnimationInitialize();
	//animationManager_->SetWaitingAnimation(std::bind(&Player::WaitingAnimationUpdate, player_.get()));

	// エネミー
	enemiesManager_->WaitingAnimationInitialize();
	animationManager_->SetWaitingAnimation(std::bind(&EnemiesManager::WaitingAnimationUpdate, enemiesManager_.get()));

}

void GameScene::ModelSetting()
{

}
