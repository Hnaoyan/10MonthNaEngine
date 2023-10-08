#pragma once
#include "UIBase.h"
#include <list>

class UIManager
{
public:
	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns></returns>
	static UIManager* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() {};

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 追加
	/// </summary>
	/// <param name="texHandle"></param>
	/// <param name="position"></param>
	/// <param name="anchor"></param>
	void AddUI(int texHandle, const Vector2& position, const Vector2& anchor);

private:
	std::list<UIBase*> uiSprites_;

};

