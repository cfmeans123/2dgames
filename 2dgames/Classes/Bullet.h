#pragma once
#include "cocos2d.h"
USING_NS_CC;


class Bullet : public cocos2d::Sprite {
public:
    static Bullet* create();
    void update(float dt);
    bool isActive = true;
    void setDirection(const cocos2d::Vec2& direction);
    Vec2 moveDirection;
    float movementSpeed = 10.0f;

    float spriteHeight;
    float spriteWidth;

};