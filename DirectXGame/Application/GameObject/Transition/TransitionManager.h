﻿#pragma once
#include "StructManager.h"
#include "ViewProjection.h"
#include "Model.h"
#include "Transition/TransitionObject.h"
#include "DirectXCommon.h"
#include "BaseCamera.h"
#include "CloudObject.h"
#include "Sprite.h"
#include "Audio.h"

#include <memory>

class TransitionManager
{
private:
	DirectXCommon* dxCommon_ = nullptr;
	Audio* audio_ = nullptr;
	// ビュー
	ViewProjection viewProjection_;

	std::unique_ptr<BaseCamera> transitionCamera_;

	uint32_t changeSEHandle_ = 0u;

	float SEVolume_;

public:
	TransitionManager();
	~TransitionManager() = default;

	/// <summary>
	/// シングルトン
	/// </summary>
	/// <returns></returns>
	static TransitionManager* GetInstance();

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

	/// <summary>
	/// 遷移の呼び出し
	/// </summary>
	void TransitionSetting();

private: // 遷移用関数
	/// <summary>
	/// 遷移の時間などの処理
	/// </summary>
	void TransitionUpdate();

	/// <summary>
	/// 雲の遷移召喚
	/// </summary>
	void CloudTransition();

	/// <summary>
	/// オブジェクト追加
	/// </summary>
	/// <param name="position"></param>
	void AddCloud(const Vector3& position, const Vector3& scale);

private:
	/// <summary>
	/// 遷移中か
	/// </summary>
	bool isNowTransition_ = false;

	float transitionTimer_ = 0;

	int endTimer_ = 0;

	bool isSceneChanger_ = false;

	bool isSettingTransition_ = false;

public:

	bool IsGetSceneChanger() { return isSceneChanger_; }

	bool GetNowTransition() { return isNowTransition_; }

	void SetIsTransition(bool flag) { isSettingTransition_ = flag; }

private:
	// モデル
	Model* cloudModel_;
	// オブジェクトのリスト
	std::list<TransitionObject*> objects_;
};
