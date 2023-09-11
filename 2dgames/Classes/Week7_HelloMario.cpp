#include "Week7_HelloMario.h"
#include <functional>
using namespace std::placeholders;


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
	auto level = TMXTiledMap::create("Art_for_2dGamesFinal/ForestMap.tmx");
	
	background = Sprite::create("Art_for_2dGamesFinal/ForestBackground.png");
	background->setScale(2);
	background->setPositionX(200);

	//background->initWithFile("Art_for_2dGamesFinal/ForestBackground.png");
	addChild(background);
	//background->
	background->setPosition(background->getContentSize().width / 2, background->getContentSize().height / 2);

	this->addChild(level);
	hero = Hero::Create();
	addChild(hero);
	hero->setPosition(hero->origin.x + hero->visibleSize.width / 2, hero->origin.y + hero->visibleSize.height / 2);
	hero->setScale(0.2);
	
	
	//attempt to create container for initPhysics functions...
	//std::function<void(TMXTiledMap*)> funcpointer = std::bind(&Hero::initPhysics, hero, _1);
	//const auto& monster = monsterpool->getMonster();

	////phyInitList.pushBack(funcpointer);

	//this->addChild(monster);
	//if (monster->heroref == nullptr)
	//{
	//	monster->heroref = hero;
	//}


	controller = KeyboardControllerComponent::create(KeyboardControllerComponent::ARROWS);
	hero->addComponent(controller);
	controller->initInput();
	//Item newItem(GreenToken);

	addChild(&myInventory);

	InitPhysics(level);

	scheduleUpdate();

	return true;
}

void HelloMario::InitPhysics(TMXTiledMap* level)
{
	auto physicsWorld = getPhysicsWorld();
	getPhysicsWorld()->setFixedUpdateRate(60);
	hero->initPhysics(level);
	int displace = 20;
	for (auto& monster : monsterpool->getMonsterPool())
	{
		monster->setPosition(hero->getPositionX() + displace, hero->getPositionY());
		this->addChild(monster);
		if (monster->heroref == nullptr)
		{
			monster->heroref = hero;
		}
		displace += 55;
		monster->initPhysics(level);
	}
	physicsWorld->setGravity(cocos2d::Vec2(0, -980));

	contacts.reserve(4);

	auto collisionLayer = level->getLayer("Collision");
	
	//mimicing composite collision volume through hard coded position values
	auto groundPhysicsBody = cocos2d::PhysicsBody::createBox(Size( 6240, 32 ), PHYSICSSHAPE_MATERIAL_DEFAULT, Vec2(800, -50));
	auto tile = collisionLayer->getTileAt(cocos2d::Vec2(73, 13));
	tile->setPhysicsBody(groundPhysicsBody);
	groundPhysicsBody->setDynamic(false);
	groundPhysicsBody->setCategoryBitmask(2);
	groundPhysicsBody->setCollisionBitmask(1);
	groundPhysicsBody->setContactTestBitmask(1);

	auto frontBoundPhysicsBody = cocos2d::PhysicsBody::createBox(Size(32, 512), PHYSICSSHAPE_MATERIAL_DEFAULT, Vec2(0, 200));
	auto tile2 = collisionLayer->getTileAt(cocos2d::Vec2(0, 14));
	tile2->setPhysicsBody(frontBoundPhysicsBody);
	frontBoundPhysicsBody->setDynamic(false);
	frontBoundPhysicsBody->setCategoryBitmask(2);
	frontBoundPhysicsBody->setCollisionBitmask(1);
	frontBoundPhysicsBody->setContactTestBitmask(1);
	
	auto backBoundPhysicsBody = cocos2d::PhysicsBody::createBox(Size(32, 512), PHYSICSSHAPE_MATERIAL_DEFAULT, Vec2(6240, 200));
	auto tile3 = collisionLayer->getTileAt(cocos2d::Vec2(1, 14));
	tile3->setPhysicsBody(backBoundPhysicsBody);
	backBoundPhysicsBody->setDynamic(false);
	backBoundPhysicsBody->setCategoryBitmask(2);
	backBoundPhysicsBody->setCollisionBitmask(1);
	backBoundPhysicsBody->setContactTestBitmask(1);

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
	
	
	if (controller->IsOnePressed())
	{
		controller->one = false;
		myInventory.addItem("GreenToken");
	}
	if (controller->IsTwoPressed())
	{
		controller->two = false;
		myInventory.addItem("PurpleToken");
	}
	if (controller->IsThreePressed())
	{
		controller->three = false;
		myInventory.removeItem("GreenToken");
	}
	if (controller->IsFourPressed())
	{
		/*controller->four = false;
		myInventory.removeItem("PurpleToken");	*/
		for (auto& monster : monsterpool->getMonsterPool())
		{
			monster->getPhysicsBody()->setDynamic(!monster->getPhysicsBody()->isDynamic());
		}

	}
	myInventory.displayInventory(hero);



	if (contact) /// hero is on the ground
	{
		if (controller->IsRPressed())
		{
			controller->rkey = false;
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
			//hero->getPhysicsBody()->setVelocity(Vec2(-hero->mMoveSpeed, 0));
			//marioPhysicsBody->applyForce(Vec2(-512, 0));
			if (hero->getPhysicsBody()->getVelocity().x > -512)
			{
				hero->getPhysicsBody()->applyForce(Vec2(-1024, hero->getPhysicsBody()->getVelocity().y));
				//hero->getPhysicsBody()->applyImpulse(Vec2(-1024, 0), Vec2::ZERO);
			}
			hero->setFlippedX(false);
		}
		else if (controller->IsRightPressed())
		{
			if (hero->getPhysicsBody()->getVelocity().x < 512)
			{
				hero->getPhysicsBody()->applyForce(Vec2(1024, hero->getPhysicsBody()->getVelocity().y));
				//hero->getPhysicsBody()->applyImpulse(Vec2(1024, 0), Vec2::ZERO);
			}
			//hero->getPhysicsBody()->setVelocity(Vec2(hero->mMoveSpeed, 0));
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
			hero->getPhysicsBody()->setVelocity(Vec2(0.0f, hero->getPhysicsBody()->getVelocity().y));
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

	auto& position = hero->getPosition();
	if (position.x > 600 && position.x < 5700)
	this->_defaultCamera->setPosition(position.x, _director->getVisibleSize().height/2 );
	/// TODO
	/// Set camera to follow mario
}