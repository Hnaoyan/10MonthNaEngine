#include "TransitionObject.h"
#include "VectorLib.h"
#include "MathCalc.h"

void TransitionObject::Initialize(Model* model)
{
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.scale_ = { 15.0f,15.0f,15.0f };
	velocity_ = {};
	isTrans_ = true;
}

void TransitionObject::Update(const float Transition_T)
{
	worldTransform_.scale_ = VectorLib::Add(worldTransform_.scale_, Vector3(0.1f, 0.1f, 0.1f));
	worldTransform_.translation_ = MathCalc::EaseInQuadF(Transition_T, prevPos_, end_);
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
	end_ = { -70.0f,pos.y,pos.z };
}
