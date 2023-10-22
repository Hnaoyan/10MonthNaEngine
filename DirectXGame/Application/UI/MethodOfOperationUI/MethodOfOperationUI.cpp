#include "MethodOfOperationUI.h"

MethodOfOperationUI::~MethodOfOperationUI()
{

	for (size_t i = 0; i < 4; i++) {
		delete moveSprites_[i];
	}

}

void MethodOfOperationUI::Initialize(std::vector<uint32_t> moveTextureHandles, uint32_t vibrationTextureHandle, uint32_t resetTextureHandle, uint32_t stageSelectTextureHandle)
{

	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };
	Vector2 anchorPoint = { 0.5f, 0.5f };

	// 移動 WWAASSDD
	// テクスチャハンドル 8
	moveTextureHandles_ = moveTextureHandles;
	// 位置
	movePosition_ = { 160.0f, 648.0f};
	// サイズ
	moveSize_ = { 128.0f, 64.0f};
	// スプライト 4
	for (size_t i = 0; i < 4; i++) {
		Sprite* moveSprite;
		moveSprite = Sprite::Create(moveTextureHandles_[i], movePosition_, color, anchorPoint, false, false);
		moveSprite->SetSize(moveSize_);
		moveSprite->SetBlendMode(Sprite::BlendMode::kNormal);
		moveSprite->Update();
		moveSprites_.push_back(moveSprite);
	}

	// 振動

	// テクスチャハンドル 2
	vibrationTextureHandle_ = vibrationTextureHandle;
	// 位置
	vibrationPosition_ = { 344.0f, 648.0f };
	// サイズ
	vibrationSize_ = {160.0f, 64.0f};
	// スプライト 1
	vibrationSprite_.reset(Sprite::Create(vibrationTextureHandle_, vibrationPosition_, color, anchorPoint, false, false));
	vibrationSprite_->SetSize(vibrationSize_);
	vibrationSprite_->SetBlendMode(Sprite::BlendMode::kNormal);
	vibrationSprite_->Update();

	// リセット

	// テクスチャハンドル 2
	resetTextureHandle_ = resetTextureHandle;
	// 位置
	resetPosition_ = { 536.0f, 648.0f };
	// サイズ
	resetSize_ = { 144.0f, 64.0f };
	// スプライト 1
	resetSprite_.reset(Sprite::Create(resetTextureHandle_, resetPosition_, color, anchorPoint, false, false));
	resetSprite_->SetSize(resetSize_);
	resetSprite_->SetBlendMode(Sprite::BlendMode::kNormal);
	resetSprite_->Update();

	// ステージセレクト

	// テクスチャハンドル 2
	stageSelectTextureHandle_ = stageSelectTextureHandle;
	// 位置
	stageSelectPosition_ = {760.0f, 648.0f};
	// サイズ
	stageSelectSize_ = {224.0f, 64.0f};
	// スプライト 1
	stageSelectSprite_.reset(Sprite::Create(stageSelectTextureHandle_, stageSelectPosition_, color, anchorPoint, false, false));
	stageSelectSprite_->SetSize(stageSelectSize_);
	stageSelectSprite_->SetBlendMode(Sprite::BlendMode::kNormal);
	stageSelectSprite_->Update();

	// 入力
	input_ = Input::GetInstance();

}

void MethodOfOperationUI::Update()
{

	Vector2 zero = { 0.0f,0.0f };

	// 移動 WWAASSDD
	Vector2 moveTexBase = {128.0f, 0.0f };
	// W
	if (input_->PressKey(DIK_W) || input_->PressKey(DIK_UP)) {
		moveSprites_[0]->SetSpriteRect(moveTexBase, moveSize_);
	}
	else {
		moveSprites_[0]->SetSpriteRect(zero, moveSize_);
	}
	moveSprites_[0]->Update();
	// A
	if (input_->PressKey(DIK_A) || input_->PressKey(DIK_LEFT)) {
		moveSprites_[1]->SetSpriteRect(moveTexBase, moveSize_);
	}
	else {
		moveSprites_[1]->SetSpriteRect(zero, moveSize_);
	}
	moveSprites_[1]->Update();
	// S
	if (input_->PressKey(DIK_S) || input_->PressKey(DIK_DOWN)) {
		moveSprites_[2]->SetSpriteRect(moveTexBase, moveSize_);
	}
	else {
		moveSprites_[2]->SetSpriteRect(zero, moveSize_);
	}
	moveSprites_[2]->Update();
	// D
	if (input_->PressKey(DIK_D) || input_->PressKey(DIK_RIGHT)) {
		moveSprites_[3]->SetSpriteRect(moveTexBase, moveSize_);
	}
	else {
		moveSprites_[3]->SetSpriteRect(zero, moveSize_);
	}
	moveSprites_[3]->Update();

	// 振動
	Vector2 vibrationTexBase = { 160.0f, 0.0f };
	if (input_->PressKey(DIK_SPACE)) {
		vibrationSprite_->SetSpriteRect(vibrationTexBase, vibrationSize_);
	}
	else {
		vibrationSprite_->SetSpriteRect(zero, vibrationSize_);
	}

	// リセット
	Vector2 resetTexBase = { 144.0f, 0.0f };
	if (input_->PressKey(DIK_R)) {
		resetSprite_->SetSpriteRect(resetTexBase, resetSize_);
	}
	else {
		resetSprite_->SetSpriteRect(zero, resetSize_);
	}

	// ステージセレクト
	Vector2 stageSelectTexBase = { 224.0f, 0.0f };
	if (input_->PressKey(DIK_ESCAPE)) {
		stageSelectSprite_->SetSpriteRect(stageSelectTexBase, stageSelectSize_);
	}
	else {
		stageSelectSprite_->SetSpriteRect(zero, stageSelectSize_);
	}

}

void MethodOfOperationUI::Draw()
{

	for (size_t i = 0; i < 4; i++) {
		moveSprites_[i]->Draw();
	}
	vibrationSprite_->Draw();
	resetSprite_->Draw();
	stageSelectSprite_->Draw();

}
