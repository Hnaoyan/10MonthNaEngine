#pragma once
#include <Sprite.h>
#include <memory>
#include <MathCalc.h>

class StageNumberUI
{
public:

	void Initialize(uint32_t texture);

	void Setting(int stageNum);

	void Update();

	void Draw();

	//void 

private:
	std::unique_ptr<Sprite> stageNumberSprite_;

	uint32_t textureHandle_ = 0u;

	Vector2 position_ = {};

	Vector2 size_ = {};

};

