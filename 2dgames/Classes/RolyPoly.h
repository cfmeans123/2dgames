#pragma once
#include "Healthbar.h"
#include "AnimationHelper.h"
#include "Hero.h"
#include <vector>

enum class MonsterState 
{
    Idle,
    Attack,
    Stun,
    Destroy
};

class Monster : public cocos2d::Sprite 
{
public:
    CREATE_FUNC(Monster);

    virtual bool init() override;

    // State machine methods
    void setState(MonsterState state);
    void update(float dt) override;
    // Reset the enemy to its initial state
    void reset();
    //cocos2d::SpriteFrame* search;
    //cocos2d::SpriteFrame* stun;
    cocos2d::Animation* animationSpawn;
    cocos2d::Animation* animationRoll;
    cocos2d::Animation* animationStun;
    cocos2d::Animation* animationIdle;
    cocos2d::Vector<cocos2d::SpriteFrame*> rollingFrames;
    cocos2d::Vector<cocos2d::SpriteFrame*> spawningFrames;
    cocos2d::Vector<SpriteFrame*> stunFrames;
    cocos2d::Vector<SpriteFrame*> idleFrames;
    Sprite* search;
    Sprite* stun;
    Sprite* attack;
    cocos2d::Texture2D* stuntex;
    cocos2d::Image* stunImage = new Image();
    Hero* heroref = nullptr;
    float timer = 0.0f;
    float health = 100.0f;
    float maxHealth = 100.0f;
    HealthBar myHealth;
    void initPhysics(TMXTiledMap* level);
    PhysicsBody* mPhysicsBody;

private:
    MonsterState currentState;
    // State-specific methods
    void enterNewState();

    void exitCurrentState();

    void updateIdleState(float dt);
    void updateAttackState(float dt);
    void updateStunState(float dt);
    void updateDestroyState(float dt);
};

class MonsterPool 
{
public:
    // Create an object pool with a specified capacity
    MonsterPool(int capacity);
    // Get an enemy from the pool
    Monster* getMonster();

    // Return an enemy to the pool
    void returnMonster(Monster* enemy);

    std::vector<Monster*> getMonsterPool() { return monsterPool; };

private:
    std::vector<Monster*> monsterPool;
};
