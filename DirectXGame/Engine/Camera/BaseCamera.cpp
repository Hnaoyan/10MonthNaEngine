#include "BaseCamera.h"
#include "ImGuiManager.h"

void BaseCamera::Initialize()
{
	// ビューの初期化
	viewProjection_.Initialize();

	// 位置・角度の設定
	initPosition_ = { 70.0f, 70.0f, -200.0f };
	viewProjection_.translate_ = initPosition_;
	viewProjection_.rotation_ = { 0.36f,0,0 };
}

void BaseCamera::Update()
{
#ifdef _DEBUG
	ImGui::Begin("base");
	// 位置
	ImGui::SliderFloat3("translate", &initPosition_.x, -50.0f, 50.0f);
	// 回転
	ImGui::DragFloat3("rotate", &viewProjection_.rotation_.x, 0.01f, -2.0f, 2.0f);
	// 視野角
	ImGui::SliderFloat("fov", &fov_, 5.0f, 100.0f);
	ImGui::End();
#endif // _DEBUG

	// データの更新と転送
	viewProjection_.SetFov(fov_);
	viewProjection_.UpdateMatrix();
}

void BaseCamera::ResetPosition()
{
	viewProjection_.translate_ = initPosition_;
}
