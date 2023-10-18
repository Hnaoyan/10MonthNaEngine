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

	transitionCamera_ = std::make_unique<BaseCamera>();
	transitionCamera_->Initialize();

	transitionCamera_->SetPosition({ 0, 5.0f, 0.0f });
}

void TransitionManager::Update()
{
	ImGui::Begin("camera");

	ImGui::End();

	if (Input::GetInstance()->TriggerKey(DIK_U)) {
		CloudTransition();
	}
	for (TransitionObject* object : objects_) {
		object->Update();
	}

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
	isNowTransition_ = false;

}

void TransitionManager::TransitionUpdate()
{
}

void TransitionManager::CloudTransition()
{
	AddObject(Vector3(10.0f, 4.0f, 0));
	AddObject(Vector3(10.0f, -12.0f, 0));
	AddObject(Vector3(0, -4.0f, 0));
	AddObject(Vector3(-10.0f, 4.0f, 0));
	AddObject(Vector3(-10.0f, -12.0f, 0));
}

void TransitionManager::AddObject(const Vector3& position)
{
	TransitionObject* newObject = new TransitionObject();
	newObject->Initialize(cloudModel_.get());
	newObject->InstanceSetting(position, Vector3(10.0f, 10.0f, 10.0f));
	objects_.push_back(newObject);
}
