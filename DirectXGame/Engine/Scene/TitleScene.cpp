#include "TitleScene.h"
#include "Input.h"

void TitleScene::Initialize()
{
}

void TitleScene::Update()
{
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		sceneNum = STAGESELECT;
	}
}

void TitleScene::Draw()
{
}
