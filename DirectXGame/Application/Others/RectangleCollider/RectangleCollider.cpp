#include "RectangleCollider.h"

void RectangleCollider::Initialize(Vector2 position, Vector2 size)
{
	// 位置
	position_ = position;

	// サイズ
	size_ = size;

}

void RectangleCollider::Update(Vector2 position)
{

	// 位置
	position_ = position;

}

void RectangleCollider::OnCollision(uint32_t collisonObj)
{

	function_(collisonObj);

}
