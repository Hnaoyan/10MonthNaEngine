#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "CollisionManager.h"
#include "FollowCamera.h"
#include "BaseScene.h"
#include "BaseCamera.h"
#include "../Effect/EffectManager.h"

#include <memory>

// ゲームシーン用
#include "UIManager.h"
#include "ParticleManager.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public BaseScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene() {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene() {};

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

public: // メンバ関数

	/// <summary>
	/// カメラ更新
	/// </summary>
	void CameraUpdate();

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// モデル読み込み場所
	/// </summary>
	static void ModelSetting();

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

	// 衝突マネージャー
	std::unique_ptr<CollisionManager> collisionManager;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	//エリア

	// プレイヤー

	//ブロック

	// エネミー

	// マネージャ系
	std::unique_ptr<EffectManager> effectManager_;
	std::unique_ptr<UIManager> uiManager_;
	std::unique_ptr<ParticleManager> particleManager_;

	std::unique_ptr<Sprite> sprite_;
	Vector2 size_;

	bool isShake_ = false;
	int shakeTime_ = 0;
	Vector3 cameraVect_ = {};

};

