// BulletPool.h

#include "Bullet.h"

class BulletPool {
public:
    Bullet* getOrCreateBullet();
    void returnBulletToPool(Bullet* bullet);
    cocos2d::Vector<Bullet*> bulletPool;

};
