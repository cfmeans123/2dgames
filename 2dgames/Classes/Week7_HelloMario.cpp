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

	addChild(background);
	background->setPosition(background->getContentSize().width / 2, background->getContentSize().height / 2);

	this->addChild(level);
	hero = Hero::Create();
	addChild(hero);
	hero->setPosition(hero->origin.x + hero->visibleSize.width / 2, hero->origin.y + hero->visibleSize.height / 2);
	

	//attempt to create container for initPhysics functions...
	//std::function<void(TMXTiledMap*)> funcpointer = std::bind(&Hero::initPhysics, hero, _1);
	//const auto& monster = monsterpool->getMonster();

	////phyInitList.pushBack(funcpointer);

	//this->addChild(monster);
	//if (monster->heroref == nullptr)
	//{
	//	monster->heroref = hero;
	//}


	controller = KeyboardControllerComponent::create(KeyboardControllerComponent::WASD);
	hero->addComponent(controller);
	controller->initInput();
	//Item newItem(GreenToken);
	initPauseMenu();

	addChild(&myInventory);

	InitPhysics(level);

	scheduleUpdate();

	return true;
}

void HelloMario::InitPhysics(TMXTiledMap* level)
{
	if (!menu->isVisible())
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
		auto groundPhysicsBody = cocos2d::PhysicsBody::createBox(Size(6240, 32), PHYSICSSHAPE_MATERIAL_DEFAULT, Vec2(800, -50));
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
}

void HelloMario::update(float dt)
{

	if (controller->IsEscapePressed())
	{
		controller->escape = false;
		if (menu->isVisible())
		{
			hidePauseMenu();
		}
		else
		{
			showPauseMenu();
		}
	}
	if (!menu->isVisible())
	{
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

		if (controller->IsLeftClickPressed())
		{

		}


		auto contact = contacts.size() > 0;
		if (contact) /// hero is on the ground
		{
			//If not affected by combat actions, allow movement state logic
			if (hero->mCombatState == CombatState::None)
			{
				if (hero->mMoveState == Jump)
				{
					if (!controller->IsRightPressed() && !controller->IsLeftPressed())
					{
						hero->setMoveState(MoveState::Idle);
					}
					else
					{
						if (controller->IsShiftPressed())
						{
							hero->setMoveState(MoveState::Sprint);
						}
						else
						{
							hero->setMoveState(MoveState::Walk);
						}
					}
				}
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
				if (controller->IsShiftPressed())
				{
					if (hero->mMoveState != MoveState::Sprint)
					{
						hero->setMoveState(MoveState::Sprint);
					}

				}
				else if (controller->shiftkey == false)
				{
					if (hero->mMoveState == MoveState::Sprint)
					{
						if (controller->IsLeftPressed() || controller->IsRightPressed())
						{
							hero->setMoveState(MoveState::Walk);
						}
						else
						{
							hero->setMoveState(MoveState::Idle);
						}
					}
				}
				if (controller->IsLeftPressed())
				{
					if (hero->getPhysicsBody()->getVelocity().x > 0)
					{
						hero->getPhysicsBody()->setVelocity(Vec2::ZERO);
					}
					if (hero->mMoveState != MoveState::Sprint)
					{
						hero->setMoveState(MoveState::Walk);
					}
					if (hero->getPhysicsBody()->getVelocity().x > -hero->mMoveSpeed)
					{
						hero->getPhysicsBody()->applyForce(Vec2(-hero->mMoveSpeed, hero->getPhysicsBody()->getVelocity().y));
					}
					hero->setFlippedX(false);
					if (hero->getChildByName("CollisionVolume") != NULL)
					{
						hero->getChildByName("CollisionVolume")->getChildByName("shape")->setPosition(Vec2(hero->getPositionX() - 200, hero->getPositionY()));
					}

				}
				else if (controller->IsRightPressed())
				{
					if (hero->getPhysicsBody()->getVelocity().x < 0)
					{
						hero->getPhysicsBody()->setVelocity(Vec2::ZERO);
					}
					if (hero->mMoveState != MoveState::Sprint)
					{
						hero->setMoveState(MoveState::Walk);
					}
					if (hero->getPhysicsBody()->getVelocity().x < hero->mMoveSpeed)
					{
						hero->getPhysicsBody()->applyForce(Vec2(hero->mMoveSpeed, hero->getPhysicsBody()->getVelocity().y));
					}
					hero->setFlippedX(true);
					if (hero->getChildByName("CollisionVolume") != NULL)
					{
						hero->getChildByName("CollisionVolume")->getChildByName("shape")->setPosition(Vec2(hero->getPositionX() + 200, hero->getPositionY()));
					}
					//hero->collisionVolume.
					//hero->collisionVolume.attackRange = -1 * (hero->collisionVolume.attackRange);
				}

				if (controller->IsUpPressed())
				{
					hero->setMoveState(MoveState::Jump);
				}

				if (hero->mMoveState != MoveState::Idle && !controller->IsLeftPressed() && !controller->IsRightPressed() && !controller->IsUpPressed())
				{
					hero->setMoveState(MoveState::Idle);
					//hero->getPhysicsBody()->setVelocity(Vec2(0.0f, hero->getPhysicsBody()->getVelocity().y));
				}
				
			}
			else
			{

			}
		}
		if (controller->IsLeftClickPressed())
		{
			hero->setCombatState(CombatState::Attack);
		}
		auto& position = hero->getPosition();
		if (position.x > 600 && position.x < 5700)
		{
			this->_defaultCamera->setPosition(position.x, _director->getVisibleSize().height / 2);
		}

	}
}

