#include "StageSelectScene.h"
#include <TextureManager.h>

void StageSelectScene::Initialize()
{

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// テクスチャハンドル (ステージ数)
	for (size_t i = 0; i < stageMax; i++) {
		uint32_t textureHandle = TextureManager::Load("stagePhot/stagePhot" + std::to_string(i) + ".png");
		stagePhotTextureHandles_.push_back(textureHandle);
	}

	// テクスチャハンドル
	leftTextureHandle_ = TextureManager::Load("Image/stageSelectArrowLeft.png");
	rightTextureHandle_ = TextureManager::Load("Image/stageSelectArrowRight.png");
	stageSelectTextureHandle_ = TextureManager::Load("Image/stageSelect.png");
	stageNumberTextureHandle_ = TextureManager::Load("Image/stageNumber.png");

	// イージング
	easeSpeed_ = 0.05f;

	// セッティングへ
	// 動いているか
	isMoveRight_ = false;
	isMoveLeft_ = false;

	// イージング
	easeTimer_ = 0.0f;

	//オブジェクト
	// ステージ写真
	stagePhot_ = std::make_unique<StagePhot>();
	stagePhot_->Initialize(stagePhotTextureHandles_, stageMax);
	// UI
	stageSelectUI_ = std::make_unique<StageSelectUI>();
	stageSelectUI_->Initialize(leftTextureHandle_, rightTextureHandle_, stageSelectTextureHandle_, stageNumberTextureHandle_);

}

void StageSelectScene::Update()
{

	if (isMoveRight_) {
		MoveRight();
	}
	else if (isMoveLeft_) {
		MoveLeft();
	}
	else {
		// 入力受付
		if (input_->TriggerKey(DIK_D) || input_->TriggerKey(DIK_RIGHT)) {
			isMoveRight_ = true;
			easeTimer_ = 0;
			stageNum++;
			if (stageNum == stageMax) {
				stageNum = 0;
			}
			stageSelectUI_->SetStageNum(stageNum);
		}
		else if (input_->TriggerKey(DIK_A) || input_->TriggerKey(DIK_LEFT)) {
			isMoveLeft_ = true;
			easeTimer_ = 0;
			stageNum--;
			if (stageNum == -1) {
				stageNum = stageMax - 1;
			}
			stageSelectUI_->SetStageNum(stageNum);
		}
		else if (input_->TriggerKey(DIK_SPACE)) {
			sceneNum = GAMESCENE;
		}
	}

}

void StageSelectScene::Draw()
{

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

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	
	stageSelectUI_->Draw(!isMoveLeft_ && !isMoveRight_);
	stagePhot_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void StageSelectScene::Setting(Scene preScene)
{

	if (preScene == GAMESCENE) {
		stageSelectUI_->SetStageNum(stageNum);
		stagePhot_->Setting(stageNum);
	}

}

void StageSelectScene::MoveRight()
{

	easeTimer_ += easeSpeed_;
	if (easeTimer_ >= 1.0f) {
		isMoveRight_ = false;
		stagePhot_->MoveRight(1.0f);
	}
	else {
		stagePhot_->MoveRight(easeTimer_);
	}
}

void StageSelectScene::MoveLeft()
{
	easeTimer_ += easeSpeed_;
	if (easeTimer_ >= 1.0f) {
		isMoveLeft_ = false;
		stagePhot_->MoveLeft(1.0f);
	}
	else {
		stagePhot_->MoveLeft(easeTimer_);
	}

}
