#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
#include "Application/Others/MapSystem/MapSystem.h"

class Block
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize(Model* model, uint32_t textureHandle, Vector2 position, MapSystem::MapNumber mapNum);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 設定
	/// </summary>
	void Setting(uint32_t textureHandle, MapSystem::MapNumber mapNum);

	/// <summary>
	/// ブロック落ちる
	/// </summary>
	void Fall(uint32_t textureHandle);

	/// <summary>
	/// ブロック落下アニメーション
	/// </summary>
	void FallAnimation(float startPoint, float endPoint);

public: // アクセッサ

	/// <summary>
	/// セットポジション
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(Vector2 position) { position_ = position; }

	/// <summary>
	/// ゲットポジション
	/// </summary>
	/// <param name="position"></param>
	Vector2 GetPosition() { return position_; }

	/// <summary>
	/// セットフラグ
	/// </summary>
	/// <param name="flag"></param>
	void SetIsFallNow(bool flag) { isFallNow_ = flag; }

	/// <summary>
	/// ゲットフラグ
	/// </summary>
	/// <returns></returns>
	bool GetFallNow() { return isFallNow_; }

	/// <summary>
	/// ゲットマップ番号
	/// </summary>
	/// <returns></returns>
	MapSystem::MapNumber GetMapNum() { return mapNum_; }

private: // メンバ変数

	//ワールドトランスフォーム
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// 現在のマス
	Vector2 position_;

	// マップ番号
	MapSystem::MapNumber mapNum_;

	// 落下中か
	bool isFallNow_ = false;

	// 落下中補完レート
	float fall_t_ = 0;

};

