#include "TransitionManager.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include "Sprite.h"
#include "Input.h"
#include "VectorLib.h"

#include <algorithm>

TransitionManager::TransitionManager()
{}

TransitionManager* TransitionManager::GetInstance()
{
	static TransitionManager instance;
	return &instance;
}

void TransitionManager::Initialize(Model* model)
{
	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	viewProjection_.Initialize();

	cloudModel_ = model;
	cloudModel_->SetAlphaValue(1.0f);

	transitionCamera_ = std::make_unique<BaseCamera>();
	transitionCamera_->Initialize();

	transitionCamera_->SetPosition({ 0, 5.0f, 0.0f });

	changeSEHandle_ = audio_->LoadWave("SE/sceanChange.wav");
	SEVolume_ = 0.3f;

}

void TransitionManager::Update()
{
	if (isSettingTransition_ && !isNowTransition_) {
		TransitionSetting();
	}

	// 更新
	TransitionUpdate();

	// カメラ更新
	transitionCamera_->Update();

	// 遷移用のカメラ
	viewProjection_.matView = transitionCamera_->GetView().matView;
	viewProjection_.matProjection = transitionCamera_->GetView().matProjection;
	viewProjection_.UpdateMatrix();

}

void TransitionManager::Draw()
{
	// コマンドリストの取得
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
	// SE
	audio_->PlayWave(changeSEHandle_, false, SEVolume_);
}

void TransitionManager::TransitionUpdate()
{
	if (isNowTransition_) {
		if (transitionTimer_ >= 1.0f) {
			transitionTimer_ = 0;
			isNowTransition_ = false;
			isSettingTransition_ = false;
			objects_.clear();
		}
		else {
			float addTimer = 1.0f / 135.0f;
			transitionTimer_ += addTimer;
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

}

void TransitionManager::CloudTransition()
{
	float midPosition_Y = -6.0f;
	float distance = 0;

	Vector3 scale = {};
	Vector3 intervalObjectScale = {};
	//---1Wave---//
	scale = { 6.0f,6.0f,6.0f };
	distance = 11.5f;
	AddCloud(Vector3(55.0f, midPosition_Y + (distance), 0), scale);
	AddCloud(Vector3(50.0f, midPosition_Y + (distance / 2), 0), scale);
	AddCloud(Vector3(45.0f, midPosition_Y, 0), scale);
	AddCloud(Vector3(50.0f, midPosition_Y - (distance / 2), 0), scale);
	AddCloud(Vector3(55.0f, midPosition_Y - (distance), 0), scale);

	//---2Wave---//
	scale = { 6.5f,6.5f,6.5f };
	intervalObjectScale = VectorLib::Scaler(scale, 0.75f);
	distance = 15.5f;
	AddCloud(Vector3(75.0f, midPosition_Y + distance, 1.0f), scale);
	AddCloud(Vector3(60.0f, midPosition_Y + (distance / 2), 1.0f), intervalObjectScale);
	AddCloud(Vector3(65.0f, midPosition_Y, 0), scale);
	AddCloud(Vector3(60.0f, midPosition_Y - (distance / 2), 0), intervalObjectScale);
	AddCloud(Vector3(75.0f, midPosition_Y - distance, 0), scale);

	//---3Wave---//
	scale = { 8.5f,8.5f,8.5f };
	intervalObjectScale = VectorLib::Scaler(scale, 0.75f);
	distance = 15.0f;
	AddCloud(Vector3(95.0f, midPosition_Y + distance, 0), scale);
	AddCloud(Vector3(80.0f, midPosition_Y + (distance / 2), 0), intervalObjectScale);
	AddCloud(Vector3(85.0f, midPosition_Y, -1.5f), scale);
	AddCloud(Vector3(80.0f, midPosition_Y - (distance / 2), 0), intervalObjectScale);
	AddCloud(Vector3(95.0f, midPosition_Y - distance, 0), scale);

	//---4Wave---//
	scale = { 6.0f,6.0f,6.0f };
	intervalObjectScale = VectorLib::Scaler(scale, 0.75f);
	distance = 15.5f;
	AddCloud(Vector3(125.0f, midPosition_Y + distance, -0.0f), scale);
	AddCloud(Vector3(110.0f, midPosition_Y + (distance / 2), -0.0f), intervalObjectScale);
	AddCloud(Vector3(125.0f, midPosition_Y, 0.0f), scale);
	AddCloud(Vector3(110.0f, midPosition_Y - (distance / 2), -0.0f), intervalObjectScale);
	AddCloud(Vector3(125.0f, midPosition_Y - distance, -0.0f), scale);

	//---5Wave---//
	scale = { 6.0f,6.0f,6.0f };
	intervalObjectScale = VectorLib::Scaler(scale, 0.75f);
	distance = 11.5f;
	AddCloud(Vector3(155.0f, midPosition_Y + distance, 0.0f), scale);
	AddCloud(Vector3(165.0f, midPosition_Y + (distance / 2), 0.0f), intervalObjectScale);
	AddCloud(Vector3(175.0f, midPosition_Y, 0.0f), scale);
	AddCloud(Vector3(165.0f, midPosition_Y - (distance / 2), 0.0f), intervalObjectScale);
	AddCloud(Vector3(155.0f, midPosition_Y - distance, 0.0f), scale);

}

void TransitionManager::AddCloud(const Vector3& position, const Vector3& scale)
{
	TransitionObject* newObject = new TransitionObject();
	newObject->Initialize(cloudModel_);
	newObject->InstanceSetting(position, scale);
	objects_.push_back(newObject);
}
