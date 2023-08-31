//#include "Hero.h"
//
//
//Hero* Hero::Create()
//{
//	Hero* hero = new Hero();
//	hero->initWithFile("Hero/");
//	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Hero/Hero_Walk/Hero_Walk.plist");
//	this->walkingFrames = getAnimation("BlueKnight_entity_000_walk_%03d", 3);
//	this->idleFrames = getAnimation("mario_idle_%02d", 2);
//	this->jumpFrames = getAnimation("mario_jump_%02d", 1);
//	return nullptr;
//	/*mario = Sprite::createWithSpriteFrame(idleFrames.front());
//	addChild(mario);
//	mario->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
//	mario->setScale(2);
//
//	animation = Animation::createWithSpriteFrames(idleFrames, 1.0f);
//	animationWalk = Animation::createWithSpriteFrames(walkingFrames, 1.0f);
//	animationFall = Animation::createWithSpriteFrames(fallingFrames, 1.0f);
//	animationJump = Animation::createWithSpriteFrames(jumpingFrames, 1.0f);*/
//
//	//mario->runAction(RepeatForever::create(Animate::create(animation)));
//
//
//};
