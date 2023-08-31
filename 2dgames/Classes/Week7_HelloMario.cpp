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

  SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Hero/Hero_Walk/Hero_Walk.plist");
  walkingFrames = getAnimation("BlueKnight_entity_000_walk_%03d.png", 9);
  SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Hero/Hero_Idle/Hero_Idle.plist");
  idleFrames = getAnimation("BlueKnight_entity_000_Idle_%03d.png", 9);
  SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Hero/Hero_Jump/Hero_Jump.plist");
  fallingFrames = getAnimation("BlueKnight_entity_000_jump_004.png", 1);
  jumpingFrames = getAnimation("BlueKnight_entity_000_jump_%03d.png", 1);

  
  mario = Sprite::createWithSpriteFrame(idleFrames.front());
  addChild(mario);
  mario->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
  mario->setScale(0.3);

  animation = Animation::createWithSpriteFrames(idleFrames, 1.0f);
  animationWalk = Animation::createWithSpriteFrames(walkingFrames, 1.0f);
  animationFall = Animation::createWithSpriteFrames(fallingFrames, 1.0f);
  animationJump = Animation::createWithSpriteFrames(jumpingFrames, 1.0f);
  
  mario->runAction(RepeatForever::create(Animate::create(animation)));

  controller = KeyboardControllerComponent::create(KeyboardControllerComponent::ARROWS);
  mario->addComponent(controller);
  controller->initInput();


  InitPhysics(level);

  scheduleUpdate();

  return true;
}

void HelloMario::InitPhysics(TMXTiledMap* level)
{
  auto physicsWorld = getPhysicsWorld();
  physicsWorld->setGravity(cocos2d::Vec2(0, -980));

  ///TODO: make code below toggleable (aka can turn on and off debug draw) 


 

  marioPhysicsBody = cocos2d::PhysicsBody::createBox(mario->getContentSize(), PHYSICSSHAPE_MATERIAL_DEFAULT);
  ///TODO: Disallow mario's rotation to be effected by physics
  ///TODO: Set mario to be dynamic (physics is applied to it)
  ///TODO: Set mario physics body's category bitmask
  ///TODO: Set mario physics body's collision bitmask
  ///TODO: Set mario physics body's contact test bitmask
  marioPhysicsBody->setRotationEnable(false);
  marioPhysicsBody->setDynamic(true);
  marioPhysicsBody->setCategoryBitmask(1);
  marioPhysicsBody->setCollisionBitmask(2);
  marioPhysicsBody->setContactTestBitmask(2);
  mario->setPhysicsBody(marioPhysicsBody);

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

    auto other = marioPhysicsBody == a ? b : a;

    if (marioPhysicsBody->getPosition().y > other->getPosition().y && abs(contact.getContactData()->normal.y) > 0.9f)
    {
      contacts.push_back(other);
    }

    return true;
  };
  contactListener->onContactSeparate = [=](PhysicsContact& contact)
  {
    auto a = contact.getShapeA()->getBody();
    auto b = contact.getShapeB()->getBody();

    auto separate = marioPhysicsBody == a ? b : a;

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

 
  if (contact) /// mario is on the ground
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
        marioPhysicsBody->setVelocity(Vec2(-256, 0));
        mario->setFlippedX(false);
      /// TODO:
      /// Move mario left with some velocity
      /// Set Position, and Flip X scale to negative

      /// Check if not in walking state + switch to walking animation
    }
    else if (controller->IsRightPressed())
    {
        marioPhysicsBody->setVelocity(Vec2(256, 0));
        mario->setFlippedX(true);
      /// TODO:
      /// Move mario right with some velocity
      /// Set Position, and Flip X scale to positive

      /// check if not walking + switch to walking
    }
    
    if (controller->IsDownPressed())
    {

      /// TODO (optional):
      /// Make mario crouch
    }
    
    if (controller->IsUpPressed())
    {
        marioPhysicsBody->applyImpulse(Vec2(0, 800), Vec2::ZERO);
      /// TODO:
      ///Check if not in jumping state
        /// Apply an impulse force to mario to make him jump
        /// switch to jumping animation / sprite
    }

    if (!controller->IsLeftPressed() && !controller->IsRightPressed() && !controller->IsDownPressed() && !controller->IsUpPressed())
    {
      /// TODO:
      /// switch to idle animation
        mario->getPhysicsBody()->setVelocity(Vec2::ZERO);
    }
  }
  else
  {
    if (controller->IsLeftPressed())
    {
        marioPhysicsBody->setVelocity(Vec2(-256, marioPhysicsBody->getVelocity().y));
        mario->setFlippedX(false);
      /// TODO:
      /// Move mario left with some velocity
      /// Set Position, and Flip X scale to negative
    }
    else if (controller->IsRightPressed())
    {
        marioPhysicsBody->setVelocity(Vec2(256, marioPhysicsBody->getVelocity().y));
        mario->setFlippedX(true);
      /// TODO:
      /// Move mario right with some velocity
      /// Set Position, and Flip X scale to positive
    }

    if (animationState != Falling && mario->getPhysicsBody()->getVelocity().y < 0)
    {
      /// TODO:
      /// change animation to falling animation
      animationState = Falling;
      mario->stopAllActions();
      mario->runAction(RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(fallingFrames, 1.0f))));
      
    }
    if (animationState != Idle && mario->getPhysicsBody()->getVelocity() == Vec2::ZERO)
    {
        /// TODO:
        animationState = Idle;
        mario->stopAllActions();
        mario->runAction(RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(idleFrames, 1.0f))));
        /// change animation to falling animation
    }
    if (animationState != Walking && mario->getPhysicsBody()->getVelocity().x != 0)
    {
        /// TODO:
        animationState = Walking;
        mario->stopAllActions();
        //mario->runAction(RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(walkingFrames, 10.0f))));
        /// change animation to falling animation
    }
    if (animationState != Jumping && mario->getPhysicsBody()->getVelocity().y > 0)
    {
        /// TODO:
        /// change animation to falling animation
        animationState = Jumping;
        mario->stopAllActions();
        mario->runAction(RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(jumpingFrames, 1.0f))));
    }
  }

  /// TODO:
  /// Set camera to follow mario
}