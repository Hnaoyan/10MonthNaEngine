#include "SceneManager.h"
#include "StageSelectScene.h"

SceneManager::SceneManager() 
{ 
	transitionManager_ = std::make_unique<TransitionManager>();
	transitionManager_->Initialize();

	sceneArray_[TITLE] = std::make_unique<TitleScene>();
	sceneArray_[TITLE]->Initialize();
	sceneArray_[GAMESCENE] = std::make_unique<GameScene>();
	sceneArray_[GAMESCENE]->Initialize();
	sceneArray_[CLEAR] = std::make_unique<TitleScene>();
	sceneArray_[CLEAR]->Initialize();
	sceneArray_[EDITOR] = std::make_unique<EditorScene>();
	sceneArray_[EDITOR]->Initialize();
	sceneArray_[STAGESELECT] = std::make_unique<StageSelectScene>();
	sceneArray_[STAGESELECT]->Initialize();

	sceneNum_ = TITLE;
	prevSceneNum_ = sceneNum_;
	//sceneArray_[sceneNum_]->Initialize();
}

SceneManager::~SceneManager() {}

void SceneManager::Update() 
{ 

#ifdef _DEBUG

	//if (Input::GetInstance()->TriggerKey(DIK_1)) {
	//	if (sceneNum_ == GAMESCENE) {
	//		sceneArray_[sceneNum_]->SetSceneNum(EDITOR);
	//	}
	//	else if (sceneNum_ == EDITOR) {
	//		sceneArray_[sceneNum_]->SetSceneNum(GAMESCENE);
	//	}
	//}

#endif // DEBUG

	//prevSceneNum_ = this->sceneNum_;
	//sceneNum_ = sceneArray_[sceneNum_]->GetSceneNum();

	if (prevSceneNum_ != sceneArray_[sceneNum_]->GetSceneNum()) {
		//LoadScene(sceneNum_);
		//sceneArray_[sceneNum_]->Initialize();
		//sceneArray_[prevSceneNum_].release();
		//sceneArray_[sceneNum_]->Initialize();
		//sceneArray_[sceneNum_]->Setting(static_cast<Scene>(prevSceneNum_));
		transitionManager_->SetIsTransition(true);
	}
	prevSceneNum_ = sceneArray_[sceneNum_]->GetSceneNum();

	if (transitionManager_->IsGetSceneChanger()) {
		//sceneArray_[sceneNum_]->SetSceneNum(GAMESCENE);
		sceneNum_ = sceneArray_[sceneNum_]->GetSceneNum();
		sceneArray_[sceneNum_]->Setting(static_cast<Scene>(prevSceneNum_));
	}

	// �J�ڍX�V
	transitionManager_->Update();

	// �V�[���X�V
	sceneArray_[sceneNum_]->Update();

}

void SceneManager::Draw() 
{
	// �V�[���`��
	sceneArray_[sceneNum_]->Draw(); 

	// �J�ڕ`��
	transitionManager_->Draw();

}

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
