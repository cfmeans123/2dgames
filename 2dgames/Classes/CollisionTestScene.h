#pragma once

#include "cocos2d.h"
#include "BulletPool.h"

USING_NS_CC;

class CollisionTestScene : public Scene
{
public:
	virtual bool init();

	virtual void update(float dt);
	
	BulletPool* bulletPool;
	void fireBullet();

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
		}
		private:
			float maxHealth;
			float currentHealth;
	};

	HealthBar* myHealth;

private:
	Node* active;
	Sprite* stone;
	Sprite* mushroom;
	Sprite* bullet2;
	Sprite* hummer;
	Sprite* circle;

	DrawNode* debug;

	bool fire;
	bool left;
	bool right;
	bool up;
	bool down;
	bool debugDrawEnabled = true;



	//bullet movement
	float movedirX = 0.0f;
	float movedirY = 0.0f;
};
