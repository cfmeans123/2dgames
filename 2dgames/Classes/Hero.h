#pragma once

#include "cocos2d.h"

USING_NS_CC;

//class Hero : public Sprite
//{
//public:
//	Hero* Create();
//	PhysicsBody* physicsBody;
//	float spriteHeight;
//	float spriteWidth;
//
//
//	Vector<SpriteFrame*> walkingFrames;
//	Vector<SpriteFrame*> idleFrames;
//	Vector<SpriteFrame*> jumpFrames;
//	Vector<SpriteFrame*> runningFrames;
//	Vector<SpriteFrame*> runningFrames;
//	Vector<SpriteFrame*> attack1Frames;
//	Vector<SpriteFrame*> attack2Frames;
//	Vector<SpriteFrame*> die1Frames;
//	Vector<SpriteFrame*> die2Frames;
//	SpriteFrame* hitFrontFrame;
//	SpriteFrame* hitBackFrame;
//	SpriteFrame* hurrahFrame;
//
//	static Vector<SpriteFrame*> getAnimation(const char* format, int count)
//	{
//		auto spritecache = SpriteFrameCache::getInstance();
//		Vector<SpriteFrame*> animFrames;
//		char str[100];
//		for (int i = 1; i <= count; i++)
//		{
//			snprintf(str, sizeof(str), format, i);
//			animFrames.pushBack(spritecache->getSpriteFrameByName(str));
//		}
//		return animFrames;
//	}
//
//	enum CombatState
//	{
//		Attack,
//		Block,
//		Idle,
//		Stun
//	};
//	enum MoveState
//	{
//		Normal,
//		Jump,
//		Sprint
//	};
//	
//
//};
