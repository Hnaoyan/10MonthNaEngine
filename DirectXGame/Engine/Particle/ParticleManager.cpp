#include "ParticleManager.h"
#include "TextureManager.h"
#include "MathCalc.h"
#include "VectorLib.h"
#include "ParticleWave.h"
#include "ParticleExplosion.h"
#include "ParticleGoal.h"
#include "GlobalVariables.h"
#include "Input.h"

#pragma region 初期化

ParticleManager::ParticleManager()
{
	planeModel_.reset(Model::CreatePlane());
	cubeModel_.reset(Model::CreateFromObj("block", true));
	texture_ = TextureManager::Load("plane/test.png");
	goalEffectTexture_ = texture_;
	waveEffectTexture_ = TextureManager::Load("plane/test2.png");
}

ParticleManager* ParticleManager::GetInstance()
{
	static ParticleManager instance;
	return &instance;
}

void ParticleManager::Initialize(ViewProjection* view)
{
	// ビルボード用
	view_ = view;

	// ゴールが空いている時の管理変数
	goalOpenParameters_ = { 0,30,false };

#pragma region json管理
	//GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	//const char* groupName = "ParticleManager";
	//// グループを追加
	//GlobalVariables::GetInstance()->CreateGroup(groupName);

	ApplyGlobalVariables();
#pragma endregion
}
#pragma endregion

#pragma region 全体的な更新処理

void ParticleManager::Update()
{
	// パターンごとの処理
	ParticleProcess();
	// パーティクルの更新
	ParticleUpdate();

}
void ParticleManager::ParticleUpdate()
{
	// リストの更新
	for (Particle* particle : particles_) {
		particle->Update();
	}
	// リストの削除・リリース
	particles_.remove_if([](Particle* particle) {
		if (particle->IsDead()) {
			delete particle;
			// セーフガード
			particle = nullptr;
			return true;
		}
		return false;
		});
}

void ParticleManager::ParticleProcess()
{
	// ゴール解放中のエフェクト
	GoalEffectUpdate();

	// 起こす波の動き
	WaveUpdate();

}

void ParticleManager::Draw(ViewProjection& viewProjection)
{
	// 描画
	for (Particle* particle : particles_) {
		particle->Draw(viewProjection);
	}
}
#pragma endregion


void ParticleManager::WaveSetting(const Vector3& position)
{
	waveParameters_ = { 0,60,50,true };
	waveRespawnPosition_ = position;
	waveRespawnPosition_.z += 5.0f;
	AddWave(waveRespawnPosition_, { 5.0f,5.0f,1.0f });
}

void ParticleManager::WaveUpdate()
{
	if (waveParameters_.isNow) {
		// フレームカウント
		waveParameters_.frameCount++;
		// 終了判定
		if (waveParameters_.frameCount > waveParameters_.endCount) {
			waveParameters_.isNow = false;
		}
		// 出現処理
		if (waveParameters_.frameCount % waveParameters_.addInterval_ == 0) {
			AddWave(waveRespawnPosition_, { 5.0f,5.0f,1.0f });
		}
	}
}

void ParticleManager::ExplosionSetting()
{
}

void ParticleManager::ExplosionUpdate()
{
	float pos_z = 40.0f;
	// 右
	AddExplosion({ 70.0f,60.0f,-pos_z }, { 1,0,0 });
	AddExplosion({ 70.0f,60.0f,-pos_z }, { 1,0,1 });
	AddExplosion({ 70.0f,60.0f,-pos_z }, { 1,0,-1 });
	// 右上
	AddExplosion({ 70.0f,60.0f,-pos_z }, { 1,1,0 });
	AddExplosion({ 70.0f,60.0f,-pos_z }, { 1,1,1 });
	AddExplosion({ 70.0f,60.0f,-pos_z }, { 1,1,-1 });
	// 右下
	AddExplosion({ 70.0f,60.0f,-pos_z }, { 1,-1,0 });
	AddExplosion({ 70.0f,60.0f,-pos_z }, { 1,-1,1 });
	AddExplosion({ 70.0f,60.0f,-pos_z }, { 1,-1,-1 });
	// 左
	AddExplosion({ 70.0f,60.0f,-pos_z }, { -1,0,0 });
	AddExplosion({ 70.0f,60.0f,-pos_z }, { -1,0,1 });
	AddExplosion({ 70.0f,60.0f,-pos_z }, { -1,0,-1 });
	// 左上
	AddExplosion({ 70.0f,60.0f,-pos_z }, { -1,1,0 });
	AddExplosion({ 70.0f,60.0f,-pos_z }, { -1,1,1 });
	AddExplosion({ 70.0f,60.0f,-pos_z }, { -1,1,-1 });
	// 左下
	AddExplosion({ 70.0f,60.0f,-pos_z }, { -1,-1,0 });
	AddExplosion({ 70.0f,60.0f,-pos_z }, { -1,-1,1 });
	AddExplosion({ 70.0f,60.0f,-pos_z }, { -1,-1,-1 });
	// 上
	AddExplosion({ 70.0f,60.0f,-pos_z }, { 0,1,0 });
	AddExplosion({ 70.0f,60.0f,-pos_z }, { 0,1,1 });
	AddExplosion({ 70.0f,60.0f,-pos_z }, { 0,1,-1 });
	// 下
	AddExplosion({ 70.0f,60.0f,-pos_z }, { 0,-1,0 });
	AddExplosion({ 70.0f,60.0f,-pos_z }, { 0,-1,1 });
	AddExplosion({ 70.0f,60.0f,-pos_z }, { 0,-1,-1 });
	// 奥手前
	AddExplosion({ 70.0f,60.0f,-pos_z }, { 0,0,1 });
	AddExplosion({ 70.0f,60.0f,-pos_z }, { 0,0,-1 });
}

