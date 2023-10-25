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

void EnemiesManager::Iintialize(MapSystem* mapSystem,
	Model* enemyModel, Model* sleepModel, Model* enemyMovePlanModel, Model* cageModel, Model* enemyDangerModel, Model* surprisedModel, Model* shadowModel,
	size_t enemyCount, size_t cageCount, uint32_t awakeEnemyTextureHandle, uint32_t sleepEnemyTextureHandle)
{

	// マップシステム
	mapSystem_ = mapSystem;

	enemyModel_ = enemyModel;
	
	sleepModel_ = sleepModel;

	enemyMovePlanModel_ = enemyMovePlanModel;

	cageModel_ = cageModel;

	enemyDangerModel_ = enemyDangerModel;

	surprisedModel_ = surprisedModel;

	shadowModel_ = shadowModel;

	awakeEnemyTextureHandle_ = awakeEnemyTextureHandle;
	sleepEnemyTextureHandle_ = sleepEnemyTextureHandle;

	Setting(enemyCount, cageCount);

}

void EnemiesManager::Update()
{
	// エネミー危険範囲
	enemyDangerWorldTransform_.clear();

	size_t i = 0;
	for (Enemy* enemy : enemies_) {
		enemy->SetRotate(direct_);
		enemy->Update(mapSystem_->GetEnemyPosition().at(i), mapSystem_->GetEnemyAwake().at(i), mapSystem_->GetCapturedEnemy().at(i));
		// エネミー危険範囲
		if (!mapSystem_->GetEnemyAwake().at(i)) {
			// エネミー位置
			int enemyMapX = static_cast<int>(enemy->GetPosition().x);
			int enemyMapY = static_cast<int>(enemy->GetPosition().y);
			EnemyDangerUpdate(enemyMapX, enemyMapY);
		}
		i++;
	}
	i = 0;
	for (EnemyMovePlan* enemyMovePlan : enemyMovePlans_) {
		enemyMovePlan->SetRotate(direct_);
		enemyMovePlan->Update(mapSystem_->GetNextEnemyPosition().at(i));
		i++;
	}
	for (Cage* cage : cages_) {
		cage->Update();
	}

}

