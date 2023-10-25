#include "SceneManager.h"
#include "StageSelectScene.h"
#include "ImGuiManager.h"

SceneManager::SceneManager() 
{ 
	audio_ = Audio::GetInstance();
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

	// 遷移マネージャーの設定
	sceneArray_[TITLE]->SetTransitionManager(transitionManager_.get());
	sceneArray_[GAMESCENE]->SetTransitionManager(transitionManager_.get());
	sceneArray_[CLEAR]->SetTransitionManager(transitionManager_.get());
	sceneArray_[STAGESELECT]->SetTransitionManager(transitionManager_.get());

	sceneNum_ = TITLE;
	prevSceneNum_ = sceneNum_;

	titleBGMHandle_ = audio_->LoadWave("BGM/Title.wav");
	playBGMHandle_ = audio_->LoadWave("BGM/Play.wav");

}

SceneManager::~SceneManager() {}

void SceneManager::Initialize()
{

}

void SceneManager::Update() 
{ 

#ifdef _DEBUG

	ImGui::Begin("sceneNum");
	ImGui::Text("NowNum : %d prevNum : %d", sceneNum_, prevSceneNum_);
	ImGui::End();
	

#endif // DEBUG

	if (!audio_->IsPlaying(voiceHandle_) && sceneNum_!= GAMESCENE) {
		audio_->StopWave(playHandle_);
		audio_->StopWave(voiceHandle_);
		voiceHandle_ = audio_->PlayWave(titleBGMHandle_, true, 0.4f);
	}
	if (!audio_->IsPlaying(playHandle_) && sceneNum_ == GAMESCENE) {
		audio_->StopWave(voiceHandle_);
		audio_->StopWave(playHandle_);
		playHandle_ = audio_->PlayWave(playBGMHandle_, true, 0.4f);
	}

	// 遷移の設定呼び出し
	if (prevSceneNum_ != sceneArray_[sceneNum_]->GetSceneNum()) {
		transitionManager_->SetIsTransition(true);
	}
	// 連打された時用
	if (!transitionManager_->IsGetSceneChanger()) {
		prevSceneNum_ = sceneArray_[sceneNum_]->GetSceneNum();
	}
	// シーン切り替えタイミング
	if (transitionManager_->IsGetSceneChanger()) {
		sceneNum_ = sceneArray_[sceneNum_]->GetSceneNum();
		sceneArray_[sceneNum_]->Setting(static_cast<Scene>(prevSceneNum_));
		audio_->StopWave(playHandle_);
		audio_->StopWave(voiceHandle_);
	}

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
