#include "BackGroundManager.h"

BackGroundManager::BackGroundManager()
{
}

void BackGroundManager::Initialize(Model* model)
{
	// 初期設定
	dxCommon_ = DirectXCommon::GetInstance();
	viewProjection_.Initialize();

	// 雲モデルの読み込み
	cloudModel_ = model;
	cloudModel_->SetAlphaValue(0.8f);

	// 背景用カメラの生成・設定
	backGroundCanvasCamera_ = std::make_unique<BaseCamera>();
	backGroundCanvasCamera_->Initialize();
	backGroundCanvasCamera_->SetPosition({ 0, 5.0f, 0.0f });
	// 天球の読み込み
	skydomeModel_.reset(Model::CreateFromObj("tenkyu", true));

	skydomeWorldTransform_.Initialize();
	skydomeWorldTransform_.translation_ = backGroundCanvasCamera_->GetView().translate_;
	skydomeWorldTransform_.scale_ = { 60.0f,60.0f,60.0f };
	skydomeModel_->SetAlphaValue(0.5f);

	// スポーン場所の設定
	startPosition_ = { 35.0f,10.0f,0 };
	startScale_ = { 5.0f,5.0f,5.0f };

	AddBackGroundCloud(startPosition_, startScale_);
	AddBackGroundCloud({ startPosition_.x,startPosition_.y - 5.0f,0 }, startScale_);
	AddBackGroundCloud({ startPosition_.x,startPosition_.y - 10.0f,0 }, startScale_);
	AddBackGroundCloud({ startPosition_.x,startPosition_.y - 15.0f,0 }, startScale_);

}

void BackGroundManager::Update()
{
	// 削除判定・処理
	DeleteList();
	// 作成判定・処理
	CreateCloud();
	// 更新処理
	for (CloudObject* cloud : clouds_) {
		cloud->Update();
	}
	// 更新
	skydomeWorldTransform_.UpdateMatrix();
}

void BackGroundManager::Draw()
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
	for (CloudObject* cloud : clouds_) {
		cloud->Draw(viewProjection_);
	}

	skydomeModel_->Draw(skydomeWorldTransform_, viewProjection_);

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

void BackGroundManager::AddBackGroundCloud(const Vector3& position, const Vector3& scale)
{
	CloudObject* newObject = new CloudObject();
	newObject->Initialize(cloudModel_);
	newObject->SetPosition(position);
	newObject->SetScale(scale);
	newObject->SetVelocity({ -0.5f,0,0 });
	clouds_.push_back(newObject);
}

void BackGroundManager::DeleteList()
{
	// 死亡判定
	for (CloudObject* cloud : clouds_) {
		if (cloud->GetPosition().x <= deletePositionX_) {
			cloud->SetIsDead(true);
		}
	}

	// 削除処理
	clouds_.remove_if([](CloudObject* cloud) {
		if (cloud->GetIsDead()) {
			delete cloud;
			return true;
		}
		return false;
	});
}

void BackGroundManager::CreateCloud()
{
	int maxAddCount = 0;
	if (static_cast<int>(clouds_.size()) <= maxAddCount) {
		AddBackGroundCloud(startPosition_, startScale_);
	}

}
