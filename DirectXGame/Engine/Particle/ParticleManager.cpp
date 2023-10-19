#include "ParticleManager.h"
#include "TextureManager.h"
#include "MathCalc.h"
#include "VectorLib.h"
#include "ParticleWave.h"
#include "ParticleExplosion.h"
#include "ParticleGoal.h"
#include "GlobalVariables.h"
#include "Input.h"

ParticleManager::ParticleManager()
{
	planeModel_.reset(Model::CreatePlane());
	cubeModel_.reset(Model::CreateFromObj("block", true));
	texture_ = TextureManager::Load("plane/test2.png");
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
	goalOpen_ = { 0,30,false };

#pragma region json管理
	//GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	//const char* groupName = "ParticleManager";
	//// グループを追加
	//GlobalVariables::GetInstance()->CreateGroup(groupName);

	ApplyGlobalVariables();
#pragma endregion
}

void ParticleManager::Update()
{
	// ゴール解放中のエフェクト
	GoalEffectUpdate();
	// パターンごとの処理
	ParticleProcess();
	// パーティクルの更新
	ParticleUpdate();

}

void ParticleManager::WaveInitialize()
{
	waveTimer_ = { 0,60,true };
	AddWave({ 70.0f,60.0f,-15.0f }, { 1.5f,1.5f,1.0f });
}

void ParticleManager::WaveUpdate()
{
	// フレームカウント
	waveTimer_.frameCount++;
	// 終了判定
	if (waveTimer_.frameCount > waveTimer_.endCount) {
		patternRequest_ = PatternNum::kNone;
	}
	if (waveTimer_.frameCount % 30 == 0) {
		AddWave({ 70.0f,60.0f,-15.0f }, { 1.5f,1.5f,1.0f });
	}
}

void ParticleManager::ExplosionInitialize()
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
	goalOpen_.isNow = true;
	goalOpen_.endCount = 30;
}

void ParticleManager::GoalEffectUpdate()
{
	if (goalOpen_.isNow) {
		goalOpen_.frameCount += 1;
		if (goalOpen_.frameCount >= goalOpen_.endCount) {
			AddGoalParticle(goalPosition_, Vector3(0, 0, -1.0f), {2.0f,2.0f,2.0f});
			goalOpen_.frameCount = 0;
		}
	}

	if (Input::GetInstance()->TriggerKey(DIK_G)) {
		AddGoalParticle(goalPosition_, Vector3(0, 0, -1.0f), { 1.0f,1.0f,1.0f });
	}

}

void ParticleManager::CatchEnemyGenerate(const Vector3& pos)
{
	for (int i = 0; i < 10; i++) {
		AddSmokeParticle(pos, { 0.5f,0.5f,0.5f });
	}
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
	if (patternRequest_) {
		pattern_ = patternRequest_.value();

		switch (pattern_)
		{
		case PatternNum::kNone:

			break;
		case PatternNum::kMove:
			WaveInitialize();
			break;
		case PatternNum::kVibration:

			break;
		case PatternNum::kExplosion:
			ExplosionUpdate();
			//for (int i = 0; i < 10; i++) {
			//	RandomRespown({ 70.0f,60.0f,-100.0f });
			//}
			break;
		}
		patternRequest_ = std::nullopt;
	}

	switch (pattern_)
	{
	case PatternNum::kNone:
		break;
	case PatternNum::kMove:
		WaveUpdate();
		break;
	case PatternNum::kVibration:

		break;
	case PatternNum::kExplosion:

		break;
	}
}

void ParticleManager::Draw(ViewProjection& viewProjection)
{
	// 描画
	for (Particle* particle : particles_) {
		particle->Draw(viewProjection);
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

void ParticleManager::AddWave(const Vector3& pos, const Vector3& scale)
{
	ParticleWave* newParticle = new ParticleWave();
	newParticle->Initialize(planeModel_.get(), texture_);
	newParticle->SetPosition(pos);
	newParticle->SetVelocity({ 0,0,0 });
	newParticle->SetRotate(Vector3(0.0f, 0.0f, 0.0f));
	newParticle->SetScale(scale);
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
	newParticle->Initialize(planeModel_.get(), texture_);
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
