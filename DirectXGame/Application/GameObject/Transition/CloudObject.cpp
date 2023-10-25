﻿#include "CloudObject.h"
#include "VectorLib.h"
#include "ImGuiManager.h"

void CloudObject::Initialize(Model* model)
{
	// モデル
	model_ = model;
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.rotation_ = { 0.0f,1.57f,0 };
}

void CloudObject::Update()
{
	// 移動処理
	worldTransform_.translation_ = VectorLib::Add(worldTransform_.translation_, velocity_);
	// 更新
	worldTransform_.UpdateMatrix();
}

void CloudObject::Draw(ViewProjection& view)
{
	// 描画
	model_->Draw(worldTransform_, view);

}
