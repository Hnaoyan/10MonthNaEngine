#include "EnemiesManager.h"
#include "Application/Others/MapSystem/MapSystem.h"

EnemiesManager::~EnemiesManager()
{

	// 解放
	for (Enemy* enemy : enemies_) {
		if (enemy) {
			delete enemy;
		}
	}
	for (Cage* cage : cages_) {
		if (cage) {
			delete cage;
		}
	}

}

void EnemiesManager::Iintialize(MapSystem* mapSystem, Model* enemyModel, Model* cageModel, size_t count)
{

	// マップシステム
	mapSystem_ = mapSystem;

	enemyModel_ = enemyModel;

	cageModel_ = cageModel;

	Setting(count);

}

void EnemiesManager::Update()
{

	size_t i = 0;
	for (Enemy* enemy : enemies_) {
		enemy->Update(mapSystem_->GetEnemyPosition().at(i));
		i++;
	}
	for (Cage* cage : cages_) {
		cage->Update();
	}

}

void EnemiesManager::Draw(const ViewProjection& viewProjection)
{

	for (Enemy* enemy : enemies_) {
		enemy->Draw(viewProjection);
	}
	for (Cage* cage : cages_) {
		cage->Draw(viewProjection);
	}

}

void EnemiesManager::Setting(size_t count)
{
	
	// 解放
	for (Enemy* enemy : enemies_) {
		if (enemy) {
			delete enemy;
		}
	}
	for (Cage* cage : cages_) {
		if (cage) {
			delete cage;
		}
	}
	// 生成
	for (size_t i = 0; i < count; i++) {
		AddEnemy(i); 
		AddCage(i);
	}


	// 設定
	size_t i = 0;
	for (Enemy* enemy : enemies_) {
		enemy->Setting(mapSystem_->GetInitialEnemyPosition().at(i));
		i++;
	}
	i = 0;
	for (Cage* cage : cages_) {
		cage->Setting(mapSystem_->GetInitialCagePosition().at(i));
		i++;
	}

}

void EnemiesManager::AddEnemy(size_t num)
{

	Enemy* enemy = new Enemy();
	enemy->Initialize(enemyModel_, mapSystem_->GetInitialEnemyPosition().at(num));
	enemies_.push_back(enemy);

}

void EnemiesManager::AddCage(size_t num)
{

	Cage* cage = new Cage();
	cage->Initialize(cageModel_, mapSystem_->GetInitialCagePosition().at(num));
	cages_.push_back(cage);

}
