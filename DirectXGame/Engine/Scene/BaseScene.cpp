#include "BaseScene.h"

int BaseScene::sceneNum = TITLE;

int BaseScene::stageNum = 0;

const int BaseScene::stageMax = 15;

bool BaseScene::stageClear[stageMax] = {false,false, false, false, false, false, false, false, false, false, false, false, false, false, false };
