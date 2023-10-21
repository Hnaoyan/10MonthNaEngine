#include "AnimationManager.h"

void AnimationManager::Initialize()
{

	// 行動アニメーション
	actionAnimation_.Initialize();

	// ゲームクリアアニメーション
	gameClearAnimation_.Initialize();

	// ゲームオーバーアニメーション
	gameOverAnimation_.Initialize();

	// オープニングアニメーション
	openingAnimation_.Initialize();

}

void AnimationManager::WaitUpdate()
{

	for (std::function<void()> function : waitingAnimations_) {
		function();
	}

}

void AnimationManager::ActionUpdate()
{

	for (std::function<void()> function : actionAnimation_.animations_) {
		function();
	}

	WaitUpdate();

	actionAnimation_.animationTimer_++;
	if (actionAnimation_.animationTimer_ > actionAnimation_.animationTime_) {
		actionAnimation_.isAnimation_ = false;
		actionAnimation_.animations_.clear();
	}

}

void AnimationManager::ActionInitialize(uint32_t animationTime)
{

	actionAnimation_.isAnimation_ = true;
	actionAnimation_.animationTimer_ = 0;
	actionAnimation_.animationTime_ = animationTime;

}

void AnimationManager::GameClearUpdate()
{

	for (std::function<void()> function : gameClearAnimation_.animations_) {
		function();
	}

	gameClearAnimation_.animationTimer_++;
	if (gameClearAnimation_.animationTimer_ > gameClearAnimation_.animationTime_) {
		gameClearAnimation_.isAnimation_ = false;
		gameClearAnimation_.animations_.clear();
	}

}

void AnimationManager::GameClearInitialize()
{

	gameClearAnimation_.isAnimation_ = true;
	gameClearAnimation_.animationTimer_ = 0;

}

void AnimationManager::GameOverUpdate()
{

	for (std::function<void()> function : gameOverAnimation_.animations_) {
		function();
	}

	gameOverAnimation_.animationTimer_++;
	if (gameOverAnimation_.animationTimer_ > gameOverAnimation_.animationTime_) {
		gameOverAnimation_.isAnimation_ = false;
		gameOverAnimation_.animations_.clear();
	}

}

void AnimationManager::GameOverInitialize()
{

	gameOverAnimation_.isAnimation_ = true;
	gameOverAnimation_.animationTimer_ = 0;

}

void AnimationManager::OpeningUpdate()
{

	for (std::function<void()> function : openingAnimation_.animations_) {
		function();
	}

	openingAnimation_.animationTimer_++;
	if (openingAnimation_.animationTimer_ > openingAnimation_.animationTime_) {
		openingAnimation_.isAnimation_ = false;
		openingAnimation_.animations_.clear();
	}
	
}

void AnimationManager::OpeningInitialize()
{

	openingAnimation_.isAnimation_ = true;
	openingAnimation_.animationTimer_ = 0;

}

void AnimationManager::Reset()
{

	// 待機アニメーション関数リスト
	waitingAnimations_.clear();

	actionAnimation_.Reset();
	gameClearAnimation_.Reset();
	gameOverAnimation_.Reset();
	openingAnimation_.Reset();

}

void AnimationManager::SetWaitingAnimation(std::function<void()> function)
{

	waitingAnimations_.push_back(function);

}

void AnimationManager::SetActionAnimation(std::function<void()> function)
{

	actionAnimation_.animations_.push_back(function);

}

void AnimationManager::SetGameClearAnimation(std::function<void()> function)
{

	gameClearAnimation_.animations_.push_back(function);

}

void AnimationManager::SetGameOverAnimation(std::function<void()> function)
{

	gameOverAnimation_.animations_.push_back(function);

}

void AnimationManager::SetOpeningAnimation(std::function<void()> function)
{

	openingAnimation_.animations_.push_back(function);

}

void AnimationManager::AnimationStruct::Initialize()
{

	// 行動アニメーションタイマー
	animationTimer_ = 0;
	// 行動アニメーションタイム
	animationTime_ = 20;
	// 行動アニメーションしているか
	isAnimation_ = false;

}

void AnimationManager::AnimationStruct::Reset()
{
	// 行動アニメーション関数リスト
	animations_.clear();
	// 行動アニメーションタイマー
	animationTimer_ = 0;
	// 行動アニメーションしているか
	isAnimation_ = false;

}