void EnemiesManager::Draw(const ViewProjection& viewProjection)
{

	bool isShadowDraw = false;
	size_t i = 0;

	for (Enemy* enemy : enemies_) {
		if (mapSystem_->GetMap()[static_cast<int>(enemy->GetPosition().y)][static_cast<int>(enemy->GetPosition().x)] == MapSystem::MapNumber::Road &&
			!mapSystem_->GetCapturedEnemy()[i]) {
			isShadowDraw = true;
		}
		enemy->Draw(viewProjection, isShadowDraw);
		i++;
	}
	i = 0;
	for (EnemyMovePlan* enemyMovePlan : enemyMovePlans_) {
		if ( !(enemyMovePlan->GetPosition().x == enemies_.at(i)->GetPosition().x &&
			enemyMovePlan->GetPosition().y == enemies_.at(i)->GetPosition().y) ) {
			enemyMovePlan->Draw(viewProjection);
		}
		i++;
	}
	for (Cage* cage : cages_) {
		cage->Draw(viewProjection,
			mapSystem_->GetMap()[static_cast<int>(cage->GetPosition().y)][static_cast<int>(cage->GetPosition().x)] == MapSystem::MapNumber::Road);
	}

	for (WorldTransform enemyDangerPosition : enemyDangerWorldTransform_) {
		enemyDangerModel_->Draw(enemyDangerPosition, viewProjection);
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
	enemy->Initialize(enemyModel_, mapSystem_->GetInitialEnemyPosition().at(num), sleepModel_, surprisedModel_, shadowModel_, awakeEnemyTextureHandle_, sleepEnemyTextureHandle_);
	enemies_.push_back(enemy);

	EnemyMovePlan* enemyMovePlan = new EnemyMovePlan();
	enemyMovePlan->Initialize(enemyMovePlanModel_, mapSystem_->GetInitialEnemyPosition().at(num));
	enemyMovePlans_.push_back(enemyMovePlan);

}

void EnemiesManager::AddCage(size_t num)
{

	Cage* cage = new Cage();
	cage->Initialize(cageModel_, shadowModel_, mapSystem_->GetInitialCagePosition().at(num));
	cages_.push_back(cage);

}

Enemy* EnemiesManager::GetEnemy(Vector2 posision)
{

	for (Enemy* enemy : enemies_) {
		if (enemy->GetPosition().x == posision.x &&
			enemy->GetPosition().y == posision.y) {
			return enemy;
		}
	}

	return nullptr;
}

EnemyMovePlan* EnemiesManager::GetEnemyMovePlan(Vector2 posision)
{
	for (EnemyMovePlan* enemyMovePlan : enemyMovePlans_) {
		if (enemyMovePlan->GetPosition().x == posision.x &&
			enemyMovePlan->GetPosition().y == posision.y) {
			return enemyMovePlan;
		}
	}
	return nullptr;
}

Cage* EnemiesManager::GetCage(Vector2 posision)
{

	for (Cage* cage : cages_) {
		if (cage->GetPosition().x == posision.x &&
			cage->GetPosition().y == posision.y) {
			return cage;
		}
	}
	return nullptr;
}

void EnemiesManager::SetAnimationPosition(const Vector2& position)
{
	for (Enemy* enemy : enemies_) {
		enemy->SetEndPosition(position);
	}
}

void EnemiesManager::ActionAnimationInitialize()
{
	for (Enemy* enemy : enemies_) {
		enemy->ActionAnimationInitialize();
	}
}

void EnemiesManager::ActionAnimationUpdate()
{
	for (Enemy* enemy : enemies_) {
		enemy->ActionAnimationUpdate();
	}
	for (Cage* cage : cages_) {
		cage->ActionAnimationUpdate();
	}
}

void EnemiesManager::WaitingAnimationInitialize()
{

	for (Enemy* enemy : enemies_) {
		enemy->WaitingAnimationInitialize();
	}

}

void EnemiesManager::WaitingAnimationUpdate()
{

	for (Enemy* enemy : enemies_) {
		enemy->WaitingAnimationUpdate();
	}

}

void EnemiesManager::GameOverAnimationInitialize()
{

	Vector2 distance = { 0.0f, 0.0f };
	Vector2 playerPosition = { mapSystem_->GetPlayerPosition().x, mapSystem_->GetPlayerPosition().y };
	for (Enemy* enemy : enemies_) {
		distance.x = enemy->GetPosition().x - playerPosition.x;
		distance.y = enemy->GetPosition().y - playerPosition.y;
		if (std::fabsf(distance.x) + std::fabsf(distance.y) <= 1.0f) {
			Vector2 playerWorldPosition = { playerPosition.x * MapSystem::kSquareSize_.x, playerPosition.y * MapSystem::kSquareSize_.y };
			enemy->GameOverAnimationInitialize(playerWorldPosition);
		}
	}

}

void EnemiesManager::GameOverAnimationUpdate()
{

	for (Enemy* enemy : enemies_) {
		enemy->GameOverAnimationUpdate();
	}

}

void EnemiesManager::EnemyDangerUpdate(int x, int y)
{

		float enemyPositionZ = -7.0f;
		// 左
		int enemyMapLeft = x - 1;
		if (enemyMapLeft >= 0) {
			if (mapSystem_->GetMap()[y][enemyMapLeft] == MapSystem::MapNumber::Road) {
				WorldTransform worldTransform;
				worldTransform.Initialize();
				worldTransform.translation_ = { static_cast<float>(enemyMapLeft) * MapSystem::kSquareSize_.x, static_cast<float>(y) * MapSystem::kSquareSize_.y, enemyPositionZ };
				worldTransform.UpdateMatrix();
				enemyDangerWorldTransform_.push_back(worldTransform);
			}
		}
		// 右
		int enemyMapRight = x + 1;
		if (enemyMapRight < MapSystem::kMapSize_.x) {
			if (mapSystem_->GetMap()[y][enemyMapRight] == MapSystem::MapNumber::Road) {
				WorldTransform worldTransform;
				worldTransform.Initialize();
				worldTransform.translation_ = { static_cast<float>(enemyMapRight) * MapSystem::kSquareSize_.x, static_cast<float>(y) * MapSystem::kSquareSize_.y, enemyPositionZ };
				worldTransform.UpdateMatrix();
				enemyDangerWorldTransform_.push_back(worldTransform);
			}
		}
		// 上
		int enemyMapTop = y + 1;
		if (enemyMapTop < MapSystem::kMapSize_.y) {
			if (mapSystem_->GetMap()[enemyMapTop][x] == MapSystem::MapNumber::Road) {
				WorldTransform worldTransform;
				worldTransform.Initialize();
				worldTransform.translation_ = { static_cast<float>(x) * MapSystem::kSquareSize_.x, static_cast<float>(enemyMapTop) * MapSystem::kSquareSize_.y, enemyPositionZ };
				worldTransform.UpdateMatrix();
				enemyDangerWorldTransform_.push_back(worldTransform);
			}
		}
		// 下
		int enemyMapBottom = y - 1;
		if (enemyMapBottom >= 0) {
			if (mapSystem_->GetMap()[enemyMapBottom][x] == MapSystem::MapNumber::Road) {
				WorldTransform worldTransform;
				worldTransform.Initialize();
				worldTransform.translation_ = { static_cast<float>(x) * MapSystem::kSquareSize_.x, static_cast<float>(enemyMapBottom) * MapSystem::kSquareSize_.y, enemyPositionZ };
				worldTransform.UpdateMatrix();
				enemyDangerWorldTransform_.push_back(worldTransform);
			}
		}

}
