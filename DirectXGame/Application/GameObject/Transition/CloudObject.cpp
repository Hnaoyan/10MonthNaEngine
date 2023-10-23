#include "CloudObject.h"

void CloudObject::Initialize(Model* model)
{
	model_ = model;
	worldTransform_.Initialize();

}

void CloudObject::Update()
{

	worldTransform_.UpdateMatrix();

}

void CloudObject::Draw(ViewProjection& view)
{

	model_->Draw(worldTransform_, view);

}
