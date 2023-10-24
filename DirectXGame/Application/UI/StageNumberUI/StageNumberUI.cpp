#include "StageNumberUI.h"
#include <GlobalVariables.h>

void StageNumberUI::Initialize(uint32_t texture)
{
	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };
	Vector2 anchorPoint = { 0,0 };
	position_ = { 1000.0f,10.0f };
	size_ = { 300.0f,128.0f };

#pragma region 調整項目クラス
	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名設定
	const char* groupName = "StageNumberUI";
	// 指定した名前でグループ追加
	globalVariables->CreateGroup(groupName);

	// メンバ変数の調整したい項目をグローバル変数に追加

	globalVariables->AddItem(groupName, "position", position_);
	globalVariables->AddItem(groupName, "size", size_);

	ApplyGlobalVariables();

#pragma endregion

	textureHandle_ = texture;
	stageNumberSprite_.reset(Sprite::Create(textureHandle_, position_, color, anchorPoint, false, false));
	stageNumberSprite_->SetSize({ size_.x - 50.0f,size_.y });
	stageNumberSprite_->SetSpriteRect(Vector2{ 0,0 }, size_);
	stageNumberSprite_->SetBlendMode(Sprite::BlendMode::kNormal);
	stageNumberSprite_->Update();
}

void StageNumberUI::Update()
{

#ifdef _DEBUG
	ApplyGlobalVariables();
	stageNumberSprite_->SetPosition(position_);
	stageNumberSprite_->SetSize({ size_.x - 50.0f,size_.y });
	//stageNumberSprite_->SetSpriteRect(Vector2{ 0,0 }, size_);
	stageNumberSprite_->Update();

#endif // _DEBUG

}

void StageNumberUI::Setting(int stageNum)
{
	// 現在のステージ番号取得
	stageNumberSprite_->SetSpriteRect(Vector2{ size_.x * (float)stageNum, 0.0f }, size_);
	stageNumberSprite_->Update();

}

void StageNumberUI::Draw()
{
	stageNumberSprite_->Draw();
}

void StageNumberUI::ApplyGlobalVariables()
{

	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "StageNumberUI";

	position_ = globalVariables->GetVector2Value(groupName, "position");
	size_ = globalVariables->GetVector2Value(groupName, "size");

}
