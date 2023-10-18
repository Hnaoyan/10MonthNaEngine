#include "AnimationManager.h"

void AnimationManager::Initialize()
{

	// 行動アニメーションタイマー
	actionAnimationTimer_ = 0;

	// 行動アニメーションタイム
	actionAnimationTime_ = 1;

	// 行動アニメーションしているか
	isActionAnimation_ = false;

}

void AnimationManager::WaitUpdate()
{

	for (std::function<void()> function : waitingAnimations_) {
		function();
	}

}

void AnimationManager::ActionUpdate()
{

	for (std::function<void()> function : actionAnimations_) {
		function();
	}

	WaitUpdate();

	actionAnimationTimer_++;
	if (actionAnimationTimer_ == actionAnimationTime_) {
		isActionAnimation_ = false;
		actionAnimations_.clear();
	}

}

void AnimationManager::ActionStart()
{

	isActionAnimation_ = true;
	actionAnimationTimer_ = 0;

}

void AnimationManager::Reset()
{

	// 待機アニメーション関数リスト
	waitingAnimations_.clear();

	// 行動アニメーション関数リスト
	actionAnimations_.clear();

	// 行動アニメーションタイマー
	actionAnimationTimer_ = 0;

	// 行動アニメーションしているか
	isActionAnimation_ = false;

}

void AnimationManager::SetWaitingAnimation(std::function<void()> function)
{

	waitingAnimations_.push_back(function);

}

void AnimationManager::SetActionAnimation(std::function<void()> function)
{

	actionAnimations_.push_back(function);

}
