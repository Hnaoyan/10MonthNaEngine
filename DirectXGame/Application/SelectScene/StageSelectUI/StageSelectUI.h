#pragma once
#include <cstdint>
#include <StructManager.h>
#include <memory>
#include <Sprite.h>

/// <summary>
/// ステージセレクトUI
/// </summary>
class StageSelectUI
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="textureHandles"></param>
	void Initialize(uint32_t leftTextureHandle, uint32_t rightTextureHandle, uint32_t stageSelectTextureHandle);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 設定
	/// </summary>
	void Setting();

private: // メンバ変数

	// レフト
	
	// テクスチャハンドル
	uint32_t leftTextureHandle_;
	// 位置
	Vector2 leftPostion_;
	// サイズ
	Vector2 leftSize_;
	// スプライト
	std::unique_ptr<Sprite> leftSprite_;

	// ライト
	
	// テクスチャハンドル
	uint32_t rightTextureHandle_;
	// 位置
	Vector2 rightPostion_;
	// サイズ
	Vector2 rightSize_;
	// スプライト
	std::unique_ptr<Sprite> rightSprite_;


	// ステージセレクト

	// テクスチャハンドル
	uint32_t stageSelectTextureHandle_;
	// 位置
	Vector2 stageSelectPostion_;
	// サイズ
	Vector2 stageSelectSize_;
	// スプライト
	std::unique_ptr<Sprite> stageSelectSprite_;

};

