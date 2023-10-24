#include "SceneManager.h"
#include "StageSelectScene.h"

SceneManager::SceneManager() 
{ 
	cloudModel_.reset(Model::CreateFromObj("cloud", true));
	transitionManager_ = std::make_unique<TransitionManager>();
	transitionManager_->Initialize(cloudModel_.get());
	backGroundManager_ = std::make_unique<BackGroundManager>();
	backGroundManager_->Initialize(cloudModel_.get());

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

	prevSceneNum_ = this->sceneNum_;
	sceneNum_ = sceneArray_[sceneNum_]->GetSceneNum();

	// 遷移の設定呼び出し
	if (prevSceneNum_ != sceneArray_[sceneNum_]->GetSceneNum()) {
		sceneArray_[sceneNum_]->Setting(static_cast<Scene>(prevSceneNum_));
		//transitionManager_->SetIsTransition(true);
	}
	// 連打された時用
	//if (!transitionManager_->IsGetSceneChanger()) {
	//	prevSceneNum_ = sceneArray_[sceneNum_]->GetSceneNum();
	//}
	//// シーン切り替えタイミング
	//if (transitionManager_->IsGetSceneChanger()) {
	//	sceneNum_ = sceneArray_[sceneNum_]->GetSceneNum();
	//	sceneArray_[sceneNum_]->Setting(static_cast<Scene>(prevSceneNum_));
	//}

	// 遷移の更新
	transitionManager_->Update();

	// 背景の更新
	if (sceneNum_ != GAMESCENE) {
		backGroundManager_->Update();
	}
	// シーンごとの更新
	sceneArray_[sceneNum_]->Update();

}

void SceneManager::Draw() 
{
	// 背景モデルの描画
	if (sceneNum_ != GAMESCENE) {
		backGroundManager_->Draw();
	}
	// シーン描画
	sceneArray_[sceneNum_]->Draw(); 

	// 遷移描画
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
