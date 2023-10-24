#pragma once
#include <vector>
#include <StructManager.h>
#include <Sprite.h>

/// <summary>
/// ステージ写真
/// </summary>
class StagePhot
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="textureHandles"></param>
	/// <param name="stageMax"></param>
	void Initialize(std::vector<uint32_t> textureHandles, size_t stageMax);

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
	void Setting(size_t stageNum);

	/// <summary>
	/// 右移動
	/// </summary>
	/// <param name="t"></param>
	void MoveRight(float t);

	/// <summary>
	/// 左移動
	/// </summary>
	/// <param name="t"></param>
	void MoveLeft(float t);

	/// <summary>
	/// テクスチャ変更
	/// </summary>
	void TextureHandleChange();

private: //メンバ関数

	/// <summary>
	/// 調整項目適用関数
	/// </summary>
	void ApplyGlobalVariables();

private:

	// テクスチャハンドル
	std::vector<uint32_t> textureHandles_;

	// スプライト5つ
	std::unique_ptr<Sprite> sprite_[5];

	// スプライト固定座標
	Vector2 positions_[5];

	// サイズ
	Vector2 sizes_[5];

	// ステージナンバー
	size_t stageMax_;
	size_t stageNum_;

	// 動きの変数
	float waveAnimation_t_;
	Vector2 waveVelocity_ = {};

	// 振幅
	float amplitude_ = 0.3f;
	// フレーム
	float fre_ = 5.0f;

};

