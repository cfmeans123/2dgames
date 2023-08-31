#pragma once

#include "cocos2d.h"
#include "KeyboardControllerComponent.h"

USING_NS_CC;

class HelloMario : public Scene
{
public:
  static Scene* createScene();

  virtual bool init();

  virtual void update(float dt);

  CREATE_FUNC(HelloMario)
private:
  static Vector<SpriteFrame*> getAnimation(const char* format, int count)
  {
    auto spritecache = SpriteFrameCache::getInstance();
    Vector<SpriteFrame*> animFrames;
    char str[1000];
    for (int i = 1; i <= count; i++)
    {
      snprintf(str, sizeof(str), format, i);
      animFrames.pushBack(spritecache->getSpriteFrameByName(str));
    }
    return animFrames;
  }

  void InitPhysics(TMXTiledMap* map);

  enum AnimationState
  {
    Idle,
    Walking,
    Jumping,
    Falling
  } animationState = Idle;

  Vector<SpriteFrame*> idleFrames;
  Vector<SpriteFrame*> walkingFrames;
  Vector<SpriteFrame*> jumpingFrames;
  Vector<SpriteFrame*> fallingFrames;

  Animation* animation;
  Animation* animationWalk;
  Animation* animationFall;
  Animation* animationJump;
  Sprite* mario;
  PhysicsBody* marioPhysicsBody;
  std::vector<PhysicsBody*> contacts;
  KeyboardControllerComponent* controller;
  bool contact = false;
};