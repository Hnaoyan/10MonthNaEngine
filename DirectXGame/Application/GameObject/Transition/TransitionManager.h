#pragma once
#include "StructManager.h"
#include "ViewProjection.h"
#include "Model.h"
#include "Transition/TransitionObject.h"
#include "DirectXCommon.h"
#include "BaseCamera.h"

#include <memory>

class TransitionManager
{
private:
	DirectXCommon* dxCommon_ = nullptr;
	// ビュー
	ViewProjection viewProjection_;

	std::unique_ptr<BaseCamera> transitionCamera_;

public:
	TransitionManager();
	~TransitionManager() = default;

	/// <summary>
	/// シングルトン
	/// </summary>
	/// <returns></returns>
	static TransitionManager* GetInstance();

	void Initialize();

	void Update();

	void Draw();

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
	void AddObject(const Vector3& position);

private:
	/// <summary>
	/// 遷移中か
	/// </summary>
	bool isNowTransition_ = false;

	int transitionTimer_ = 0;

	int endTimer_ = 0;

private:
	// モデル
	std::unique_ptr<Model> cloudModel_;
	// オブジェクトのリスト
	std::list<TransitionObject*> objects_;

};

