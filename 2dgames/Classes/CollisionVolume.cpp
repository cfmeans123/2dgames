#include "CollisionVolume.h"

bool CollisionVolume::init()
{

	if (!Node::init()) return false;

	auto shape = cocos2d::DrawNode::create();
	shape->setName("shape");
	shape->drawRect(this->getPosition(), cocos2d::Vec2(this->getPositionX() + 300, this->getPositionY() + 400), cocos2d::Color4F::BLUE);
	addChild(shape);
	setVisible(false);

	return true;
}