#pragma once
#include "Model.h"
#include "Sprite.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "DirectXCommon.h"
#include "BaseCamera.h"
#include "Transition/CloudObject.h"
#include <list>
#include <memory>

class BackGroundManager
{
public:
	// DirectXのインスタンス
	DirectXCommon* dxCommon_ = nullptr;
	// ビュー
	ViewProjection viewProjection_;
	// 背景用カメラ
	std::unique_ptr<BaseCamera> backGroundCanvasCamera_;

public:
	BackGroundManager();
	~BackGroundManager() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private:
	/// <summary>
	/// 追加
	/// </summary>
	/// <param name="position"></param>
	/// <param name="scale"></param>
	void AddBackGroundCloud(const Vector3& position, const Vector3& scale, const Vector3& velocity);

	/// <summary>
	/// 削除処理
	/// </summary>
	void DeleteList();

	/// <summary>
	/// 背景の雲の生成関数
	/// </summary>
	void CreateCloud();

private:
	// 雲のモデル
	Model* cloudModel_ = nullptr;

	// 天球のモデル
	std::unique_ptr<Model> skydomeModel_;

	// 雲のリスト
	std::list<CloudObject*> clouds_;

	// 雲の削除座標
	float deletePositionX_ = -80.0f;

	// 雲の開始時の位置
	Vector3 startPosition_;

	// 雲の開始時のスケール
	Vector3 startScale_;

	WorldTransform skydomeWorldTransform_;

};

