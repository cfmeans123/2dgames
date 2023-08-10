#pragma once

#include <stdlib.h>
#include "cocos2d.h"
#include "BulletPool.h"
#include "Player.h"
#include "EnemyPool.h"
#include "Enemy.h"
#include "time.h"

enum GameState
{
	Menu,
	Gameloop,
	Pause
};

USING_NS_CC;

class CollisionTestScene : public Scene
{
public:
	virtual bool init();

	virtual void update(float dt);

	GameState curGameState;
	Vector<SpriteFrame*> getAnimation(const char* format, int count);

	BulletPool* bulletPool;
	EnemyPool* enemyPool;
	void fireBullet();
	void createEnemy();

	CREATE_FUNC(CollisionTestScene)

	struct HealthBar
	{
	public:
		Sprite* fillBar;
		Sprite* emptyBar;

		HealthBar()
		{
			fillBar = Sprite::create("bar_red.png");
			emptyBar = Sprite::create("bar_empty.png");
			maxHealth = 100.0f;
			currentHealth = 100.0f;
		}
		float maxHealth;
		float currentHealth;		


	};


	void RunAnimOnce(Sprite* sprite);

	//anim
	
	

	HealthBar* myHealth;



	Size visibleSize;
	float waveTimer;
	float waveIncrement;
	int enemiesDefeated;


	void spawnWave(int count);


private:
	Node* active;
	Sprite* stone;
	Sprite* mushroom;
	Sprite* bullet2;
	Sprite* hummer;
	Sprite* circle;
	Player* player;


	DrawNode* debug;

	bool fire;
	bool left;
	bool right;
	bool up;
	bool down;
	bool r;
	bool debugDrawEnabled = true;



	//bullet movement
	float movedirX = 0.0f;
	float movedirY = 0.0f;
};
