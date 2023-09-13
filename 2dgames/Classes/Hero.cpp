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
	hero->setScale(0.2);
	hero->reset();


	hero->collisionVolume = CollisionVolume::create();
	hero->collisionVolume->setName("CollisionVolume");
	hero->collisionVolume->setVisible(false);
	hero->addChild(hero->collisionVolume);


	return hero;
};
void Hero::initPhysics(TMXTiledMap* level)
{
	physicsBody = cocos2d::PhysicsBody::createCircle(getContentSize().width / 4, PHYSICSSHAPE_MATERIAL_DEFAULT, Vec2(0.0f, -40.0f));
	attackCollisionMask = cocos2d::PhysicsShapeBox::create(cocos2d::Size(100, 75), PHYSICSSHAPE_MATERIAL_DEFAULT, Vec2(0.0f, 0.0f));
	attackCollisionMask->setCategoryBitmask(1);
	attackCollisionMask->setCollisionBitmask(2);
	attackCollisionMask->setContactTestBitmask(1);

	physicsBody->setRotationEnable(false);
	physicsBody->setDynamic(true);
	physicsBody->setCategoryBitmask(1);
	physicsBody->setCollisionBitmask(2);
	physicsBody->setContactTestBitmask(2);
	setPhysicsBody(physicsBody);
	getPhysicsBody()->setLinearDamping(0.1);
	getPhysicsBody()->setVelocityLimit(1024);
}

void Hero::setCombatState(CombatState state)
{
	if (mCombatState == state)
	{
		return;
	}

	exitCurrentCombatState();
	mCombatState = state;
	enterNewCombatState();
}

void Hero::setMoveState(MoveState state)
{
	if (mMoveState == state)
	{
		return;
	}
	exitCurrentMoveState();
	mMoveState = state;
	enterNewMoveState();
}

void Hero::reset()
{
	setCombatState(CombatState::None);
	setMoveState(MoveState::Idle);
}

void Hero::enterNewCombatState()
{
	switch (mCombatState)
	{
	case CombatState::None:
		break;
	case CombatState::Attack:
		setMoveState(MoveState::Off);
		this->stopAllActions();
		toggleCollisionVolume();
		this->runAction(RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(attack2Frames, 0.08))));
		stunTimer = 0.5f;
		break;
	case CombatState::Stun:
		this->stopAllActions();
		break;
	}
}
void Hero::enterNewMoveState()
{
	switch (mMoveState)
	{
	case MoveState::Idle:
		stopAllActions();
		physicsBody->setVelocity(Vec2(0.0f, physicsBody->getVelocity().y));
		runAction(RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(idleFrames, 0.1))));
		break;
	case MoveState::Jump:
		stopAllActions();
		getPhysicsBody()->applyImpulse(Vec2(0, 450), Vec2::ZERO);
		runAction(RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(jumpFrames, 0.1))));
		break;
	case MoveState::Sprint:
		mMoveSpeed = sprintMoveSpeed;
		frameSpeed = sprintFrameSpeed;
		stopAllActions();
		runAction(RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(walkingFrames, frameSpeed))));
		break;
	case MoveState::Walk:
		stopAllActions();
		runAction(RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(walkingFrames, frameSpeed))));
		break;
	}
}

void Hero::exitCurrentCombatState()
{
	switch (mCombatState)
	{
	case CombatState::None:
		break;
	case CombatState::Attack:
		toggleCollisionVolume();
		break;
	case CombatState::Stun:
		break;
	}
}
void Hero::exitCurrentMoveState()
{
	switch (mMoveState)
	{
	case MoveState::Jump:
		break;
	case MoveState::Sprint:
		mMoveSpeed = defaultMoveSpeed;
		frameSpeed = 0.1f;
		if (physicsBody->getVelocity().x > defaultMoveSpeed)
		{
			physicsBody->setVelocity(Vec2(defaultMoveSpeed, physicsBody->getVelocity().y));
		}
		else if (physicsBody->getVelocity().x < -defaultMoveSpeed)
		{
			physicsBody->setVelocity(Vec2(-defaultMoveSpeed, physicsBody->getVelocity().y));
		}
		break;
	case MoveState::Idle:
		break;
	case MoveState::Walk:
		break;
	}
}

void Hero::updateNoneState(float dt)
{

}

void Hero::updateAttackState(float dt)
{
	stunTimer -= dt;
	if (stunTimer <= 0.0f)
	{
		setCombatState(CombatState::None);
	}
	const auto& velx = physicsBody->getVelocity().x;
	if (velx != 0)
	{
		physicsBody->applyForce(Vec2((-1 * velx), 0.0f));
	}

}

void Hero::updateStunState(float dt)
{
	stunTimer -= dt;
	if (stunTimer <= 0.0f)
	{
		setCombatState(CombatState::None);
	}
}

void Hero::update(float dt)
{
	switch (mCombatState)
	{
	case CombatState::None:
		updateNoneState(dt);
		break;
	case CombatState::Attack:
		updateAttackState(dt);
		break;
	case CombatState::Stun:
		updateStunState(dt);
		break;
	}
}


void Hero::toggleCollisionVolume()
{
	auto collisionVolume = getChildByName("CollisionVolume"); // Assuming you named the child node
	if (collisionVolume)
	{
		collisionVolume->setVisible(!collisionVolume->isVisible());
	}
}


