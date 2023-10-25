#pragma once
#include "BaseScene.h"
#include "Sprite.h"
#include <vector>
#include <Input.h>
#include "DirectXCommon.h"
#include "Model.h"
#include <Audio.h>

#include "Application/SelectScene/StagePhot/StagePhot.h"
#include "Application/SelectScene/StageSelectUI/StageSelectUI.h"

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

	/// <summary>
	/// 
	/// </summary>
	void Setting(Scene preScene) override;

private: // メンバ関数

	void MoveRight();

	void MoveLeft();

private: // メンバ変数 (ステージ写真)

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// テクスチャハンドル
	std::vector<uint32_t> stagePhotTextureHandles_;

	// テクスチャハンドル
	uint32_t leftTextureHandle_;
	uint32_t rightTextureHandle_;
	uint32_t stageSelectTextureHandle_;
	uint32_t stageNumberTextureHandle_;
	uint32_t selectUITextureHandle_;
	uint32_t clearTextureHandle_;

	// 音源ハンドル

	uint32_t slideSEHandle_;
	uint32_t selectSEHandle_;
	uint32_t dropSEHandle_;

	float SEVolume_;

	// 動いているか
	bool isMoveRight_;
	bool isMoveLeft_;

	// イージング
	float easeTimer_;
	float easeSpeed_;

	//オブジェクト
	// ステージ写真
	std::unique_ptr<StagePhot> stagePhot_ = nullptr;
	// UI
	std::unique_ptr<StageSelectUI> stageSelectUI_ = nullptr;

};

