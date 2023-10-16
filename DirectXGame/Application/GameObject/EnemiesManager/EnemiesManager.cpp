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
	for (EnemyMovePlan* enemyMovePlan : enemyMovePlans_) {
		if (enemyMovePlan) {
			delete enemyMovePlan;
		}
	}
	for (Cage* cage : cages_) {
		if (cage) {
			delete cage;
		}
	}

}

void EnemiesManager::Iintialize(MapSystem* mapSystem, Model* enemyModel, Model* enemyMovePlanModel, Model* cageModel, size_t enemyCount, size_t cageCount)
{

	// マップシステム
	mapSystem_ = mapSystem;

	enemyModel_ = enemyModel;

	enemyMovePlanModel_ = enemyMovePlanModel;

	cageModel_ = cageModel;

	Setting(enemyCount, cageCount);

}

void EnemiesManager::Update()
{

	size_t i = 0;
	for (Enemy* enemy : enemies_) {
		enemy->Update(mapSystem_->GetEnemyPosition().at(i));
		i++;
	}
	i = 0;
	for (EnemyMovePlan* enemyMovePlan : enemyMovePlans_) {
		enemyMovePlan->Update(mapSystem_->GetNextEnemyPosition().at(i));
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
	size_t i = 0;
	for (EnemyMovePlan* enemyMovePlan : enemyMovePlans_) {
		if ( !(enemyMovePlan->GetPosition().x == enemies_.at(i)->GetPosition().x &&
			enemyMovePlan->GetPosition().y == enemies_.at(i)->GetPosition().y) ) {
			enemyMovePlan->Draw(viewProjection);
		}
		i++;
	}
	for (Cage* cage : cages_) {
		cage->Draw(viewProjection);
	}

}

void EnemiesManager::Setting(size_t enemyCount, size_t cageCount)
{
	
	// 解放
	for (Enemy* enemy : enemies_) {
		if (enemy) {
			delete enemy;
		}
	}
	enemies_.clear();
	for (EnemyMovePlan* enemyMovePlan : enemyMovePlans_) {
		if (enemyMovePlan) {
			delete enemyMovePlan;
		}
	}
	enemyMovePlans_.clear();
	for (Cage* cage : cages_) {
		if (cage) {
			delete cage;
		}
	}
	cages_.clear();
	// 生成
	for (size_t i = 0; i < enemyCount; i++) {
		AddEnemy(i); 
	}
	for (size_t i = 0; i < cageCount; i++) {
		AddCage(i);
	}

	// 設定
	size_t i = 0;
	for (Enemy* enemy : enemies_) {
		enemy->Setting(mapSystem_->GetInitialEnemyPosition().at(i));
		i++;
	}
	i = 0;
	for (EnemyMovePlan* enemyMovePlan : enemyMovePlans_) {
		enemyMovePlan->Update(mapSystem_->GetNextEnemyPosition().at(i));
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

	EnemyMovePlan* enemyMovePlan = new EnemyMovePlan();
	enemyMovePlan->Initialize(enemyMovePlanModel_, mapSystem_->GetInitialEnemyPosition().at(num));
	enemyMovePlans_.push_back(enemyMovePlan);

}

void EnemiesManager::AddCage(size_t num)
{

	Cage* cage = new Cage();
	cage->Initialize(cageModel_, mapSystem_->GetInitialCagePosition().at(num));
	cages_.push_back(cage);

}
