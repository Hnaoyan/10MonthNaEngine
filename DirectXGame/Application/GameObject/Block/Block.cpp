#include "Block.h"
#include "MathCalc.h"
#include "Application/GameObject/BlockManager/BlockManager.h"
#include <Application/Others/Math2d/Math2d.h>
#include "ImGuiManager.h"

void Block::Initialize(Model* model, uint32_t textureHandle, Vector2 position, MapSystem::MapNumber mapNum)
{

	// ワールドトランスフォーム
	worldTransform_.Initialize();

	// モデル
	model_ = model;

	// テクスチャハンドル
	textureHandle_ = textureHandle;

	// ポジション
	position_ = position;

	// マップ番号
	mapNum_ = mapNum;
	if (mapNum_ == MapSystem::MapNumber::Hole) {
		worldTransform_.translation_ = { position_.x * MapSystem::kSquareSize_.x, position_.y * MapSystem::kSquareSize_.y, 4.0f };
	}
	else {
		worldTransform_.translation_ = { position_.x * MapSystem::kSquareSize_.x, position_.y * MapSystem::kSquareSize_.y, 0.0f };
	}

}

void Block::Update()
{
	worldTransform_.UpdateMatrix();
}

void Block::Draw(const ViewProjection& viewProjection)
{

	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}

void Block::Setting(uint32_t textureHandle, MapSystem::MapNumber mapNum)
{

	worldTransform_.translation_ = { position_.x * MapSystem::kSquareSize_.x, position_.y * MapSystem::kSquareSize_.y, 0.0f };
	worldTransform_.rotation_ = { 0.0f,0.0f,0.0f };
	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransform_.UpdateMatrix();

	// テクスチャハンドル
	textureHandle_ = textureHandle;

	// マップ番号
	mapNum_ = mapNum;

	// イージング用
	// 補間レート
	fall_t_ = 0;
	// フラグ
	isFallNow_ = false;
}

void Block::Fall(uint32_t textureHandle)
{

	// テクスチャハンドル
	textureHandle_ = textureHandle;

	// マップ番号
	mapNum_ = MapSystem::MapNumber::Hole;

	// 落下アニメーションフラグ
	isFallNow_ = true;
	fall_t_ = 0;
}

void Block::FallAnimation(float startPoint, float endPoint)
{
	if (fall_t_ >= 1.0f) {
		fall_t_ = 1.0f;
		isFallNow_ = false;
	}
	else {
		fall_t_ += 1.0f / 20.0f;
	}
	worldTransform_.translation_.z = MathCalc::EaseInCubicF(fall_t_, startPoint, endPoint);
	worldTransform_.UpdateMatrix();
}
