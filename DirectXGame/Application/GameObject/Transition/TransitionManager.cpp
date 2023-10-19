#include "TransitionManager.h"
#include "ImGuiManager.h"
#include "Sprite.h"
#include "Input.h"

TransitionManager::TransitionManager()
{
	cloudModel_.reset(Model::CreateFromObj("Cloud", true));
}

TransitionManager* TransitionManager::GetInstance()
{
	static TransitionManager instance;
	return &instance;
}

void TransitionManager::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	viewProjection_.Initialize();

	cloudModel_->SetAlphaValue(1.0f);

	transitionCamera_ = std::make_unique<BaseCamera>();
	transitionCamera_->Initialize();

	transitionCamera_->SetPosition({ 0, 5.0f, 0.0f });
}

void TransitionManager::Update()
{
	ImGui::Begin("camera");

	ImGui::End();

	if (Input::GetInstance()->TriggerKey(DIK_U)) {
		TransitionSetting();
	}

	TransitionUpdate();

	transitionCamera_->Update();

	viewProjection_.matView = transitionCamera_->GetView().matView;
	viewProjection_.matProjection = transitionCamera_->GetView().matProjection;
	viewProjection_.UpdateMatrix();

}

void TransitionManager::Draw()
{	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	for (TransitionObject* object : objects_) {
		object->Draw(viewProjection_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void TransitionManager::TransitionSetting()
{
	endTimer_ = 600;
	transitionTimer_ = 0;
	isNowTransition_ = true;
	isSceneChanger_ = false;
	CloudTransition();

}

void TransitionManager::TransitionUpdate()
{
	if (isNowTransition_) {
		if (transitionTimer_ >= 1.0f) {
			transitionTimer_ = 0;
			isNowTransition_ = false;
			objects_.clear();
		}
		else {
			transitionTimer_ += 0.005f;
		}
		if (transitionTimer_ >= 0.75f && transitionTimer_ <= 0.76f) {
			isSceneChanger_ = true;
		}
		else {
			isSceneChanger_ = false;
		}
	}
	for (TransitionObject* object : objects_) {
		object->Update(transitionTimer_);
	}

	ImGui::Begin("time");
	ImGui::DragFloat("timer", &transitionTimer_, 0, -1.0f, 1.0f);
	ImGui::End();

}

void TransitionManager::CloudTransition()
{
	Vector3 scale = {};
	//---1Wave---//
	scale = { 3.0f,3.0f,3.0f };
	AddObject(Vector3(50.0f, -7.0f, 0), scale);
	AddObject(Vector3(55.0f, -3.0f, 0), scale);
	AddObject(Vector3(55.0f, -10.0f, 0), scale);

	//---2Wave---//
	scale = { 4.5f,4.5f,4.5f };
	AddObject(Vector3(65.0f, -18.5f, 1.0f), scale);
	AddObject(Vector3(75.0f, 4.0f, 0), scale);
	AddObject(Vector3(90.0f, -8.0f, 0), scale);

	//---3Wave---//
	scale = { 6,6,6 };
	AddObject(Vector3(100.0f, 3.0f, 0), scale);
	AddObject(Vector3(105.0f, -20.0f, 0), scale);
	AddObject(Vector3(120.0f, -8.0f, 0), scale);

	//---4Wave---//
	scale = { 8,8,8 };
	AddObject(Vector3(150.0f, 4.0f, 0), scale);
	AddObject(Vector3(165.0f, -3.0f, 1.0f), scale);
	AddObject(Vector3(165.0f, -18.5f, -1.0f), scale);

	//---5Wave---//
	scale = { 4,4,4 };
	AddObject(Vector3(175.0f, 4.0f, 2.0f), scale);
	AddObject(Vector3(175.0f, -10.5f, -7.0f), scale);
	AddObject(Vector3(185.0f, 0.5f, 5.0f), scale);
}

void TransitionManager::AddObject(const Vector3& position, const Vector3& scale)
{
	TransitionObject* newObject = new TransitionObject();
	newObject->Initialize(cloudModel_.get());
	newObject->InstanceSetting(position, scale);
	//newObject->SetVelocity(Vector3(-1.0f, 0, 0));
	objects_.push_back(newObject);
}
