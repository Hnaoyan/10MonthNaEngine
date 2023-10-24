#include "BaseCamera.h"
#include "ImGuiManager.h"
#include <Math/MathCalc.h>

void BaseCamera::Initialize()
{
	// ビューの初期化
	viewProjection_.Initialize();

	// 位置・角度の設定
	initPosition_ = { 73.0f, -35.9f, -158.0f };
	initRotate_ = { -0.550f, 0.0f, 0.0f};
	viewProjection_.translate_ = initPosition_;
	viewProjection_.rotation_ = initRotate_;
	fov_ = 45.0f;

	// オープニングスタート位置
	openingStartPostion_ = { initPosition_.x, initPosition_.y - 100.0f, 0.0f };
	// オープニングスタート角度
	openingStartRotate_ = { -1.57f, 0.0f, 0.0f };
	// フレーム数
	openingFrame_ = 90;
	OpeningAnimationInitialize();

	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名設定
	const char* groupName = "Camera";
	// 指定した名前でグループ追加
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "position", initPosition_);
	globalVariables->AddItem(groupName, "rotate", initRotate_);
	globalVariables->AddItem(groupName, "scale", viewProjection_.scale_);
	globalVariables->AddItem(groupName, "Fov", fov_);

	globalVariables->AddItem(groupName, "openingStartPosition", openingStartPostion_);
	globalVariables->AddItem(groupName, "openingStartRotate", openingStartRotate_);
	globalVariables->AddItem(groupName, "openingFrame", static_cast<int>(openingFrame_));

	ApplyGlobalVariables();

}

void BaseCamera::Update()
{
//#ifdef _DEBUG
//	ImGui::Begin("base");
//	// 位置
//	ImGui::SliderFloat3("translate", &initPosition_.x, -50.0f, 50.0f);
//	// 回転
//	ImGui::DragFloat3("rotate", &viewProjection_.rotation_.x, 0.01f, -2.0f, 2.0f);
//	// 視野角
//	ImGui::SliderFloat("fov", &fov_, 5.0f, 100.0f);
//	ImGui::End();
//#endif // _DEBUG

	ApplyGlobalVariables();

	// データの更新と転送
	viewProjection_.SetFov(fov_);
	viewProjection_.UpdateMatrix();
}

void BaseCamera::ApplyGlobalVariables()
{	
	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名設定
	const char* groupName = "Camera";

	// 取得
	initPosition_ = globalVariables->GetVector3Value(groupName, "position");
	initRotate_ = globalVariables->GetVector3Value(groupName, "rotate");
	viewProjection_.scale_ = globalVariables->GetVector3Value(groupName, "scale");
	fov_ = globalVariables->GetFloatValue(groupName, "Fov");

	openingStartPostion_ = globalVariables->GetVector3Value(groupName, "openingStartPosition");
	openingStartRotate_ = globalVariables->GetVector3Value(groupName, "openingStartRotate");
	openingFrame_ = static_cast<uint32_t>(globalVariables->GetIntValue(groupName, "openingFrame"));

}

void BaseCamera::OpeningAnimationInitialize()
{

	// t
	openingT_ = 0.0f;

}

void BaseCamera::OpeningAnimationUpdate()
{
	
	//t計算
	openingT_ += 1.0f / static_cast<float>(openingFrame_);
	if (openingT_ > 1.0f) {
		openingT_ = 1.0f;
	}

	viewProjection_.translate_ = MathCalc::EaseInCubicF(openingT_, openingStartPostion_, initPosition_);
	viewProjection_.rotation_ = MathCalc::EaseInCubicF(openingT_, openingStartRotate_, initRotate_);

	// ビュープロジェクション更新
	viewProjection_.UpdateMatrix();

}

void BaseCamera::ResetPosition()
{
	viewProjection_.translate_ = initPosition_;
}
