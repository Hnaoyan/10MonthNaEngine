#pragma once
#include <memory>
#include "BaseScene.h"
#include "TitleScene.h"
#include "GameScene.h"

class SceneManager {
private:
	std::unique_ptr<BaseScene> sceneArray_[3];

	int sceneNum_;
	int prevSceneNum_;

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
