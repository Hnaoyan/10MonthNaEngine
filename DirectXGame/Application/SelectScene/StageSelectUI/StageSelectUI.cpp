#include "StageSelectUI.h"
#include "VectorLib.h"
#include <numbers>
#include <GlobalVariables.h>

void StageSelectUI::Initialize(uint32_t leftTextureHandle, uint32_t rightTextureHandle, uint32_t stageSelectTextureHandle,
	uint32_t stageNumberTextureHandle, uint32_t stageUiTextureHandle)
{

	// スプライト
	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };
	Vector2 anchorPoint = { 0.5f, 0.5f };

	// レフト
	leftTextureHandle_ = leftTextureHandle;
	leftPostion_ = { 178.0f, 360.0f };
	leftSize_ = { 128.0f, 192.0f };
	leftSprite_.reset(Sprite::Create(leftTextureHandle_, leftPostion_, color, anchorPoint, false, false));
	leftSprite_->SetSize(leftSize_);
	leftSprite_->SetSpriteRect(Vector2{ 128.0f, 0.0f }, leftSize_);
	leftSprite_->SetBlendMode(Sprite::BlendMode::kNormal);
	leftSprite_->Update();

	// ライト
	rightTextureHandle_ = rightTextureHandle;
	rightPostion_ = { 1102.0f, 360.0f };
	rightSize_ = { 128.0f, 192.0f };
	rightSprite_.reset(Sprite::Create(rightTextureHandle_, rightPostion_, color, anchorPoint, false, false));
	rightSprite_->SetSize(rightSize_);
	rightSprite_->SetSpriteRect(Vector2{ 128.0f, 0.0f }, rightSize_);
	rightSprite_->SetBlendMode(Sprite::BlendMode::kNormal);
	rightSprite_->Update();

	// ステージセレクト
	stageSelectTextureHandle_ = stageSelectTextureHandle;
	stageSelectPostion_ = { 640.0f, 650.0f };
	stageSelectSize_ = { 480.0f, 128.0f };
	stageSelectSprite_.reset(Sprite::Create(stageSelectTextureHandle_, stageSelectPostion_, color, anchorPoint, false, false));
	stageSelectSprite_->SetSize(stageSelectSize_);
	stageSelectSprite_->SetBlendMode(Sprite::BlendMode::kNormal);
	stageSelectSprite_->Update();

	// ステージナンバー
	stageNumberTextureHandle_ = stageNumberTextureHandle;
	stageNumberPostion_ = { 640.0f, 64.0f };
	stageNumberSize_ = { 300.0f, 128.0f };
	stageNumberSprite_.reset(Sprite::Create(stageNumberTextureHandle_, stageNumberPostion_, color, anchorPoint, false, false));
	stageNumberSprite_->SetSize(stageNumberSize_);
	stageNumberSprite_->SetSpriteRect(Vector2{ 0.0f, 0.0f }, Vector2{ 300.0f, 128.0f });
	stageNumberSprite_->SetBlendMode(Sprite::BlendMode::kNormal);
	stageNumberSprite_->Update();

	// ステージナンバー
	stageUiTextureHandle_ = stageUiTextureHandle;
	stageUiPostion_ = { 1100.0f, 650.0f };
	stageUiSize_ = { 320.0f, 128.0f };
	stageUiSprite_.reset(Sprite::Create(stageUiTextureHandle_, stageUiPostion_, color, anchorPoint, false, false));
	stageUiSprite_->SetSize(stageUiSize_);
	stageUiSprite_->SetSpriteRect(Vector2{ 0.0f, 0.0f }, Vector2{ 320.0f, 128.0f });
	stageUiSprite_->SetBlendMode(Sprite::BlendMode::kNormal);
	stageUiSprite_->Update();

	stageNum_ = 0;

