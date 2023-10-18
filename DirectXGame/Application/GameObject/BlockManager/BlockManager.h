#pragma once
#include <vector>
#include "Application/GameObject/Block/Block.h"

/// <summary>
/// ブロックマネージャー
/// </summary>
class BlockManager
{

public: // メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	~BlockManager();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, std::vector<uint32_t> textureHandles, int** map);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 設定
	/// </summary>
	void Setting();

	/// <summary>
	/// ブロック情報取得
	/// </summary>
	/// <param name="position">位置</param>
	/// <returns></returns>
	Block* GetBlock(Vector2 position);

private: // メンバ関数

	/// <summary>
	/// 調整項目適用関数
	/// </summary>
	void ApplyGlobalVariables();

public: // アクセッサ

private: // メンバ変数

	// ブロック
	std::vector<Block*> blocks_;

	//モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	std::vector<uint32_t> textureHandles_;

	// マップ
	int** map_;

};

