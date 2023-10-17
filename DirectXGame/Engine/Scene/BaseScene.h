#pragma once

enum Scene {
	TITLE,GAMESCENE,CLEAR,EDITOR,STAGESELECT
};

class BaseScene 
{
public: // 仮想関数
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(){};
	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update(){};
	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw(){};
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BaseScene() = default;
	/// <summary>
	/// 設定
	/// </summary>
	virtual void Setting(Scene preScene) { preScene ;};

public: // 取得・設定
	/// <summary>
	/// シーン番号取得
	/// </summary>
	/// <returns></returns>
	int GetSceneNum() { return sceneNum; }

	/// <summary>
	/// シーン番号取得
	/// </summary>
	/// <returns></returns>
	void SetSceneNum(int num) { sceneNum = num; }

protected:
	/// <summary>
	/// シーン番号
	/// </summary>
	static int sceneNum;
	
	static int stageNum;

};
