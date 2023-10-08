#pragma once
#include "WorldTransform.h"
#include "Sprite.h"
#include <memory>

class UIBase
{
public:
	void Initialize(int texHandle);
	void Draw();

public:

	void SetPosition(const Vector2& pos) { position_ = pos; }

	void SetSize(const Vector2& size) { size_ = size; }

	void SetAnchor(const Vector2& anchor) { anchorPoint_ = anchor; }

	void SetColor(const Vector4& color) { color_ = color; }

private:
	std::unique_ptr<Sprite> sprite_;
	uint32_t textureHandle_ = 0u;

	Vector2 position_;
	Vector2 size_;
	Vector2 anchorPoint_;
	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };
};

