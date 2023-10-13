#pragma once
#include <Input.h>

/// <summary>
/// コマンドクラス
/// </summary>
class Command
{

public: // サブクラス
	// コマンド番号
	enum CommandNumber {
		None,   // ナシ(デバッグ)
		Left,   // 左
		Right,  // 右
		Up,     // 上
		Down,   // 下
		Button, // ワンボタン
		Restart // リスタート
	};

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

private: // メンバ関数

	/// <summary>
	/// 入力受付
	/// </summary>
	void InputReception();

public: // アクセッサ

	// コマンド番号
	int GetCommandNumber() { return static_cast<int>(comandNumber_); }

	// 入力受付中か
	void SetAcceptingInput(bool acceptingInput) { acceptingInput_ = acceptingInput; }

private: // メンバ変数

	// インプット
	Input* input_ = nullptr;

	// コマンド番号
	CommandNumber comandNumber_;

	// 入力受付中か
	bool acceptingInput_;

};

