#include "BulletPool.h"

Bullet* BulletPool::getOrCreateBullet() {
	for (Bullet* bullet : bulletPool) {
		if (!bullet->isActive) {
			bullet->isActive = true;
			return bullet;
		}
	}

	Bullet* newBullet = Bullet::create();
	newBullet->isActive = true;
	bulletPool.pushBack(newBullet);	
	return newBullet;
}

void BulletPool::returnBulletToPool(Bullet* bullet) {
	bullet->stopAllActions();
	bullet->setPosition(Vec2::ZERO);
	bullet->moveDirection = Vec2::ZERO;
	bullet->isActive = false;
}