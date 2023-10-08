#include "UIManager.h"

UIManager* UIManager::GetInstance()
{
	static UIManager instance;
	return &instance;
}

void UIManager::Draw()
{
	for (UIBase* ui : uiSprites_) {
		ui->Draw();
	}
}

void UIManager::AddUI(int texHandle, const Vector2& position, const Vector2& anchor)
{
	UIBase* newUi = new UIBase();
	newUi->SetPosition(position);
	newUi->SetAnchor(anchor);
	newUi->Initialize(texHandle);
	uiSprites_.push_back(newUi);
}
