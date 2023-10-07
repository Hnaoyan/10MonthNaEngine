#include "PlayerAttack.h"

void PlayerAttack::Initialize(std::list<Block*> blocks)
{

	// ブロック
	blocks_ = blocks;

	// 親ブロック
	parentBlock_ = blocks_.front();

	// コンボ
	combo = 1;

	// このフレームでコンボが増えたか
	isComboUp = false;

}

void PlayerAttack::Update()
{

	DeleteBlock();

	isComboUp = false;

}

void PlayerAttack::DeleteBlock()
{

	//blockの解放
	blocks_.remove_if([](Block* block) {
		if (!block) {
			return true;
		}
		return false;
	});

}

void PlayerAttack::ComboUp()
{

	combo++;

	isComboUp = true;

}

void PlayerAttack::AddBlockList(std::list<Block*> blocks)
{

	for (Block* block : blocks) {
		blocks_.push_back(block);
	}

}
