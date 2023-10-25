#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "CollisionManager.h"
#include "FollowCamera.h"
#include "BaseScene.h"
#include "BaseCamera.h"
#include "../Effect/EffectManager.h"

#include <memory>
#include <vector>

// ゲームシーン用
#include "SpriteManager.h"
#include "ParticleManager.h"

#include "Application/Others/Command/Command.h"
#include "Others/MapSystem/MapSystem.h"
#include "Application/GameObject/Player/Player.h"
#include "Application/GameObject/BlockManager/BlockManager.h"
#include "Application/GameObject/EnemiesManager/EnemiesManager.h"
#include "Application/GameObject/Goal/Goal.h"
#include "Application/Others/AnimationManager/AnimationManager.h"

// UI
#include "UI/CaptureEnemyUI/CaptureEnemyUI.h"
#include "UI/StageNumberUI/StageNumberUI.h"
#include <UI/MethodOfOperationUI/MethodOfOperationUI.h>


/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public BaseScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene() {};

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 設定
	/// </summary>
	/// <param name="preScene"></param>
	void Setting(Scene preScene) override;

private: // メンバ関数 ゲームシステム

	/// <summary>
	/// カメラ更新
	/// </summary>
	void CameraUpdate();

	/// <summary>
	/// コマンド待ち
	/// </summary>
	void WaitingCommand();

	/// <summary>
	/// 待機アニメーション
	/// </summary>
	void WaitingAnimation();

	/// <summary>
	/// 行動アニメーション
	/// </summary>
	void ActionAnimation();

	/// <summary>
	/// ゲームクリアアニメーション
	/// </summary>
	void GameClearAnimation();

	/// <summary>
	/// ゲームオーバーアニメーション
	/// </summary>
	void GameOverAnimation();

	/// <summary>
	/// オープニングアニメーション
	/// </summary>
	void OpeningAnimation();

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// 待機アニメーションセット
	/// </summary>
	void SetWaitingAnimation();

public: // メンバ関数 モデル

	/// <summary>
	/// モデル読み込み場所
	/// </summary>
	static void ModelSetting();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	ViewProjection viewProjection_;

private:	// メンバポインタ
	// ベースカメラ
	std::unique_ptr<BaseCamera> baseCamera_;

	bool isDebug_ = false;

	// 衝突マネージャー
	std::unique_ptr<CollisionManager> collisionManager;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	// コマンド
	std::unique_ptr<Command> command_;

	// マップ
	std::unique_ptr<MapSystem> mapSystem_;

	// 天球モデル
	std::unique_ptr<Model> skyDomeModel_;

	// プレイヤー
	std::unique_ptr<Player> player_;
	std::unique_ptr<Model> playerModel_;

	//ブロック
	std::unique_ptr<BlockManager> blockManager_;
	std::unique_ptr<Model> blockModel_;
	std::vector<uint32_t> blocktextureHandles_;

	// エネミー
	std::unique_ptr<EnemiesManager> enemiesManager_;
	std::unique_ptr<Model> enemyModel_;
	std::unique_ptr<Model> sleepModel_;
	std::unique_ptr<Model> enemyMovePlanModel_;
	std::unique_ptr<Model> cageModel_;
	std::unique_ptr<Model> enemyDagerModel_;
	std::unique_ptr<Model> surprisedModel_;
	std::unique_ptr<Model> shadowModel_;
	uint32_t awakeEnemyTextureHandle_ = 0u;
	uint32_t sleepEnemyTextureHandle_ = 0u;

	// ゴール
	std::unique_ptr<Goal> goal_;
	std::unique_ptr<Model> goalModel_;
	uint32_t goalRedTextureHandle_;
	uint32_t goalBlueTextureHandle_;

	// マネージャ系
	std::unique_ptr<EffectManager> effectManager_;
	std::unique_ptr<SpriteManager> uiManager_;
	std::unique_ptr<ParticleManager> particleManager_;
	std::unique_ptr<AnimationManager> animationManager_;


	/// <summary>
	/// UI用
	/// </summary>
	
	// 敵の数カウント
	std::unique_ptr<CaptureEnemyUI> captureEnemyUI_;
	// ステージ番号
	std::unique_ptr<StageNumberUI> stageNumberUI_;
	// 操作方法
	std::unique_ptr<MethodOfOperationUI> methodOfOperationUI_;

	// テクスチャ
	// EnemyKazu
	uint32_t enmyKazuTextureHandle_;
	// num
	uint32_t numTextureHandle_;
	// slash
	uint32_t slashTextureHandle_;
	// goGoal
	uint32_t goGoalTextureHandle_;
	
	// SE関係
	// 動けないときの音
	uint32_t deathSEHandle_;

	uint32_t clearSEHandle_;

	// テクスチャハンドル
	uint32_t stageNumberTextureHandle_;
	// 操作方法テクスチャハンドル
	std::vector<uint32_t> moveTextureHandles_;
	uint32_t vibrationTextureHandle_;
	uint32_t resetTextureHandle_;
	uint32_t stageSelectTextureHandle_;

	WorldTransform skyDomeWorldTransform_;

	std::unique_ptr<Sprite> sprite_;
	Vector2 size_;

	bool isShake_ = false;
	int shakeTime_ = 0;
	// SEの大きさ
	float SEVolume_ = 0;
	Vector3 cameraVect_ = {};

private: // ホワイトアウト用

	std::unique_ptr<Sprite> whiteSprite_;

	bool isWhiteOut_ = false;

	float whiteOutT_ = 0;

	float alphaValue_;

	void WhiteOutUpdate();

	void WhiteOutSetting();

};

