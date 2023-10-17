#pragma once
#include "BaseScene.h"
#include "Sprite.h"
#include <vector>
#include <Input.h>
#include "DirectXCommon.h"
#include "Model.h"
#include <Audio.h>

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

private: // メンバ関数

	void MoveRight();

	void MoveLeft();

private: // メンバ変数 (ステージ写真)

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// テクスチャハンドル
	std::vector<uint32_t> textureHandles_;

	// スプライト5つ
	std::unique_ptr<Sprite> sprite_[5];
	std::unique_ptr<Sprite> sprite2_;
	std::unique_ptr<Sprite> sprite3_;
	std::unique_ptr<Sprite> sprite4_;
	std::unique_ptr<Sprite> sprite5_;

	// スプライト固定座標
	Vector2 positions_[5];

	// サイズ
	Vector2 sizes_[5];

	// 動いているか
	bool isMoveRight_;
	bool isMoveLeft_;

	// 
	float easeTimer;
	float easeSpeed;

};

