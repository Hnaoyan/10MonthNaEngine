#pragma once
#include <memory>
#include "BaseScene.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "EditorScene.h"

class SceneManager {
private:
	std::unique_ptr<BaseScene> sceneArray_[4];

	int sceneNum_;
	int prevSceneNum_;

public:
	SceneManager();
	~SceneManager();


	void Update();

	void Draw();

};
