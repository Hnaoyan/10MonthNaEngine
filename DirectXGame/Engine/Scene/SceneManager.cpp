#include "SceneManager.h"

SceneManager::SceneManager() 
{ 
	sceneArray_[TITLE] = std::make_unique<TitleScene>();
	sceneArray_[TITLE]->Initialize();
	sceneArray_[GAMESCENE] = std::make_unique<GameScene>();
	sceneArray_[GAMESCENE]->Initialize();
	sceneArray_[CLEAR] = std::make_unique<TitleScene>();
	sceneArray_[CLEAR]->Initialize();
	sceneArray_[EDITOR] = std::make_unique<EditorScene>();
	sceneArray_[EDITOR]->Initialize();

	sceneNum_ = GAMESCENE;
	//sceneArray_[sceneNum_]->Initialize();
}

SceneManager::~SceneManager() {}

void SceneManager::Update() 
{ 

#ifdef _DEBUG

	if (Input::GetInstance()->TriggerKey(DIK_1)) {
		if (sceneNum_ == GAMESCENE) {
			sceneArray_[sceneNum_]->SetSceneNum(EDITOR);
		}
		else if (sceneNum_ == EDITOR) {
			sceneArray_[sceneNum_]->SetSceneNum(GAMESCENE);
		}
	}

#endif // DEBUG

	prevSceneNum_ = this->sceneNum_;
	sceneNum_ = sceneArray_[sceneNum_]->GetSceneNum();

	if (prevSceneNum_ != sceneNum_) {
		//LoadScene(sceneNum_);
		//sceneArray_[sceneNum_]->Initialize();
		//sceneArray_[prevSceneNum_].release();
		//sceneArray_[sceneNum_]->Initialize();
		sceneArray_[sceneNum_]->Setting(static_cast<Scene>(prevSceneNum_));
	}

	sceneArray_[sceneNum_]->Update();

}

void SceneManager::Draw() 
{ sceneArray_[sceneNum_]->Draw(); }

void SceneManager::LoadScene(int number)
{
	switch (number)
	{
	case TITLE:
		sceneArray_[number] = std::make_unique<TitleScene>();
		break;
	case GAMESCENE:
		sceneArray_[number] = std::make_unique<GameScene>();
		break;
	}
}
