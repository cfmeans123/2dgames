#include "EnemyPool.h"

Enemy* EnemyPool::getOrCreateEnemy() {
	for (Enemy* enemy : enemyPool) {
		if (!enemy->isActive) {
			enemy->isActive = true;
			return enemy;
		}
	}

	Enemy* newEnemy = Enemy::create();
	newEnemy->isActive = true;
	enemyPool.pushBack(newEnemy);
	return newEnemy;
}

void EnemyPool::returnEnemyToPool(Enemy* enemy) {
	enemy->stopAllActions();
	enemy->setPosition(Vec2::ZERO);
	enemy->moveDirection = Vec2::ZERO;
	enemy->isActive = false;
}