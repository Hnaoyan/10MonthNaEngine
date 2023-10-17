#pragma once
#include "UIBase.h"
#include <list>

class SpriteManager
{
public:
	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns></returns>
	static SpriteManager* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() {};

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// リストに追加
	/// </summary>
	/// <param name="texHandle"></param>
	/// <param name="position"></param>
	/// <param name="anchor"></param>
	void AddUI(int texHandle, const Vector2& position, const Vector2& anchor, const std::string& name);

	/// <summary>
	/// 指定したUIを削除
	/// </summary>
	/// <param name="name"></param>
	void DeleteUI(const std::string& name);

	/// <summary>
	/// タグからポインタの検索
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	UIBase* GetUI(const std::string& name);

	/// <summary>
	/// スプライトのリストを取得
	/// </summary>
	/// <returns></returns>
	std::list<UIBase*> GetList() { return uiSprites_; }

private:
	std::list<UIBase*> uiSprites_;

};

