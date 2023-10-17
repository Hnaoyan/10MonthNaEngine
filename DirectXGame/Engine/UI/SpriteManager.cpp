#include "SpriteManager.h"

SpriteManager* SpriteManager::GetInstance()
{
	static SpriteManager instance;
	return &instance;
}

void SpriteManager::Update()
{
	for (UIBase* ui : uiSprites_) {
		ui->Update();
	}
}

void SpriteManager::Draw()
{
	for (UIBase* ui : uiSprites_) {
		ui->Draw();
	}
}

void SpriteManager::AddUI(int texHandle, const Vector2& position, const Vector2& anchor, const std::string& name)
{
	UIBase* newUi = new UIBase(name);
	newUi->SetPosition(position);
	newUi->SetAnchor(anchor);
	newUi->Initialize(texHandle);
	uiSprites_.push_back(newUi);
}

void SpriteManager::DeleteUI(const std::string& name)
{
	for (auto it = uiSprites_.begin(); it != uiSprites_.end(); ++it) {
		if ((*it)->GetTagName() == name) {
			delete *it;
			it = uiSprites_.erase(it);
		}
	}
}

UIBase* SpriteManager::GetUI(const std::string& name)
{
	for (auto ui : uiSprites_) {
		if (ui->GetTagName() == name) {
			return ui;
		}
	}
	return nullptr;
}
