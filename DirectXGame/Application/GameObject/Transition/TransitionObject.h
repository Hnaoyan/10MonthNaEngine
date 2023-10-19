#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "ViewProjection.h"

class TransitionObject
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	void Initialize(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const float Transition_T);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="view"></param>
	void Draw(ViewProjection& view);

	/// <summary>
	/// 生成時の設定
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="scale"></param>
	void InstanceSetting(const Vector3& pos, const Vector3& scale);

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	Vector3 velocity_ = {};
	Vector3 prevPos_ = {};

	bool isTrans_ = false;

	float t_ = 0;

	Vector3 end_ = {};

public: // アクセッサ
	/// <summary>
	/// 速さの設定
	/// </summary>
	/// <param name="velocity"></param>
	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }

	/// <summary>
	/// 座標設定
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(const Vector3& pos) { worldTransform_.translation_ = pos; }

	/// <summary>
	/// イージングの開始座標
	/// </summary>
	/// <param name="pos"></param>
	void SetPrevPos(const Vector3& pos) { prevPos_ = pos; }

	/// <summary>
	/// 座標取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetPosition() { return worldTransform_.translation_; }

	/// <summary>
	/// 回転角設定
	/// </summary>
	/// <param name="rotate"></param>
	void SetRotate(const Vector3& rotate) { worldTransform_.rotation_ = rotate; }

	/// <summary>
	/// スケール設定
	/// </summary>
	/// <param name="scale"></param>
	void SetScale(const Vector3& scale) { worldTransform_.scale_ = scale; }

	WorldTransform GetWorld() { return worldTransform_; }

	void SetWorld(WorldTransform world) { worldTransform_ = world; }

};