void HelloMario::initPauseMenu()
{
	// Create "New Game" button
	auto newGameButton = cocos2d::MenuItemLabel::create(cocos2d::Label::createWithTTF("New Game", "fonts/Marker Felt.ttf", 24), [](cocos2d::Ref* sender)
		{
			Director::getInstance()->replaceScene(createScene());
		});

	
	/// TODO: Create Quit button
	auto newGameButton4 = cocos2d::MenuItemLabel::create(cocos2d::Label::createWithTTF("Quit", "fonts/Marker Felt.ttf", 24), [=](cocos2d::Ref* sender)
		{
			Director::getInstance()->end();
		});
	/// TODO: add buttons to menu

	// TODO: add buttons into menu below
	menu = cocos2d::Menu::create(newGameButton, nullptr);
	menu->addChild(newGameButton4);
	menu->alignItemsVerticallyWithPadding(20);

	// Add the menu to the scene
	this->addChild(menu);

	hidePauseMenu();
}

void HelloMario::showPauseMenu()
{
	getPhysicsWorld()->setSpeed(0.0f);
	auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	auto pos = Vec2{ cocos2d::Director::getInstance()->getVisibleSize() * 0.5f } + origin;

	Vec2 screenCenter = Director::getInstance()->getVisibleSize() * 0.5f;

	// Get the camera's position
	Vec2 cameraPosition = _defaultCamera->getPosition();

	// Calculate the offset for the menu's position
	Vec2 menuOffset = cameraPosition - screenCenter;

	/// TODO: Pause Physics from updating

	menu->setPosition(pos + menuOffset);
	menu->setVisible(true);
}

void HelloMario::hidePauseMenu()
{
	/// TODO: Resume Physics from updating
	/// TODO: Hide menu
	getPhysicsWorld()->setSpeed(1.0f);
	menu->setVisible(false);
}

bool isEnemyWithinCollisionVolume(cocos2d::Node* enemy, CollisionVolume* collisionVolume)
{
	if (!enemy || !collisionVolume) return false;


	// Get the world coordinates of the collision volume
	auto collisionVolumeWorldPos = collisionVolume->getParent()->convertToWorldSpace(collisionVolume->getPosition());

	// Check if the enemy node is within the collision volume
	return enemy->getBoundingBox().containsPoint(collisionVolumeWorldPos);
}