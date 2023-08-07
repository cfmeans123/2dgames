// BulletPool.h

#include "Enemy.h"

class EnemyPool {
public:
    Enemy* getOrCreateEnemy();
    void returnEnemyToPool(Enemy* enemy);
    cocos2d::Vector<Enemy*> enemyPool;

};
#pragma once
