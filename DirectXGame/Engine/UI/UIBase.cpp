#include "UIBase.h"

void UIBase::Initialize(int texHandle)
{
	textureHandle_ = texHandle;
	sprite_.reset(Sprite::Create(textureHandle_, position_, color_, anchorPoint_, false, false));
	size_ = sprite_->GetSize();
	sprite_->SetSpriteRect(Vector2(100.0f, 0), Vector2(300.0f, 512.0f));
}

void UIBase::Update()
{
	sprite_->SetSize(size_);
	sprite_->Update();
}

void UIBase::Draw()
{
	// 描画
	sprite_->Draw();
}
