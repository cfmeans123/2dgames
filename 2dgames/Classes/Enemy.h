#pragma once
#include "cocos2d.h"
USING_NS_CC;


class Enemy : public cocos2d::Sprite {
public:
    static Enemy* create();
    void update(float dt);
    bool isActive = true;
    void setDirection(const cocos2d::Vec2& direction);
    Vec2 moveDirection;
    float movementSpeed = 5.0f;

    float spriteHeight;
    float spriteWidth;

};