void ParticleManager::GoalEffectSetting(const Vector3& pos)
{
	goalPosition_ = pos;
	goalOpenParameters_ = { 0,30,true };
}

void ParticleManager::GoalEffectUpdate()
{
	if (goalOpenParameters_.isNow) {
		goalOpenParameters_.frameCount += 1;
		if (goalOpenParameters_.frameCount >= goalOpenParameters_.endCount) {
			AddGoalParticle(goalPosition_, Vector3(0, 0, -1.0f), {2.0f,2.0f,2.0f});
			goalOpenParameters_.frameCount = 0;
		}
	}
}

void ParticleManager::CatchEnemyGenerate(const Vector3& pos)
{
	for (int i = 0; i < 10; i++) {
		AddSmokeParticle(pos, { 0.5f,0.5f,0.5f });
	}
}

void ParticleManager::RandomRespown(const Vector3& point)
{
	for (int i = 0; i < 5; i++) {
		Vector3 resPos = { (float)(rand() % 5 - 2) + point.x,(float)(rand() % 5 - 2) + point.y ,(float)(rand() % 5 - 2) + point.z };
		Vector3 inverseVect = VectorLib::Subtract(resPos, point);
		float speed = 5.0f;
		float deltaTime = (1.0f / 60.0f) * speed;
		inverseVect = VectorLib::Scaler(inverseVect, deltaTime);
		AddExplosion(resPos, inverseVect);
	}
}

#pragma region パーティクルの生成関数
void ParticleManager::AddWave(const Vector3& pos, const Vector3& scale)
{
	ParticleWave* newParticle = new ParticleWave();
	newParticle->Initialize(planeModel_.get(), waveEffectTexture_);
	newParticle->SetPosition(pos);
	newParticle->SetScale(scale);
	newParticle->SetAddScaleValue(Vector2(0.4f, 0.4f));
	particles_.push_back(newParticle);
}

void ParticleManager::AddExplosion(const Vector3& pos, const Vector3& velo)
{
	ParticleExplosion* newParticle = new ParticleExplosion();
	newParticle->Initialize(cubeModel_.get(), texture_);
	newParticle->SetPosition(pos);
	newParticle->SetVelocity(velo);
	newParticle->SetRotate(Vector3(1.65f, 0.0f, 0.0f));
	newParticle->SetScale(Vector3(1.0f, 1.0f, 1.0f));
	particles_.push_back(newParticle);
}

void ParticleManager::AddGoalParticle(const Vector3& pos, const Vector3& velo, const Vector3& scale)
{
	ParticleGoal* newParticle = new ParticleGoal();
	newParticle->Initialize(planeModel_.get(), goalEffectTexture_);
	// 座標
	newParticle->SetPosition(pos);
	// 回転
	newParticle->SetRotate(Vector3(0.0f, 0.0f, 0.0f));
	// スケール
	newParticle->SetScale(scale);
	// 速さ
	newParticle->SetVelocity(velo);
	particles_.push_back(newParticle);

}

void ParticleManager::AddSmokeParticle(const Vector3& pos, const Vector3& scale)
{
	uint32_t tex_ = TextureManager::Load("uvChecker.png");
	Particle* newParticle = new Particle();
	newParticle->Initialize(planeModel_.get(), tex_);
	// 出現場所
	Vector3 randomPos = { float(rand() % 7 - 3) + pos.x,float(rand() % 7 - 3) + pos.y,pos.z };
	newParticle->SetPosition(randomPos);
	// 方向ベクトル
	Vector3 inverseVect = VectorLib::Subtract(randomPos, pos);
	inverseVect = MathCalc::Normalize(inverseVect);
	inverseVect = VectorLib::Scaler(inverseVect, 0.01f);
	newParticle->SetVelocity(Vector3(inverseVect.x, inverseVect.y, 0));
	// 角度
	newParticle->SetRotate(Vector3(0.0f, 0.0f, 0.0f));
	// スケール
	newParticle->SetScale(scale);
	newParticle->SetBillBoard(view_);
	particles_.push_back(newParticle);
}

void ParticleManager::ApplyGlobalVariables()
{
	// 調整項目クラスのインスタンス取得
	//GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名の設定
	//const char* groupName = "ParticleManager";

}
#pragma endregion
