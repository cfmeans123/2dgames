#include "RolyPoly.h"


bool Monster::init()
{
	if (!Sprite::init())
	{
		return false;
	}
	setScale(2);

	stun = Sprite::create("Art_for_2dGamesFinal/stun.png");
	search = Sprite::create("Art_for_2dGamesFinal/search.png");
	attack = Sprite::create("Art_for_2dGamesFinal/attack.png");
	ded = Sprite::create("Art_for_2dGamesFinal/ded.png");
	/*cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Art_for_2dGamesFinal/spawn.plist");
	spawningFrames = AnimationHelper::getAnimation("spawn%01d.png", 3);
	animationSpawn = Animation::createWithSpriteFrames(spawningFrames, 0.1);

	cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Art_for_2dGamesFinal/RightRoll.plist");
	rollingFrames = AnimationHelper::getAnimation("rightroll%01d.png", 3);
	animationRoll = Animation::createWithSpriteFrames(rollingFrames, 0.1);

	cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Hero/Hero_Idle/Hero_Idle.plist");
	rollingFrames = AnimationHelper::getAnimation("BlueKnight_entity_000_Idle_%03d.png", 10);

	stun = cocos2d::SpriteFrame::create("Art_for_2dGamesFinal/stun.png", cocos2d::Rect(0, 0, 39, 38));
	cocos2d::SpriteFrameCache::getInstance()->addSpriteFrame(stun, "stun");

	search = cocos2d::SpriteFrame::create("Art_for_2dGamesFinal/search.png", cocos2d::Rect(0, 0, 39, 38));
	cocos2d::SpriteFrameCache::getInstance()->addSpriteFrame(search, "search");


	idleFrames = AnimationHelper::getAnimation("search.png", 1);
	animationIdle = Animation::createWithSpriteFrames(idleFrames, 0.1);
	stun = cocos2d::SpriteFrame::create("Art_for_2dGamesFinal/stun.png", cocos2d::Rect(0, 0, 39, 38));
	cocos2d::SpriteFrameCache::getInstance()->addSpriteFrame(stun, "stun");

	stuntex->autorelease();
	stuntex->initWithImage(stunImage);*/
	
	myHealth.emptyBar->setPosition(this->getPositionX() + 19, this->getPositionY() + 42);
	myHealth.fillBar->setPosition(this->getPositionX() + 19, this->getPositionY() + 42);
	myHealth.currentHealth = health;
	myHealth.maxHealth = maxHealth;
	addChild(myHealth.emptyBar);
	addChild(myHealth.fillBar);

	particleSystem = ParticleSystemQuad::create("Particles/particle_texture.plist");
	addChild(particleSystem);
	particleSystem->stop();

	setState(MonsterState::Attack);
	scheduleUpdate();


	return true;
}


void Monster::setState(MonsterState state)
{
	if (currentState == state)
	{
		return;
	}

	exitCurrentState();
	currentState = state;
	enterNewState();
}


void Monster::update(float dt)
{
	switch (currentState)
	{
	case MonsterState::Idle:
		updateIdleState(dt);
		updateHealthBar();	
		break;
	case MonsterState::Attack:
		updateAttackState(dt);
		updateHealthBar();
		break;
	case MonsterState::Stun:
		updateStunState(dt);
		updateHealthBar();
		break;
	case MonsterState::Destroy:
		updateDestroyState(dt);
		break;
	}
	if (myHealth.currentHealth <= 0.0f)
	{
		setState(MonsterState::Destroy);
	}
}
void Monster::reset()
{
	setState(MonsterState::Idle);
}

