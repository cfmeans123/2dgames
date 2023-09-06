#pragma once

#include "cocos2d.h"


USING_NS_CC;


enum CombatState
{
	Attack,
	Block,
	Idle,
	Stun
};
enum MoveState
{
	Normal,
	Jump,
	Sprint,
	Off
};

class Hero : public Sprite
{
public:
	static Hero* Create();
	//Hero(Hero &h);

	PhysicsBody* physicsBody;
	float spriteHeight;
	float spriteWidth;

	cocos2d::Size visibleSize = _director->getVisibleSize();
	cocos2d::Vec2 origin = _director->getVisibleOrigin();
	

	Animation* animationIdle;
	Animation* animationWalk;
	Animation* animationFall;
	Animation* animationJump;
	Animation* animationDeathOne;
	Animation* animationDeathTwo;
	Animation* animationAttackOne;
	Animation* animationAttackTwo;
	Animation* animationHurrah;


	Sprite* mario;
	//PhysicsBody* heroPhysicsBody;
	std::vector<PhysicsBody*> contacts;


	Vector<SpriteFrame*> walkingFrames;
	Vector<SpriteFrame*> idleFrames;
	Vector<SpriteFrame*> jumpFrames;
	Vector<SpriteFrame*> fallFrames;
	Vector<SpriteFrame*> runningFrames;
	Vector<SpriteFrame*> attack1Frames;
	Vector<SpriteFrame*> attack2Frames;
	Vector<SpriteFrame*> die1Frames;
	Vector<SpriteFrame*> die2Frames;
	Vector<SpriteFrame*> hurrahFrames;
	SpriteFrame* hitFrontFrame;
	SpriteFrame* hitBackFrame;

	MoveState mMoveState;
	CombatState mCombatState;

	//movement
	float defaultMoveSpeed = 256;
	float sprintMoveSpeed = 512;
	float mMoveSpeed = 256;

	//virtual void update(float dt);

	static Vector<SpriteFrame*> getAnimation(const char* format, int count)
	{
		auto spritecache = SpriteFrameCache::getInstance();
		Vector<SpriteFrame*> animFrames;
		char str[100];
		for (int i = 1; i <= count; i++)
		{
			snprintf(str, sizeof(str), format, i);
			animFrames.pushBack(spritecache->getSpriteFrameByName(str));
		}
		return animFrames;
	}

	
	

};


