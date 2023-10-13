#include "Command.h"

void Command::Initialize()
{

	input_ = Input::GetInstance();

	// コマンド番号
	comandNumber_ = None;

	// 入力受付中か
	acceptingInput_ = true;

}

void Command::Update()
{

	// 入力受付していれば関数呼び出し
	if (acceptingInput_) {
		InputReception();
	}

}

void Command::InputReception()
{

	// キーボード操作

	// 左
	if (input_->TriggerKey(DIK_A) || input_->TriggerKey(DIK_LEFT)) {
		comandNumber_ = Left;
		acceptingInput_ = false;
	}
	// 右
	else if (input_->TriggerKey(DIK_D) || input_->TriggerKey(DIK_RIGHT)) {
		comandNumber_ = Right;
		acceptingInput_ = false;
	}
	// 上
	else if (input_->TriggerKey(DIK_W) || input_->TriggerKey(DIK_UP)) {
		comandNumber_ = Up;
		acceptingInput_ = false;
	}
	// 下
	else if (input_->TriggerKey(DIK_S) || input_->TriggerKey(DIK_DOWN)) {
		comandNumber_ = Down;
		acceptingInput_ = false;
	}
	// ワンボタン
	else if (input_->TriggerKey(DIK_SPACE)) {
		comandNumber_ = Button;
		acceptingInput_ = false;
	}
	// リセット
	else if (input_->TriggerKey(DIK_R)) {
		comandNumber_ = Restart;
		acceptingInput_ = false;
	}

}
