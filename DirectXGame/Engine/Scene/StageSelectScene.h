#pragma once
#include "BaseScene.h"

class StageSelectScene : public BaseScene
{
public:
	StageSelectScene() {};
	~StageSelectScene() {};

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

};

