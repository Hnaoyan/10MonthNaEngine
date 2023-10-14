#include "EditorScene.h"
#include <Sprite.h>

using namespace std;

void EditorScene::Initialize()
{

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	viewProjection_.Initialize();

	baseCamera_ = make_unique<BaseCamera>();
	baseCamera_->Initialize();

	//baseCamera_->SetPosition({ 10.0f, 20.0f, -70.0f });
	baseCamera_->SetRotation({ 0.0f, 0.0f, 0.0f });

	enemyModel_.reset(Model::CreateFromObj("enemy", true));
	cageModel_.reset(Model::CreateFromObj("cage", true));
	startModel_.reset(Model::CreateFromObj("start", true));
	goalModel_.reset(Model::CreateFromObj("goal", true));
	blockModel_.reset(Model::CreateFromObj("block", true));

	mapEdit_ = make_unique<MapEdit>();
	mapEdit_->Initialize(enemyModel_.get(), cageModel_.get(), startModel_.get(), goalModel_.get(), blockModel_.get());

}

void EditorScene::Update()
{

	baseCamera_->Update();

	viewProjection_.matView = baseCamera_->GetView().matView;
	viewProjection_.matProjection = baseCamera_->GetView().matProjection;
	viewProjection_.TransferMatrix();

	mapEdit_->Update();

}

void EditorScene::Draw()
{

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	//uiManager_->Draw();

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
	mapEdit_->Draw(viewProjection_);

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
