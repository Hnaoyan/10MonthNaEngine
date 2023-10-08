#include "EffectManager.h"
#include "VectorLib.h"

EffectManager* EffectManager::GetInstance()
{
    static EffectManager instance;
    return &instance;
}

void EffectManager::HitStopUpdate()
{
    if (isStop_) {
        stopTimer_++;
        if (stopTimer_ >= kHitStopTimer) {
            isStop_ = false;
            stopTimer_ = 0;
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
