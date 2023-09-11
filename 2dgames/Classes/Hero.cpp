#include "Hero.h"
#include "AnimationHelper.h"

Hero* Hero::Create()
{
	Hero* hero = new Hero();
	hero->initWithFile("Hero/Hero_HitBack/BlueKnight_entity_000_hit back_000.png");


	//Animation Init
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Hero/Hero_Walk/Hero_Walk.plist");
	hero->walkingFrames = AnimationHelper::getAnimation("BlueKnight_entity_000_walk_%03d.png", 9);
	hero->animationWalk = Animation::createWithSpriteFrames(hero->walkingFrames, 0.1f);
	
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Hero/Hero_Idle/Hero_Idle.plist");
	hero->idleFrames = AnimationHelper::getAnimation("BlueKnight_entity_000_Idle_%03d.png", 10);
	hero->animationIdle = Animation::createWithSpriteFrames(hero->idleFrames, 0.1f);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Hero/Hero_Jump/Hero_Jump.plist");
	hero->jumpFrames = AnimationHelper::getAnimation("BlueKnight_entity_000_jump_%03d.png", 9);
	hero->animationJump = Animation::createWithSpriteFrames(hero->jumpFrames, 0.1f);

	hero->fallFrames = AnimationHelper::getAnimation("BlueKnight_entity_000_jump_006.png", 1);
	hero->animationFall = Animation::createWithSpriteFrames(hero->fallFrames, 0.1f);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Hero/Hero_Die2/Hero_Die2.plist");
	hero->die1Frames = AnimationHelper::getAnimation("BlueKnight_entity_000_die backward_%03d.png", 8);
	hero->animationDeathOne = Animation::createWithSpriteFrames(hero->die1Frames, 0.1f);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Hero/Hero_Attack1/Hero_Attack1Plist.plist");
	hero->attack1Frames = AnimationHelper::getAnimation("BlueKnight_entity_000_basic attack 1_%03d.png", 9);
	hero->animationAttackOne = Animation::createWithSpriteFrames(hero->attack1Frames, 0.1f);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Hero/Hero_Attack2/Hero_Attack2Plist.plist");
	hero->attack2Frames = AnimationHelper::getAnimation("BlueKnight_entity_000_basic attack style 2_%03d.png", 9);
	hero->animationAttackTwo = Animation::createWithSpriteFrames(hero->attack2Frames, 0.1f);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Hero/Hero_Hurrah/Hero_Hurrah.plist");
	hero->hurrahFrames = AnimationHelper::getAnimation("BlueKnight_entity_000_summon_000_%03d.png", 8);
	hero->animationHurrah = Animation::createWithSpriteFrames(hero->hurrahFrames, 0.1f);
	//hero->runAction(RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(hero->idleFrames, 10.0f))));

	return hero;
};
void Hero::initPhysics(TMXTiledMap* level)
{
	physicsBody = cocos2d::PhysicsBody::createCircle(getContentSize().width / 4, PHYSICSSHAPE_MATERIAL_DEFAULT, Vec2(0.0f, -40.0f));

	physicsBody->setRotationEnable(false);
	physicsBody->setDynamic(true);
	physicsBody->setCategoryBitmask(1);
	physicsBody->setCollisionBitmask(2);
	physicsBody->setContactTestBitmask(2);
	//heroPhysicsBody->setMass(5.0f);
	setPhysicsBody(physicsBody);
	getPhysicsBody()->setLinearDamping(0.1);
	getPhysicsBody()->setVelocityLimit(1024);
}


