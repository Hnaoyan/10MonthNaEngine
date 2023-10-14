#include "ParticleManager.h"
#include "TextureManager.h"
#include "VectorLib.h"
#include "ParticleWave.h"

ParticleManager* ParticleManager::GetInstance()
{
	static ParticleManager instance;
	return &instance;
}

void ParticleManager::Initialize(ViewProjection* view)
{
	view_ = view;
	model_.reset(Model::CreatePlane());
	texture_ = TextureManager::Load("plane/test2.png");
}

void ParticleManager::Update()
{
	ParticleProcess();
	// パーティクルの更新
	ParticleUpdate();
}

void ParticleManager::WaveInitialize()
{
	waveTimer_ = { 0,60 };
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
		Test(Vector3(70.0f, 60.0f, -180.0f));
	}
}

void ParticleManager::ParticleUpdate()
{
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
		case ParticleManager::PatternNum::kNone:

			break;
		case ParticleManager::PatternNum::kMove:
			WaveInitialize();
			break;
		case ParticleManager::PatternNum::kVibration:

			break;
		}
		patternRequest_ = std::nullopt;
	}

	switch (pattern_)
	{
	case ParticleManager::PatternNum::kNone:
		break;
	case ParticleManager::PatternNum::kMove:
		WaveUpdate();
		break;
	case ParticleManager::PatternNum::kVibration:

		break;
	}
}

void ParticleManager::Draw(ViewProjection& viewProjection)
{
	for (Particle* particle : particles_) {
		particle->Draw(viewProjection);
	}
}

void ParticleManager::RandomRespown(const Vector3& point)
{
	for (int i = 0; i < 5; i++) {
		Vector3 resPos = { (float)(rand() % 5 - 2) + point.x,(float)(rand() % 5 - 2) + point.y ,(float)(rand() % 5 - 2) + point.z };
		Vector3 inverseVect = VectorLib::Subtract(resPos, point);
		float speed = 3.0f;
		float deltaTime = (1.0f / 60.0f) * speed;
		inverseVect = VectorLib::Scaler(inverseVect, deltaTime);
		AddParticle3D(resPos, inverseVect);
	}
}

void ParticleManager::Test(const Vector3& point)
{
	Vector3 inverseVect = {};
	Vector3 pos = point;
	AddParticle3D(pos, inverseVect);
}

void ParticleManager::AddParticle2D(Vector3& position, Vector3& velocity)
{
	Particle* newParticle = new Particle();
	newParticle->Initialize(model_.get(), texture_);
	newParticle->SetPosition(position);
	newParticle->SetVelocity(velocity);
	newParticle->SetBillBoard(view_);
	particles_.push_back(newParticle);
}

void ParticleManager::AddParticle3D(Vector3& position, Vector3& velocity)
{
	Particle* newParticle = new Particle();
	newParticle->Initialize(model_.get(), texture_);
	newParticle->SetPosition(position);
	newParticle->SetVelocity(velocity);
	newParticle->SetRotate(Vector3(1.65f, 0.0f, 0.0f));
	newParticle->SetScale(Vector3(2.0f, 2.0f, 1.0f));
	particles_.push_back(newParticle);
}

void ParticleManager::AddWave(const Vector3& pos, const Vector3& velo)
{
	//ParticleWave* newParticle = new ParticleWave();
	pos;
	velo;

}
