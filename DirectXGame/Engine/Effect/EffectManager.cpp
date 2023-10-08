#include "EffectManager.h"
#include "VectorLib.h"
#include "GlobalVariables.h"

EffectManager* EffectManager::GetInstance()
{
    static EffectManager instance;
    return &instance;
}

void EffectManager::Initialize()
{
    // 調整項目クラスのインスタンス取得
    GlobalVariables* globalVariables = GlobalVariables::GetInstance();
    // グループ名設定
    const char* groupName = "EffectManager";
    // 指定した名前でグループ追加
    globalVariables->CreateGroup(groupName);

    globalVariables->AddItem(groupName, "HitStopEndTimer", hitStop_.endTimer);
    globalVariables->AddItem(groupName, "ShakeEndTimer", shake_.endTimer);

    hitStop_ = { false,0,20 };
    shake_ = { false,0,30 };

    ApplyGlobalVariables();
}

void EffectManager::Update()
{
    ApplyGlobalVariables();
}

void EffectManager::Update(EffectParameters type)
{
    if (type.isFlag) {
        type.countTimer++;
        if (type.countTimer >= type.endTimer) {
            type.isFlag = false;
            type.countTimer = 0;
        }
    }
}

void EffectManager::HitStopUpdate()
{
    if (hitStop_.isFlag) {
        hitStop_.countTimer++;
        if (hitStop_.countTimer >= hitStop_.endTimer) {
            hitStop_.isFlag = false;
            hitStop_.countTimer = 0;
        }
    }
}

void EffectManager::ShakeUpdate()
{
    if (shake_.isFlag) {
        shake_.countTimer++;
        if (shake_.countTimer >= shake_.endTimer) {
            shake_.isFlag = false;
            shake_.countTimer = 0;
        }
    }
}

Vector3 EffectManager::ShakeUpdate(const Vector3& pos, int rangeMax, int rangeMin)
{
    Vector3 result = {};
    result = { float(rand() % rangeMax - rangeMin),float(rand() % rangeMax - rangeMin) ,0 };
    result = VectorLib::Add(pos, result);
    return result;
}

Vector3 EffectManager::ShakeUpdate(const Vector3& pos, const Vector2& rangeMax, const Vector2& rangeMin)
{
    Vector3 result = {};
    result = { float(rand() % (int)rangeMax.x - rangeMin.y),float(rand() % (int)rangeMax.y - rangeMin.y) ,0 };
    result = VectorLib::Add(pos, result);
    return result;
}

void EffectManager::ApplyGlobalVariables()
{
    // 調整項目クラスのインスタンス取得
    GlobalVariables* globalVariables = GlobalVariables::GetInstance();
    // グループ名の設定
    const char* groupName = "EffectManager";

    hitStop_.endTimer = globalVariables->GetIntValue(groupName, "HitStopEndTimer");
    shake_.endTimer = globalVariables->GetIntValue(groupName, "ShakeEndTimer");

}
