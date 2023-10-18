#pragma once
#include <memory>
#include "DirectXCommon.h"
#include "Sprite.h"
#include "BaseScene.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "EditorScene.h"
#include "Transition/TransitionManager.h"

class SceneManager {
private:
	std::unique_ptr<BaseScene> sceneArray_[4];
	std::unique_ptr<TransitionManager> transitionManager_;

	int sceneNum_;
	int prevSceneNum_;

	int changeNum_;

public:
	SceneManager();
	~SceneManager();

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
};
