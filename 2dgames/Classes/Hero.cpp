#include "Hero.h"


Hero* Hero::Create()
{
	Hero* hero = new Hero();
	hero->initWithFile("Hero/Hero_HitBack/BlueKnight_entity_000_hit back_000.png");


	//Animation Init
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Hero/Hero_Walk/Hero_Walk.plist");
	hero->walkingFrames = getAnimation("BlueKnight_entity_000_walk_%03d.png", 9);
	hero->animationWalk = Animation::createWithSpriteFrames(hero->walkingFrames, 0.1f);
	
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Hero/Hero_Idle/Hero_Idle.plist");
	hero->idleFrames = getAnimation("BlueKnight_entity_000_Idle_%03d.png", 10);
	hero->animationIdle = Animation::createWithSpriteFrames(hero->idleFrames, 0.1f);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Hero/Hero_Jump/Hero_Jump.plist");
	hero->jumpFrames = getAnimation("BlueKnight_entity_000_jump_%03d.png", 9);
	hero->animationJump = Animation::createWithSpriteFrames(hero->jumpFrames, 0.1f);

	hero->fallFrames = getAnimation("BlueKnight_entity_000_jump_006.png", 1);
	hero->animationFall = Animation::createWithSpriteFrames(hero->fallFrames, 0.1f);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Hero/Hero_Die2/Hero_Die2.plist");
	hero->die1Frames = getAnimation("BlueKnight_entity_000_die backward_%03d.png", 8);
	hero->animationDeathOne = Animation::createWithSpriteFrames(hero->die1Frames, 0.1f);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Hero/Hero_Attack1/Hero_Attack1Plist.plist");
	hero->attack1Frames = getAnimation("BlueKnight_entity_000_basic attack 1_%03d.png", 9);
	hero->animationAttackOne = Animation::createWithSpriteFrames(hero->attack1Frames, 0.1f);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Hero/Hero_Attack2/Hero_Attack2Plist.plist");
	hero->attack2Frames = getAnimation("BlueKnight_entity_000_basic attack style 2_%03d.png", 9);
	hero->animationAttackTwo = Animation::createWithSpriteFrames(hero->attack2Frames, 0.1f);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Hero/Hero_Hurrah/Hero_Hurrah.plist");
	hero->hurrahFrames = getAnimation("BlueKnight_entity_000_summon_000_%03d.png", 8);
	hero->animationHurrah = Animation::createWithSpriteFrames(hero->hurrahFrames, 0.1f);
	//hero->runAction(RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(hero->idleFrames, 10.0f))));

	return hero;




};

//void Hero::update(float dt)
//{
//	switch (mMoveState)
//	{
//	case Normal:
//		
//		break;
//
//	case Jump:
//
//		break;
//
//	case Sprint:		
//
//		break;
//
//	case Off:
//		
//		break;
//	}
//	switch (mCombatState)
//	{
//	case Attack:
//
//		break;
//
//	case Block:
//
//		break;
//
//	case Idle:
//
//		break;
//
//	case Stun:
//
//		break;
//	}
//
//
//}
