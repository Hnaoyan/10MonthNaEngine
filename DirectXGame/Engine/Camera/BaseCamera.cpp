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
	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名設定
	const char* groupName = "Camera";
	// 指定した名前でグループ追加
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "position", initPosition_);
	globalVariables->AddItem(groupName, "rotate", viewProjection_.rotation_);
	globalVariables->AddItem(groupName, "scale", viewProjection_.scale_);
	globalVariables->AddItem(groupName, "Fov", fov_);
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
	viewProjection_.rotation_ = globalVariables->GetVector3Value(groupName, "rotate");
	viewProjection_.scale_ = globalVariables->GetVector3Value(groupName, "scale");
	fov_ = globalVariables->GetFloatValue(groupName, "Fov");
}

void BaseCamera::ResetPosition()
{
	viewProjection_.translate_ = initPosition_;
}
