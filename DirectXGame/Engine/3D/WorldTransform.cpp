#include "WorldTransform.h"
#include "D3D12Lib.h"
#include "DirectXCommon.h"
#include <cassert>
#include "MathCalc.h"
#include <numbers>

void WorldTransform::Initialize()
{
	this->CreateConstBuffer();
	this->Map();
	this->UpdateMatrix();
}

void WorldTransform::CreateConstBuffer()
{
	HRESULT result;

	D3D12_HEAP_PROPERTIES heapProps = D3D12Lib::SetHeapProperties(D3D12_HEAP_TYPE_UPLOAD);

	D3D12_RESOURCE_DESC resourceDesc = D3D12Lib::SetResourceDesc((sizeof(ConstBufferDataWorldTransform) + 0xff) & ~0xff);

	result = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));

}

void WorldTransform::Map()
{
	HRESULT result = constBuff_->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));
}

void WorldTransform::TransferMatrix()
{
	constMap->matWorld = matWorld_;
}

void WorldTransform::SetViewProjection(ViewProjection* view)
{
	view_ = view;
}

void WorldTransform::UpdateMatrix() {
	this->matWorld_ =
		MatLib::MakeAffineMatrix(this->scale_, this->rotation_, this->translation_);
	//Matrix4x4 backToFrontMat = MatLib::MakeRotateYMatrix(std::numbers::pi_v<float>);
	//Matrix4x4 billBoardMat = MatLib::Multiply(backToFrontMat, view_->matView);
	//billBoardMat.m[3][0] = 0.0f;
	//billBoardMat.m[3][1] = 0.0f;
	//billBoardMat.m[3][2] = 0.0f;
	/*Matrix4x4 billBoardMat = MatLib::MakeInverse(view_->matView);
	billBoardMat.m[3][0] = 0.0f;
	billBoardMat.m[3][1] = 0.0f;
	billBoardMat.m[3][2] = 0.0f;*/

	//Matrix4x4 billBoardMat = MatLib::MakeBillBoard(
	//	translation_, view_->translate_, Vector3(0.0f, 1.0f, 0.0f));

	//Matrix4x4 worldMat = MatLib::MakeAffineMatrix(
	//	scale_, rotation_, translation_);
	//matWorld_ = MatLib::Multiply(billBoardMat, worldMat);
	// 定数バッファに転送
	TransferMatrix();
}