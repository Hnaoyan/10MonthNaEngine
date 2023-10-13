#pragma once
#include "WorldTransform.h"
#include "Sprite.h"
#include <memory>

class UIBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="name"></param>
	UIBase(const std::string& name) : tagName_(name) {};

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="texHandle"></param>
	void Initialize(int texHandle);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

public:
	/// <summary>
	/// 座標設定
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(const Vector2& pos) { position_ = pos; }

	/// <summary>
	/// サイズ設定
	/// </summary>
	/// <param name="size"></param>
	void SetSize(const Vector2& size) { size_ = size; }

	/// <summary>
	/// アンカーポイント設定
	/// </summary>
	/// <param name="anchor"></param>
	void SetAnchor(const Vector2& anchor) { anchorPoint_ = anchor; }
	
	/// <summary>
	/// 色設定
	/// </summary>
	/// <param name="color"></param>
	void SetColor(const Vector4& color) { color_ = color; }

	/// <summary>
	/// タグ受け取り
	/// </summary>
	/// <returns></returns>
	std::string GetTagName() { return tagName_; }

	/// <summary>
	/// サイズ取得
	/// </summary>
	/// <returns></returns>
	Vector2 GetSize() { return sprite_->GetSize(); }

private:
	std::unique_ptr<Sprite> sprite_;
	uint32_t textureHandle_ = 0u;
	// 座標
	Vector2 position_{};
	// サイズ
	Vector2 size_{};
	// アンカーポイント
	Vector2 anchorPoint_{};
	// 色
	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };
	// タグ
	std::string tagName_{};
};

