#pragma once
#include "Transition/TransitionManager.h"

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

	/// <summary>
	/// 遷移マネージャーの設定
	/// </summary>
	/// <param name="manager"></param>
	void SetTransitionManager(TransitionManager* manager) { transitionManager_ = manager; }

protected:
	/// <summary>
	/// シーン番号
	/// </summary>
	static int sceneNum;
	
	/// <summary>
	/// ステージ番号
	/// </summary>
	static int stageNum;

	/// <summary>
	/// ステージ最大数
	/// </summary>
	static const int stageMax;

	TransitionManager* transitionManager_;

};
