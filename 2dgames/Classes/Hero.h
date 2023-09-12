#pragma once

#include "cocos2d.h"
#include "CollisionVolume.h"
#include "KeyboardControllerComponent.h"


USING_NS_CC;


enum CombatState
{
	Attack,
	None,
	Stun
};

enum MoveState
{
	Idle,
	Jump,
	Walk,
	Sprint,
	Off
};

class Hero : public Sprite
{
public:
	static Hero* Create();
	void initPhysics(TMXTiledMap* level);

	//Hero(Hero &h);

	PhysicsBody* physicsBody;
	PhysicsShapeBox* attackCollisionMask;
	float spriteHeight;
	float spriteWidth;

	cocos2d::Size visibleSize = _director->getVisibleSize();
	cocos2d::Vec2 origin = _director->getVisibleOrigin();
	

	Animation* animationIdle;
	Animation* animationWalk;
	Animation* animationFall;
	Animation* animationJump;
	Animation* animationDeathOne;
	Animation* animationDeathTwo;
	Animation* animationAttackOne;
	Animation* animationAttackTwo;
	Animation* animationHurrah;

	Sprite* mario;
	//PhysicsBody* heroPhysicsBody;
	std::vector<PhysicsBody*> contacts;


	Vector<SpriteFrame*> walkingFrames;
	Vector<SpriteFrame*> idleFrames;
	Vector<SpriteFrame*> jumpFrames;
	Vector<SpriteFrame*> fallFrames;
	Vector<SpriteFrame*> runningFrames;
	Vector<SpriteFrame*> attack1Frames;
	Vector<SpriteFrame*> attack2Frames;
	Vector<SpriteFrame*> die1Frames;
	Vector<SpriteFrame*> die2Frames;
	Vector<SpriteFrame*> hurrahFrames;
	SpriteFrame* hitFrontFrame;
	SpriteFrame* hitBackFrame;

	MoveState mMoveState;
	CombatState mCombatState;


	//movement
	float defaultMoveSpeed = 256;
	float sprintMoveSpeed = 512;
	float mMoveSpeed = 256;

	//State Machine
	void setCombatState(CombatState state);
	void setMoveState(MoveState state);
	void update(float dt) override;
	// Reset the enemy to its initial state
	void reset();
	float stunTimer = 3.0f;
	CollisionVolume collisionVolume;

	float frameSpeed = 0.1f;
	float sprintFrameSpeed = 0.05f;
	void toggleCollisionVolume();
private:
	void enterNewCombatState();
	void enterNewMoveState();

	void exitCurrentCombatState();
	void exitCurrentMoveState();
	void updateNoneState(float dt);
	void updateAttackState(float dt);
	void updateStunState(float dt);	
};





