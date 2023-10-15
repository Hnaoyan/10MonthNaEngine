#pragma once
#include "BaseScene.h"
#include <DirectXCommon.h>
#include <Input.h>
#include <Audio.h>
#include <ViewProjection.h>
#include <BaseCamera.h>
#include <memory>
#include <Model.h>

#include "Editor/MapEdit.h"

class EditorScene : public BaseScene
{

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	EditorScene() {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EditorScene() {};

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	ViewProjection viewProjection_;

private:	// メンバポインタ
	// カメラ
	//std::unique_ptr<DebugCamera> debugCamera_;
	// ベースカメラ
	std::unique_ptr<BaseCamera> baseCamera_;

	bool isDebug_ = false;

	// マップエディタ
	std::unique_ptr<MapEdit> mapEdit_;
	std::unique_ptr<Model> enemyModel_;
	std::unique_ptr<Model> cageModel_;
	std::unique_ptr<Model> startModel_;
	std::unique_ptr<Model> goalModel_;
	std::unique_ptr<Model> blockModel_;

};

