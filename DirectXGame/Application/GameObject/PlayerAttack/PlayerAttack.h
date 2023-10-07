#pragma once
#include "Application/GameObject/Block/Block.h"
#include <list>

class PlayerAttack
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(std::list<Block*> blocks);
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ブロックリストから削除
	/// </summary>
	void DeleteBlock();

	/// <summary>
	/// コンボアップ
	/// </summary>
	void ComboUp();

	/// <summary>
	/// ブロックリスト追加
	/// </summary>
	/// <param name="blocks"></param>
	void AddBlockList(std::list<Block*> blocks);

public: //アクセッサ

private: //メンバ変数

	// ブロック
	std::list<Block*> blocks_;

	// 親ブロック
	Block* parentBlock_;

	// コンボ
	uint32_t combo;

	// このフレームでコンボが増えたか
	bool isComboUp;

};

