#pragma once
#include <Sprite.h>
#include <memory>
#include <MathCalc.h>

class StageNumberUI
{
public:

	void Initialize(uint32_t texture);

	void Update();

	void Setting(int stageNum);

	void Draw();

private: //メンバ関数

	/// <summary>
	/// 調整項目適用関数
	/// </summary>
	void ApplyGlobalVariables();

private:
	std::unique_ptr<Sprite> stageNumberSprite_;

	uint32_t textureHandle_ = 0u;

	Vector2 position_ = {};

	Vector2 size_ = {};

};

