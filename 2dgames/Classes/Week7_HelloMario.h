#pragma once

#include "cocos2d.h"
#include "KeyboardControllerComponent.h"
#include "Inventory.h"
#include "RolyPoly.h"


USING_NS_CC;

class Hero;

class HelloMario : public Scene
{
public:
	static Scene* createScene();

	virtual bool init();

	virtual void update(float dt);

	Hero* hero = nullptr;
	Sprite* background;
	CREATE_FUNC(HelloMario)
	MonsterPool* monsterpool = new MonsterPool(1);

private:

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
	PhysicsBody* heroPhysicsBody;
//	cocos2d::Vector<std::function<void(TMXTiledMap*)>> phyInitList;
	//void (*hero_ptr)(TMXTiledMap*);

	Inventory myInventory;
	//Item myItem;


	std::vector<PhysicsBody*> contacts;
	KeyboardControllerComponent* controller;
	bool contact = false;
};