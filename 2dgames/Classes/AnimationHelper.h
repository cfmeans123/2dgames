#pragma once
#include "cocos2d.h"

USING_NS_CC;

static class AnimationHelper
{
public:
	static Vector<SpriteFrame*> getAnimation(const char* format, int count)
	{
		auto spritecache = SpriteFrameCache::getInstance();
		Vector<SpriteFrame*> animFrames;
		char str[100];
		for (int i = 1; i <= count; i++)
		{
			snprintf(str, sizeof(str), format, i);
			animFrames.pushBack(spritecache->getSpriteFrameByName(str));
		}
		return animFrames;
	}

};