#pragma region 調整項目クラス
	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名設定
	const char* groupName = "StageSelectUI";
	// 指定した名前でグループ追加
	globalVariables->CreateGroup(groupName);

	// メンバ変数の調整したい項目をグローバル変数に追加

	globalVariables->AddItem(groupName, "leftPostion", leftPostion_);
	globalVariables->AddItem(groupName, "leftSize", leftSize_);

	globalVariables->AddItem(groupName, "rightPostion", rightPostion_);
	globalVariables->AddItem(groupName, "rightSize", rightSize_);

	globalVariables->AddItem(groupName, "stageSelectPostion", stageSelectPostion_);
	globalVariables->AddItem(groupName, "stageSelectSize", stageSelectSize_);

	globalVariables->AddItem(groupName, "stageNumberPostion", stageNumberPostion_);
	globalVariables->AddItem(groupName, "stageNumberSize", stageNumberSize_);

	globalVariables->AddItem(groupName, "stageUiPostion", stageUiPostion_);
	globalVariables->AddItem(groupName, "stageUiSize", stageUiSize_);

	ApplyGlobalVariables();

#pragma endregion

}

void StageSelectUI::Update()
{

#ifdef _DEBUG
	ApplyGlobalVariables();

	leftSprite_->SetPosition(leftPostion_);
	leftSprite_->SetSize(leftSize_);
	leftSprite_->SetSpriteRect(Vector2{ 0,0 }, leftSize_);
	leftSprite_->Update();

	rightSprite_->SetPosition(rightPostion_);
	rightSprite_->SetSize(rightSize_);
	rightSprite_->SetSpriteRect(Vector2{ 0,0 }, rightSize_);
	rightSprite_->Update();

	stageSelectSprite_->SetPosition(stageSelectPostion_);
	stageSelectSprite_->SetSize(stageSelectSize_);
	stageSelectSprite_->SetSpriteRect(Vector2{ 0,0 }, stageSelectSize_);
	stageSelectSprite_->Update();

	stageNumberSprite_->SetPosition(stageNumberPostion_);
	stageNumberSprite_->SetSize(stageNumberSize_);
	stageNumberSprite_->SetSpriteRect(Vector2{ 0,0 }, stageNumberSize_);
	stageNumberSprite_->Update();


	stageUiSprite_->SetPosition(stageUiPostion_);
	stageUiSprite_->SetSize(stageUiSize_);
	stageUiSprite_->SetSpriteRect(Vector2{ 0,0 }, stageUiSize_);
	stageUiSprite_->Update();

#endif // _DEBUG

}

void StageSelectUI::Draw(bool isMove)
{

	if (isMove) {
		leftSprite_->Draw();
		rightSprite_->Draw();
		stageNumberSprite_->Draw();
		stageUiSprite_->Draw();
	}
	stageSelectSprite_->Draw();

}

void StageSelectUI::Setting()
{

}

void StageSelectUI::SetStageNum(uint32_t stageNum)
{
	
	stageNum_ = stageNum;
	stageNumberSprite_->SetSpriteRect(Vector2{ stageNumberSize_.x * stageNum, 0.0f }, stageNumberSize_);
	stageNumberSprite_->Update();

}

void StageSelectUI::ApplyGlobalVariables()
{

	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "StageSelectUI";

	leftPostion_ = globalVariables->GetVector2Value(groupName, "leftPostion");
	leftSize_ = globalVariables->GetVector2Value(groupName, "leftSize");

	rightPostion_ = globalVariables->GetVector2Value(groupName, "rightPostion");
	rightSize_ = globalVariables->GetVector2Value(groupName, "rightSize");

	stageSelectPostion_ = globalVariables->GetVector2Value(groupName, "stageSelectPostion");
	stageSelectSize_ = globalVariables->GetVector2Value(groupName, "stageSelectSize");

	stageNumberPostion_ = globalVariables->GetVector2Value(groupName, "stageNumberPostion");
	stageNumberSize_ = globalVariables->GetVector2Value(groupName, "stageNumberSize");

	stageUiPostion_ = globalVariables->GetVector2Value(groupName, "stageUiPostion");
	stageUiSize_ = globalVariables->GetVector2Value(groupName, "stageUiSize");

}
