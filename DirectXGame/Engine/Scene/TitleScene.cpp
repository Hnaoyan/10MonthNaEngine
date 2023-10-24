#include "TitleScene.h"
#include "TextureManager.h"
#include "VectorLib.h"
#include <numbers>

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

}

void TitleScene::Update()
{
	waveAnimation_t_ += 0.01f;
	float amplitude_ = 0.5f;
	float fre = 5.0f;
	waveVelocity_.y = amplitude_ * std::cosf(1.0f * float(std::numbers::pi) * fre * waveAnimation_t_);

	spritePosition_ = VectorLib::Add(spritePosition_, waveVelocity_);
	titleSprite_->SetPosition(spritePosition_);
	titleSprite_->Update();
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		sceneNum = STAGESELECT;
	}

	if (Input::GetInstance()->PressKey(DIK_SPACE)) {
		gameStartSprite_->SetSpriteRect({ gameStartSize_.x, 0.0f }, gameStartSize_);
		gameStartSprite_->Update();
	}
	else {
		gameStartSprite_->SetSpriteRect({ 0.0f, 0.0f }, gameStartSize_);
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
