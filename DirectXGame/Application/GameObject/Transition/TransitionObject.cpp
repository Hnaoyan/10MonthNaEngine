#include "TransitionObject.h"

void TransitionObject::Initialize(Model* model)
{
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.scale_ = { 15.0f,15.0f,15.0f };
}

void TransitionObject::Update()
{
	worldTransform_.UpdateMatrix();
}

void TransitionObject::Draw(ViewProjection& view)
{
	model_->Draw(worldTransform_, view);
}

void TransitionObject::InstanceSetting(const Vector3& pos, const Vector3& scale)
{
	SetPosition(pos);
	SetPrevPos(pos);
	SetScale(scale);
}
