#include "CloudObject.h"
#include "VectorLib.h"
#include "ImGuiManager.h"

void CloudObject::Initialize(Model* model)
{
	// モデル
	model_ = model;
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//velocity_ = { -1.0f,0,0 };
}

void CloudObject::Update()
{
	ImGui::Begin("wolrd");
	ImGui::DragFloat3("pos", &worldTransform_.translation_.x, 0.1f, -100.0f, 100.0f);
	ImGui::End();
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
