#include "GameScene.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include <cassert>
#include <functional>
#include "VectorLib.h"
#include "EffectManager.h"

using namespace std;

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

	// コマンド
	command_ = make_unique<Command>();
	command_->Initialize();

	// マップ
	mapSystem_ = make_unique<MapSystem>();
	mapSystem_->Initialize(0);

	// プレイヤー
	playerModel_.reset(Model::CreateFromObj("player", true));
	player_ = make_unique<Player>();
	player_->Initialize(playerModel_.get());
	player_->SetPosition(mapSystem_->GetPlayerPosition());

	// ブロック
	blockModel_.reset(Model::CreateFromObj("block", true));
	uint32_t blockWalltextureHandle = TextureManager::Load("uvChecker.png");
	uint32_t blockLoadtextureHandle = TextureManager::Load("uvChecker.png");
	blocktextureHandles_.push_back(blockWalltextureHandle);
	blocktextureHandles_.push_back(blockLoadtextureHandle);
	blockManager_ = make_unique<BlockManager>();
	blockManager_->Initialize(blockModel_.get(), blocktextureHandles_, mapSystem_->GetMap());

	// エネミー

	// エフェクト
	effectManager_ = make_unique<EffectManager>();
	effectManager_->Initialize();
	// パーティクルエフェクト
	//particleManager_ = make_unique<ParticleManager>();
	//particleManager_->Initialize(baseCamera_->GetViewPlayer());

	// マネージャーの設定
	//player_->SetEffectManager(effectManager_.get());
	
	// UIの設定
	//uint32_t sprite = TextureManager::Load("uvChecker.png");
	//string spName_ = "UV";
	//uint32_t ui = TextureManager::Load("white1x1.png");
	//string uiName_ = "white";
	//uiManager_ = make_unique<UIManager>();
	//uiManager_->AddUI(sprite, { 200,100 }, { 0.0f,0.0f }, spName_);
	//uiManager_->AddUI(ui, { 100,50 }, { 0.5f,0.5f }, uiName_);


}

void GameScene::Update()
{
	/// カメラ関係の更新処理
	CameraUpdate();

	uiManager_->Update();
	effectManager_->Update();
	particleManager_->Update();

	if (input_->TriggerKey(DIK_9)) {
		particleManager_->RandomRespown(player_->GetPosition());
	}
	if (input_->TriggerKey(DIK_0)) {
		Vector3 pos = { player_->GetPosition().x,player_->GetPosition().y - 1.0f,player_->GetPosition().z };
		particleManager_->Test(pos);
	}

	if(effectManager_->IsStop()){
		// ヒットストップ関係の時間処理
		effectManager_->HitStopUpdate();
	}
	else {
		// コマンド
		command_->Update();
		// マップ
		if (!command_->GetAcceptingInput()) {
			mapSystem_->Update(command_->GetCommandNumber());
			command_->SetAcceptingInput(true);
		}
		player_->Update(mapSystem_->GetPlayerPosition());
		blockManager_->Update();

	}

	// ゲームオーバーか

	// リセット
	if (input_->TriggerKey(DIK_R)) {
		Reset();
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

	//particleManager_->Draw(viewProjection_);
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

void GameScene::Reset()
{

	// プレイヤー

	// ブロックマネージャー

	// エネミー

}

void GameScene::ModelSetting()
{

}
