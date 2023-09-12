#include "RolyPoly.h"


bool Monster::init()
{
	if (!Sprite::init())
	{
		return false;
	}
	setScale(2);
	// Initialize enemy sprite and other properties here
	cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Art_for_2dGamesFinal/spawn.plist");
	spawningFrames = AnimationHelper::getAnimation("spawn%01d.png", 3);
	//animationSpawn = Animation::createWithSpriteFrames(spawningFrames, 0.1);

	cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Art_for_2dGamesFinal/RightRoll.plist");
	rollingFrames = AnimationHelper::getAnimation("rightroll%01d.png", 3);
	//animationRoll = Animation::createWithSpriteFrames(rollingFrames, 0.1);

	//cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Hero/Hero_Idle/Hero_Idle.plist");
	//rollingFrames = AnimationHelper::getAnimation("BlueKnight_entity_000_Idle_%03d.png", 10);

	stun = Sprite::create("Art_for_2dGamesFinal/stun.png");
	search = Sprite::create("Art_for_2dGamesFinal/search.png");
	attack = Sprite::create("Art_for_2dGamesFinal/attack.png");

	//stun = cocos2d::SpriteFrame::create("Art_for_2dGamesFinal/stun.png", cocos2d::Rect(0, 0, 39, 38));
	//cocos2d::SpriteFrameCache::getInstance()->addSpriteFrame(stun, "stun");

	//search = cocos2d::SpriteFrame::create("Art_for_2dGamesFinal/search.png", cocos2d::Rect(0, 0, 39, 38));
	//cocos2d::SpriteFrameCache::getInstance()->addSpriteFrame(search, "search");


	//idleFrames = AnimationHelper::getAnimation("search.png", 1);
	//animationIdle = Animation::createWithSpriteFrames(idleFrames, 0.1);
	//stun = cocos2d::SpriteFrame::create("Art_for_2dGamesFinal/stun.png", cocos2d::Rect(0, 0, 39, 38));
	//cocos2d::SpriteFrameCache::getInstance()->addSpriteFrame(stun, "stun");

	//stuntex->autorelease();
	//stuntex->initWithImage(stunImage);
	
	myHealth.emptyBar->setPosition(this->getPositionX() + 19, this->getPositionY() + 42);
	myHealth.fillBar->setPosition(this->getPositionX() + 19, this->getPositionY() + 42);
	myHealth.currentHealth = health;
	myHealth.maxHealth = maxHealth;
	addChild(myHealth.emptyBar);
	addChild(myHealth.fillBar);


	// Set initial state to Idle
	setState(MonsterState::Attack);
	scheduleUpdate();
	//this->_offsetPosition = Vec2(19.5, 19);

	// Schedule update method

	return true;
}


void Monster::setState(MonsterState state)
{
	if (currentState == state)
	{
		return; // No change in state
	}

	exitCurrentState();
	currentState = state;
	enterNewState();
}


void Monster::update(float dt)
{

	//mPhysicsBody->setVelocity(Vec2(0.0f, 0.0f));
	// Update logic for the current state
	Vec2 velocity = this->getPhysicsBody()->getVelocity();
	updateHealthBar();
	//getPhysicsBody()->setVelocity(Vec2(0.0f, 0.0f));
	switch (currentState)
	{
	case MonsterState::Idle:
		updateIdleState(dt);
		break;
	case MonsterState::Attack:
		updateAttackState(dt);
		break;
	case MonsterState::Stun:
		updateStunState(dt);
		break;
	case MonsterState::Destroy:
		updateDestroyState(dt);
		break;
	}
}
void Monster::reset()
{
	setState(MonsterState::Idle);
	// Reset other properties of the enemy here
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
		// Enter Attack state
		//this->stopAllActions();
		this->setTexture("Art_for_2dGamesFinal/attack.png");
		//this->runAction(RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(rollingFrames, 0.1))));
		timer = 3.0f;
		break;
	case MonsterState::Stun:
		// Enter Stun state
		timer = 3.0f;
		this->stopAllActions();
		this->setTexture("Art_for_2dGamesFinal/stun.png");
		mPhysicsBody->setVelocity(Vec2::ZERO);

		//this->setSpriteFrame(stun);
		break;
	case MonsterState::Destroy:
		// Enter Destroy state
		break;
	}
}

void Monster::exitCurrentState()
{
	switch (currentState)
	{
	case MonsterState::Idle:
		// Exit Idle state
		break;
	case MonsterState::Attack:
		// Exit Attack state
		break;
	case MonsterState::Stun:
		// Exit Stun state      
		break;
	case MonsterState::Destroy:
		// Exit Destroy state
		break;
	}
}

void Monster::updateIdleState(float dt)
{
	// Update logic for Idle state
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
	// Update logic for Stun state
	timer -= dt;
	if (timer <= 0.0f)
	{
		setState(MonsterState::Idle);
	}
}

void Monster::updateDestroyState(float dt)
{
	// Update logic for Destroy state
}

void Monster::initPhysics(TMXTiledMap* level)
{
	mPhysicsBody = cocos2d::PhysicsBody::createCircle(15, PHYSICSSHAPE_MATERIAL_DEFAULT, Vec2(18.5, 19));

	mPhysicsBody->setRotationEnable(false);
	mPhysicsBody->setDynamic(true);

	mPhysicsBody->setCategoryBitmask(1);
	mPhysicsBody->setCollisionBitmask(2);
	mPhysicsBody->setContactTestBitmask(1);
	//heroPhysicsBody->setMass(5.0f);
	this->setPhysicsBody(mPhysicsBody);
	mPhysicsBody->setPositionOffset(Vec2(-(attack->getContentSize().width), -(attack->getContentSize().height) + 20));
	//getPhysicsBody()->setLinearDamping(0.1);
	//getPhysicsBody()->setVelocityLimit(1);
	//mPhysicsBody->setVelocity(Vec2(0.0f, 0.0f));
}


void Monster::updateHealthBar()
{
	myHealth.emptyBar->setScale(0.25);
	myHealth.fillBar->setScale(0.25);
	myHealth.fillBar->setScaleX((myHealth.currentHealth / myHealth.maxHealth) * 0.25f);
	const auto& posx = myHealth.emptyBar->getPositionX();
	const auto& posy = myHealth.emptyBar->getPositionY();

	//myHealth.emptyBar->setPosition(myHealth.emptyBar->getPositionX(), this->getPositionY() + 50);
	myHealth.fillBar->setPosition(posx  * (myHealth.currentHealth / myHealth.maxHealth), posy);
}

MonsterPool::MonsterPool(int capacity)
{
	for (int i = 0; i < capacity; ++i)
	{
		Monster* monster = Monster::create();
		monster->retain(); // Retain to prevent auto-release
		monsterPool.push_back(monster);
	}
}

// Get an enemy from the pool
Monster* MonsterPool::getMonster()
{
	if (!monsterPool.empty())
	{
		Monster* monster = monsterPool.back();
		monsterPool.pop_back();
		monster->reset(); // Reset the enemy's state and properties
		return monster;
	}
	return nullptr; // Pool is empty
}

// Return an enemy to the pool
void MonsterPool::returnMonster(Monster* monster)
{
	if (monster)
	{
		monsterPool.push_back(monster);
	}
}

