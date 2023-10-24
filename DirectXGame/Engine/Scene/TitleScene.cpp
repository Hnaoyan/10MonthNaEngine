#include "TitleScene.h"
#include "TextureManager.h"
#include "VectorLib.h"
#include <numbers>
#include <GlobalVariables.h>

TitleScene::TitleScene()
{
	titleTexture_ = TextureManager::Load("Image/title.png");
	gameStartTexture_ = TextureManager::Load("gameStart/gameStart.png");
}

void TitleScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	spritePosition_ = { float(WinApp::kClientWidth / 2),float(WinApp::kClientHeight / 3.0f) };
	titleSprite_.reset(Sprite::Create(titleTexture_, spritePosition_,
		{ 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f }, false, false));

	// ゲームスタートポジション
	gameStartPosition_ = { float(WinApp::kClientWidth / 2.0f),float(WinApp::kClientHeight * 3.0f / 4.0f) };
	// ゲームスタートサイズ
	gameStartSize_ = { 360.0f, 144.0f };
	// ゲームスタートスプライト
	gameStartSprite_.reset(Sprite::Create(gameStartTexture_, gameStartPosition_,
		{ 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f }, false, false));
	gameStartSprite_->SetSize(gameStartSize_);
	gameStartSprite_->SetSpriteRect({ 0.0f, 0.0f }, gameStartSize_);
	gameStartSprite_->Update();

#pragma region 調整項目クラス
	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名設定
	const char* groupName = "Title";
	// 指定した名前でグループ追加
	globalVariables->CreateGroup(groupName);

	// メンバ変数の調整したい項目をグローバル変数に追加
	// ゲームスタートポジション
	globalVariables->AddItem(groupName, "gameStartPosition", gameStartPosition_);
	// ゲームスタートサイズ
	globalVariables->AddItem(groupName, "gameStartSize", gameStartSize_);

	ApplyGlobalVariables();

#pragma endregion

}

void TitleScene::Update()
{

#ifdef _DEBUG

	ApplyGlobalVariables();
	gameStartSprite_->SetSize(gameStartSize_);
	gameStartSprite_->SetPosition(gameStartPosition_);
	gameStartSprite_->Update();

#endif // _DEBUG


	waveAnimation_t_ += 0.01f;
	// 振幅
	float amplitude = 0.5f;
	// 振動数
	float frequency = 5.0f;
	waveVelocity_.y = amplitude * std::cosf(1.0f * float(std::numbers::pi) * frequency * waveAnimation_t_);

	spritePosition_ = VectorLib::Add(spritePosition_, waveVelocity_);
	titleSprite_->SetPosition(spritePosition_);
	titleSprite_->Update();
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		sceneNum = STAGESELECT;
	}

	if (Input::GetInstance()->PressKey(DIK_SPACE)) {
		gameStartSprite_->SetSpriteRect({ 480.0f, 0.0f }, { 480.0f, 192.0f });
		gameStartSprite_->Update();
	}
	else {
		gameStartSprite_->SetSpriteRect({ 0.0f, 0.0f }, { 480.0f, 192.0f });
		gameStartSprite_->Update();
	}

}

void TitleScene::Draw()
{	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	titleSprite_->Draw();
	gameStartSprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
}

void TitleScene::ApplyGlobalVariables()
{

	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "Title";

	// ゲームスタートポジション
	gameStartPosition_ = globalVariables->GetVector2Value(groupName, "gameStartPosition");
	// ゲームスタートサイズ
	gameStartSize_ = globalVariables->GetVector2Value(groupName, "gameStartSize");

}
