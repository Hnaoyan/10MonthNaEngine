﻿#pragma once
#include <memory>
#include "DirectXCommon.h"
#include "Sprite.h"
#include "BaseScene.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "EditorScene.h"
#include "Transition/TransitionManager.h"
#include "BackGroundManager.h"
#include "Audio.h"
#include "AudioManager.h"

class SceneManager {
private:
	std::unique_ptr<BaseScene> sceneArray_[5];
	std::unique_ptr<TransitionManager> transitionManager_;
	std::unique_ptr<BackGroundManager> backGroundManager_;

	std::unique_ptr<Model> cloudModel_;

	int sceneNum_;
	int prevSceneNum_;

	int changeNum_;

	uint32_t titleBGMHandle_ = 0u;
	uint32_t playBGMHandle_ = 0u;

	uint32_t voiceHandle_ = 0u;
	uint32_t playHandle_ = 0u;

	float BGMVolume_ = 0;

public:
	SceneManager();
	~SceneManager();

	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// シーンの読み込み
	/// </summary>
	/// <param name="number"></param>
	void LoadScene(int number);

private:
	Audio* audio_;

};