void Monster::enterNewState()
{
	switch (currentState)
	{
	case MonsterState::Idle:

		//this->stopAllActions();
		//this->runAction(RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(spawningFrames, 0.1))));
		this->setTexture("Art_for_2dGamesFinal/search.png");
		// Enter Idle state
		break;
	case MonsterState::Attack:

		//this->stopAllActions();
		this->setTexture("Art_for_2dGamesFinal/attack.png");
		//this->runAction(RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(rollingFrames, 0.1))));
		timer = 3.0f;
		break;
	case MonsterState::Stun:

		timer = 3.0f;
		this->stopAllActions();
		this->setTexture("Art_for_2dGamesFinal/stun.png");
		mPhysicsBody->setVelocity(Vec2::ZERO);
		break;
	case MonsterState::Destroy:

		this->setTexture("Art_for_2dGamesFinal/ded.png");
		this->autorelease();
		this->removeAllChildrenWithCleanup(true);
		break;
	}
}

void Monster::exitCurrentState()
{
	switch (currentState)
	{
	case MonsterState::Idle:
		break;
	case MonsterState::Attack:
		break;
	case MonsterState::Stun:  
		break;
	case MonsterState::Destroy:
		break;
	}
}

void Monster::updateIdleState(float dt)
{
	if (heroref != nullptr)
	{
		if (heroref->getPositionX() < this->getPositionX() + 200 && heroref->getPositionX() > this->getPositionX() - 200)
		{
			setState(MonsterState::Attack);
		}
	}
}

void Monster::updateAttackState(float dt)
{
	// Update logic for Attack state

	if (heroref->getPositionX() > getPositionX())
	{
		if (isFlippedX())
		{
			setFlippedX(false);
		}
		if (mPhysicsBody->getVelocity().x < 256)
		{
			this->getPhysicsBody()->applyForce(Vec2(256, 0.0f));
		}
	}
	else
	{
		if (!isFlippedX())
		{
			setFlippedX(true);
		}
		if (mPhysicsBody->getVelocity().x > -256)
		{
			this->getPhysicsBody()->applyForce(Vec2(-256, 0.0f));
		}
	}

	timer -= dt;
	if (timer <= 0.0f)
	{
		setState(MonsterState::Stun);
	}

}

void Monster::updateStunState(float dt)
{
	timer -= dt;
	if (timer <= 0.0f)
	{
		setState(MonsterState::Idle);
	}
}

void Monster::updateDestroyState(float dt)
{

}

void Monster::initPhysics(TMXTiledMap* level)
{
	mPhysicsBody = cocos2d::PhysicsBody::createCircle(15, PHYSICSSHAPE_MATERIAL_DEFAULT, Vec2(18.5, 19));

	mPhysicsBody->setRotationEnable(false);
	mPhysicsBody->setDynamic(true);

	mPhysicsBody->setCategoryBitmask(1);
	mPhysicsBody->setCollisionBitmask(2);
	mPhysicsBody->setContactTestBitmask(1);
	this->setPhysicsBody(mPhysicsBody);
	mPhysicsBody->setPositionOffset(Vec2(-(attack->getContentSize().width), -(attack->getContentSize().height) + 20));
}


void Monster::updateHealthBar()
{
	const auto& posx = myHealth.emptyBar->getPositionX();
	const auto& posy = myHealth.emptyBar->getPositionY();
	
	myHealth.emptyBar->setScale(0.25);
	myHealth.fillBar->setScale(0.25);
	myHealth.fillBar->setScaleX((myHealth.currentHealth / myHealth.maxHealth) * 0.25f);
	myHealth.fillBar->setPosition(posx  * (myHealth.currentHealth / myHealth.maxHealth), posy);
}

MonsterPool::MonsterPool(int capacity)
{
	for (int i = 0; i < capacity; ++i)
	{
		Monster* monster = Monster::create();
		monster->retain();
		monsterPool.push_back(monster);
	}
}

Monster* MonsterPool::getMonster()
{
	if (!monsterPool.empty())
	{
		Monster* monster = monsterPool.back();
		monsterPool.pop_back();
		monster->reset();
		return monster;
	}
	return nullptr; 
}

void MonsterPool::returnMonster(Monster* monster)
{
	if (monster)
	{
		monsterPool.push_back(monster);
	}
}

