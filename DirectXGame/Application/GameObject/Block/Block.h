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

public: // アクセッサ

	/// <summary>
	/// セットポジション
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(Vector2 position) { position_ = position; }

	/// <summary>
	/// セットポジション
	/// </summary>
	/// <param name="position"></param>
	Vector2 GetPosition() { return position_; }

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

};

