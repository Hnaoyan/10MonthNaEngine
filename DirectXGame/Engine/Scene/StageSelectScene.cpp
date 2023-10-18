#include "StageSelectScene.h"
#include <TextureManager.h>

void StageSelectScene::Initialize()
{

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// テクスチャハンドル (ステージ数)
	const size_t StageMax = 5;
	for (size_t i = 0; i < StageMax; i++) {
		uint32_t textureHandle = TextureManager::Load("stagePhot/stagePhot" + std::to_string(i + 1) + ".png");
		textureHandles_.push_back(textureHandle);
	}

	// スプライト固定座標
	for (size_t i = 0; i < 5; i++) {
		positions_[i] = {200.0f * i, 360.0f};
	}

	positions_[2] = { 640.0f, 360.0f };

	// サイズ
	for (size_t i = 0; i < 5; i++) {
		sizes_[i] = {1280, 720} ;
	}

	// スプライト
	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };
	Vector2 anchorPoint = { 0.5f, 0.5f };

	for (size_t i = 0; i < 5; i++) {
		sprite_[i].reset(Sprite::Create(textureHandles_[0], positions_[i], color, anchorPoint, false, false));
		sprite_[i]->SetSize(sizes_[i]);
		sprite_[i]->Update();
	}

	isMoveRight_ = false;
	isMoveLeft_ = false;

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
			easeTimer = 0;
		}
		if (input_->TriggerKey(DIK_A) || input_->TriggerKey(DIK_LEFT)) {
			isMoveLeft_ = true;
			easeTimer = 0;
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
	
	//for (size_t i = 0; i < 5; i++) {
	//	sprite_[i]->Draw();
	//}
	sprite_[2]->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void StageSelectScene::MoveRight()
{

	easeTimer += easeSpeed;
	if (easeTimer >= 1.0f) {
		isMoveRight_ = false;
	}
	else {

	}
}

void StageSelectScene::MoveLeft()
{
	easeTimer += easeSpeed;
	if (easeTimer >= 1.0f) {
		isMoveLeft_ = false;
	}
}
