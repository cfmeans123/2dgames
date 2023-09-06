#pragma once
#include "cocos2d.h"
USING_NS_CC;

enum ItemType
{
	PurpleToken,
	GreenToken,
	PurpleEssence,
	GreenEssence
};

class Item : public Sprite
{
public:
	Item();
	Item(ItemType type);
	


private:
	//Texture2D mTexture;
	ItemType mType;
	cocos2d::Size slotSize = {10.0f, 10.0f};
};
