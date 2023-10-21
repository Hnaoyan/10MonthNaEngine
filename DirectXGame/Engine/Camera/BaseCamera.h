#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"

class BaseCamera
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 調整項目適用関数
	/// </summary>
	void ApplyGlobalVariables();

public: // オープニングアニメーション

	/// <summary>
	/// 初期化
	/// </summary>
	void OpeningAnimationInitialize();

	/// <summary>
	/// 更新
	/// </summary>
	void OpeningAnimationUpdate();

public: // 設定・取得
#pragma region セッターとゲッター
	/// <summary>
	/// ビューの取得
	/// </summary>
	/// <returns></returns>
	ViewProjection GetView() { return viewProjection_; }

	/// <summary>
	/// アドレス用
	/// </summary>
	/// <returns></returns>
	ViewProjection* GetViewPlayer() { return &viewProjection_; }

	/// <summary>
	/// 座標設定
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(const Vector3& pos) { viewProjection_.translate_ = pos; }

	/// <summary>
	/// 角度設定
	/// </summary>
	/// <param name="rot"></param>
	void SetRotation(const Vector3& rot) { viewProjection_.rotation_ = rot; }

	/// <summary>
	/// スケール設定
	/// </summary>
	/// <param name="scale"></param>
	void SetScale(const Vector3& scale) { viewProjection_.scale_ = scale; }

	/// <summary>
	/// 視野角設定
	/// </summary>
	/// <param name="range"></param>
	void SetFov(float range) { fov_ = range; }

	/// <summary>
	/// シェイクなどで移動したカメラを初期位置に戻す
	/// </summary>
	void ResetPosition();

	/// <summary>
	/// 初期座標取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetInitPosition() { return initPosition_; }

	/// <summary>
	/// オープニングアニメーションフレーム数
	/// </summary>
	/// <returns></returns>
	uint32_t GetOpeningFrame() { return openingFrame_; }

#pragma endregion
private:

	ViewProjection viewProjection_;

	Vector3 initPosition_;
	Vector3 initRotate_;

	float fov_ = 45.0f;

private: // オープニングアニメーション
	
	// オープニングスタート位置
	Vector3 openingStartPostion_;
	// オープニングエンド位置
	Vector3 openingEndPostion_;
	// オープニングスタート角度
	Vector3 openingStartRotate_;
	// オープニングエンド角度
	Vector3 openingEndRotate_;

	// フレーム数
	uint32_t openingFrame_;
	// t
	float openingT_;

};

