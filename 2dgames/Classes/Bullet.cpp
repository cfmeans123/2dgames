#include "Bullet.h"

USING_NS_CC;

Bullet* Bullet::create() {
    Bullet* bullet = new Bullet();
    if (bullet) {
        bullet->autorelease();
        bullet->initWithFile("carrot.png");
        bullet->isActive = false;
        return bullet;
    }
    CC_SAFE_DELETE(bullet);
    return nullptr;
}

void Bullet::update(float dt) {
    if (isActive) {
        this->setPositionX(this->getPositionX() + (moveDirection.x * (movementSpeed * dt)));
        this->setPositionY(this->getPositionY() + (moveDirection.y * (movementSpeed * dt)));
    }
}

void Bullet::setDirection(const Vec2& direction) {
    moveDirection = direction;
}