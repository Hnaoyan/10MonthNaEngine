#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"

class CloudObject
{
public:
	void Initialize(Model* model);

	void Update();

	void Draw(ViewProjection& view);

private:
	bool isDead_ = false;

	Vector3 velocity_ = {};

	Model* model_ = nullptr;

	WorldTransform worldTransform_;

public:
	/// <summary>
	/// 設定
	/// </summary>
	/// <param name="dead"></param>
	void SetIsDead(bool dead) { isDead_ = dead; }

	bool GetIsDead() { return isDead_; }

};

