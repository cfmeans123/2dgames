#include "cocos2d.h"
USING_NS_CC;

class Player : public cocos2d::Sprite
{
public:
	static Player* create();

	
	Vec2 moveDir;
	float spriteHeight;
	float spriteWidth;
	float x;
	float y;
	float dx;
	float dy;
	float rotAngle = 0.0f;

private:

};