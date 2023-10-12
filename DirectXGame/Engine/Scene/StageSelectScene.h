#pragma once
#include "BaseScene.h"

class StageSelectScene : public BaseScene
{
public:
	StageSelectScene() {};
	~StageSelectScene() {};

	/// <summary>
	/// ‰Šú‰»
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// –ˆƒtƒŒ[ƒ€ˆ—
	/// </summary>
	void Update() override;

	/// <summary>
	/// •`‰æ
	/// </summary>
	void Draw() override;

};

