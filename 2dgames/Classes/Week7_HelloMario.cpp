#include "Week7_HelloMario.h"



Scene* HelloMario::createScene()
{
	auto ret = new (std::nothrow) HelloMario();
	if (ret && ret->initWithPhysics() && ret->init())
	{
		ret->autorelease();
		return ret;
	}
	else
	{
		CC_SAFE_DELETE(ret);
		return nullptr;
	}

	return ret;
}

bool HelloMario::init()
{
	if (!cocos2d::Scene::init())
	{
		return false;
	}


	auto visibleSize = _director->getVisibleSize();
	auto origin = _director->getVisibleOrigin();

	/// TODO Design your own level(s)
	auto level = TMXTiledMap::create("Mario/tmx/MarioSampleLevel.tmx");

	this->addChild(level);
	hero = Hero::Create();
	addChild(hero);
	hero->setPosition(hero->origin.x + hero->visibleSize.width / 2, hero->origin.y + hero->visibleSize.height / 2);
	hero->setScale(0.2);

	controller = KeyboardControllerComponent::create(KeyboardControllerComponent::ARROWS);
	hero->addComponent(controller);
	controller->initInput();
	Item newItem(GreenToken);

	myInventory.addItem("GreenToken");
	myInventory.addItem("GreenToken");
	addChild(&myInventory);

	InitPhysics(level);

	scheduleUpdate();

	return true;
}

