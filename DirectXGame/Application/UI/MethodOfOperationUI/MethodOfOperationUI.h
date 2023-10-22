#pragma once
#include <cstdint>
#include <memory>
#include <Sprite.h>
#include <vector>
#include <Input.h>

/// <summary>
/// 操作方法
/// </summary>
class MethodOfOperationUI
{

public: // メンバ関数

	~MethodOfOperationUI();

	void Initialize(std::vector<uint32_t> moveTextureHandles, uint32_t vibrationTextureHandle,
		uint32_t resetTextureHandle, uint32_t stageSelectTextureHandle);

	void Update();

	void Draw();

private: // メンバ変数

	// 移動 WWAASSDD

	// スプライト 4
	std::vector<Sprite*> moveSprites_;
	// テクスチャハンドル 
	std::vector<uint32_t> moveTextureHandles_;
	// 位置
	Vector2 movePosition_;
	// サイズ
	Vector2 moveSize_;

	// 振動

	// スプライト 1
	std::unique_ptr<Sprite> vibrationSprite_;
	// テクスチャハンドル 
	uint32_t vibrationTextureHandle_;
	// 位置
	Vector2 vibrationPosition_;
	// サイズ
	Vector2 vibrationSize_;


	// リセット

	// スプライト 1
	std::unique_ptr<Sprite> resetSprite_;
	// テクスチャハンドル 
	uint32_t resetTextureHandle_;
	// 位置
	Vector2 resetPosition_;
	// サイズ
	Vector2 resetSize_;


	// ステージセレクト

	// スプライト 1
	std::unique_ptr<Sprite> stageSelectSprite_;
	// テクスチャハンドル 
	uint32_t stageSelectTextureHandle_;
	// 位置
	Vector2 stageSelectPosition_;
	// サイズ
	Vector2 stageSelectSize_;


	//入力
	Input* input_ = nullptr;

};

