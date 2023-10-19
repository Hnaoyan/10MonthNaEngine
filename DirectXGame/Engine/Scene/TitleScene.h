#pragma once
#include "BaseScene.h"
#include "StructManager.h"
#include "Sprite.h"
#include "DirectXCommon.h"
#include "Audio.h"
#include "Input.h"
#include <memory>

class TitleScene : public BaseScene
{
public:
	TitleScene();
	~TitleScene() {};

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

private:
	DirectXCommon* dxCommon_ = nullptr;
	Audio* audio_ = nullptr;

	// タイトルスプライト
	std::unique_ptr<Sprite> titleSprite_;
	// タイトルテクスチャ
	uint32_t titleTexture_ = 0u;

	Vector2 spritePosition_ = {};

	Vector2 waveVelocity_ = {};

	float waveAnimation_t_ = 0;

};

