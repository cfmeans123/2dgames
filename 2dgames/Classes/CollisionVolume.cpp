#include "CollisionVolume.h"

bool CollisionVolume::init()
{

	if (!Node::init()) return false;

	// Create and add a visible shape to represent the collision volume
	auto shape = cocos2d::DrawNode::create();
	shape->setName("shape");
	shape->drawRect(this->getPosition(), cocos2d::Vec2(this->getPositionX() + 300, this->getPositionY() + 400), cocos2d::Color4F::BLUE);
	//shape->drawCircle(this->getPosition(), 50, 0, 360, false, 1.0, 1.0, cocos2d::Color4F::RED);
	addChild(shape);

	// Initially, set the collision volume to be invisible
	setVisible(false);

	return true;
}