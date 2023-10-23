#include "TitleScene.h"
#include "TextureManager.h"
#include "VectorLib.h"
#include <numbers>

TitleScene::TitleScene()
{
	titleTexture_ = TextureManager::Load("Image/title.png");
}

void TitleScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	//titleBGM_ = audio_->LoadWave("maou_se_system49.wav");
	spritePosition_ = { float(WinApp::kClientWidth / 2),float(WinApp::kClientHeight / 2) };
	titleSprite_.reset(Sprite::Create(titleTexture_, spritePosition_,
		{ 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f }, false, false));
	//audio_->PlayWave(titleBGM_, true, 0.3f);
}

void TitleScene::Update()
{
	waveAnimation_t_ += 0.01f;
	float amplitude = 0.5f;
	float fre = 5.0f;
	waveVelocity_.y = amplitude * std::cosf(1.0f * float(std::numbers::pi) * fre * waveAnimation_t_);

	spritePosition_ = VectorLib::Add(spritePosition_, waveVelocity_);
	titleSprite_->SetPosition(spritePosition_);
	titleSprite_->Update();
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		sceneNum = STAGESELECT;
	}
}

void TitleScene::Draw()
{	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	titleSprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
}