void HelloMario::InitPhysics(TMXTiledMap* level)
{
	auto physicsWorld = getPhysicsWorld();
	physicsWorld->setGravity(cocos2d::Vec2(0, -980));

	heroPhysicsBody = cocos2d::PhysicsBody::createBox(cocos2d::Size(hero->getContentSize().width / 2, hero->getContentSize().height), PHYSICSSHAPE_MATERIAL_DEFAULT);

	heroPhysicsBody->setRotationEnable(false);
	heroPhysicsBody->setDynamic(true);
	heroPhysicsBody->setCategoryBitmask(1);
	heroPhysicsBody->setCollisionBitmask(2);
	heroPhysicsBody->setContactTestBitmask(2);
	hero->setPhysicsBody(heroPhysicsBody);
	hero->getPhysicsBody()->setLinearDamping(0.1);
	hero->getPhysicsBody()->setVelocityLimit(1024);
	//getPhysicsWorld()->setSpeed(1.5);
	contacts.reserve(4);

	auto collisionLayer = level->getLayer("Collision");
	for (int row = 0; row < level->getMapSize().height; ++row)
	{
		for (int col = 0; col < level->getMapSize().width; ++col)
		{
			auto tile = collisionLayer->getTileAt(cocos2d::Vec2(col, row));
			if (tile)
			{
				auto physicsBody = cocos2d::PhysicsBody::createBox(tile->getContentSize(), PHYSICSSHAPE_MATERIAL_DEFAULT);
				tile->setPhysicsBody(physicsBody);
				physicsBody->setDynamic(false);
				physicsBody->setCategoryBitmask(2);
				physicsBody->setCollisionBitmask(1);
				physicsBody->setContactTestBitmask(1);
			}
		}
	}

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = [=](PhysicsContact& contact) -> bool
	{
		auto a = contact.getShapeA()->getBody();
		auto b = contact.getShapeB()->getBody();

		auto other = hero->getPhysicsBody() == a ? b : a;

		if (hero->getPhysicsBody()->getPosition().y > other->getPosition().y && abs(contact.getContactData()->normal.y) > 0.9f)
		{
			contacts.push_back(other);
		}

		return true;
	};
	contactListener->onContactSeparate = [=](PhysicsContact& contact)
	{
		auto a = contact.getShapeA()->getBody();
		auto b = contact.getShapeB()->getBody();

		auto separate = hero->getPhysicsBody() == a ? b : a;

		for (int i = 0; i < contacts.size(); ++i)
		{
			if (contacts[i] == separate)
			{
				contacts[i] = contacts[contacts.size() - 1];
				contacts.pop_back();
				break;
			}
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

void HelloMario::update(float dt)
{
	auto contact = contacts.size() > 0;
	myInventory.displayInventory();

	if (contact) /// hero is on the ground
	{
		if (controller->IsRPressed())
		{
			auto physicsWorld = getPhysicsWorld();
			if (physicsWorld->getDebugDrawMask() == PhysicsWorld::DEBUGDRAW_ALL)
			{
				physicsWorld->setDebugDrawMask(NULL);
			}
			else
			{
				physicsWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
			}
		}
		if (controller->IsLeftPressed())
		{
			hero->getPhysicsBody()->setVelocity(Vec2(-hero->mMoveSpeed, 0));
			//marioPhysicsBody->applyForce(Vec2(-512, 0));
			hero->setFlippedX(false);
		}
		else if (controller->IsRightPressed())
		{
			hero->getPhysicsBody()->setVelocity(Vec2(hero->mMoveSpeed, 0));
			//marioPhysicsBody->applyForce(Vec2(512, 0));
			hero->setFlippedX(true);
		}
		if (animationState != Walking && hero->getPhysicsBody()->getVelocity().x != 0)
		{
			animationState = Walking;
			hero->stopAllActions();
			hero->runAction(RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(hero->walkingFrames, 0.1))));
		}
		if (controller->IsDownPressed())
		{

		}

		if (controller->IsUpPressed())
		{
			hero->getPhysicsBody()->applyImpulse(Vec2(0, 450), Vec2::ZERO);
		}

		if (animationState != Idle && !controller->IsLeftPressed() && !controller->IsRightPressed() && !controller->IsDownPressed() && !controller->IsUpPressed())
		{
			animationState = Idle;
			hero->stopAllActions();
			hero->runAction(RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(hero->idleFrames, 0.1))));
			hero->getPhysicsBody()->setVelocity(Vec2::ZERO);
		}
	}
	else
	{
		if (controller->IsLeftPressed())
		{
			//marioPhysicsBody->setVelocity(Vec2(-256, marioPhysicsBody->getVelocity().y));
			if (hero->getPhysicsBody()->getVelocity().x > -512)
			{
				hero->getPhysicsBody()->applyForce(Vec2(-1024, hero->getPhysicsBody()->getVelocity().y));
			}
			hero->setFlippedX(false);
			/// TODO:
			/// Move mario left with some velocity
			/// Set Position, and Flip X scale to negative
		}
		else if (controller->IsRightPressed())
		{
			//marioPhysicsBody->setVelocity(Vec2(256, marioPhysicsBody->getVelocity().y));
			if (hero->getPhysicsBody()->getVelocity().x < 512)
			{
				hero->getPhysicsBody()->applyForce(Vec2(1024, hero->getPhysicsBody()->getVelocity().y));
			}
			hero->setFlippedX(true);
			/// TODO:
			/// Move mario right with some velocity
			/// Set Position, and Flip X scale to positive
		}

		if (animationState != Falling && hero->getPhysicsBody()->getVelocity().y < 0)
		{
			/// TODO:
			/// change animation to falling animation
			animationState = Falling;
			hero->stopAllActions();
			hero->runAction(RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(hero->fallFrames, 0.1))));

		}
		if (animationState != Jumping && hero->getPhysicsBody()->getVelocity().y > 0)
		{
			/// TODO:
			/// change animation to falling animation
			animationState = Jumping;
			hero->stopAllActions();
			hero->runAction(RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(hero->jumpFrames, 0.1))));
		}
		if (animationState != Idle && hero->getPhysicsBody()->getVelocity() == Vec2::ZERO)
		{
			/// TODO:
			animationState = Idle;
			hero->stopAllActions();
			hero->runAction(RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(hero->idleFrames, 0.1))));
			/// change animation to falling animation
		}
		if (animationState != Walking && hero->getPhysicsBody()->getVelocity().y == 0 && hero->getPhysicsBody()->getVelocity().x != 0)
		{
			/// TODO:
			animationState = Walking;
			hero->stopAllActions();
			hero->runAction(RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(hero->walkingFrames, 0.1))));
			/// change animation to falling animation
		}
	}

	/// TODO:
	/// Set camera to follow mario
}