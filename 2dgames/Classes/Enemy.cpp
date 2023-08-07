#include "Enemy.h"

USING_NS_CC;

Enemy* Enemy::create() {
    Enemy* enemy = new Enemy();
    if (enemy) {
        enemy->autorelease();
        enemy->initWithFile("interceptor_01.png");
        enemy->isActive = false;
        enemy->spriteHeight = enemy->getContentSize().height;
        enemy->spriteWidth = enemy->getContentSize().width;
        return enemy;
    }
    CC_SAFE_DELETE(enemy);
    return nullptr;
}

void Enemy::update(float dt) {
    if (isActive) {
        this->setPositionX(this->getPositionX() + (moveDirection.x * (movementSpeed * dt)));
        this->setPositionY(this->getPositionY() + (moveDirection.y * (movementSpeed * dt)));
    }
}

void Enemy::setDirection(const Vec2& direction) {
    moveDirection = direction;
    
}