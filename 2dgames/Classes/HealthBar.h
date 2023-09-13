#pragma once
#include "cocos2d.h"

struct HealthBar
{
public:
    cocos2d::Sprite* fillBar;
    cocos2d::Sprite* emptyBar;

    HealthBar()
    {
        fillBar = cocos2d::Sprite::create("bar_red.png");
        emptyBar = cocos2d::Sprite::create("bar_empty.png");
        maxHealth = 100.0f;
        currentHealth = 100.0f;
    }
    float maxHealth;
    float currentHealth;
};

