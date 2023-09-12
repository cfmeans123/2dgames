#pragma once
#include "cocos2d.h"

class CollisionVolume : public cocos2d::Node
{
public:
	CREATE_FUNC(CollisionVolume);
	float attackRange = 50.0f;
	bool init() override;
};