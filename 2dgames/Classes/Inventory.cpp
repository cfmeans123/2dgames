#include "Inventory.h"


using namespace cocos2d;



Inventory::Inventory()
{
	items.clear();
	//itemDraws.reserve(25);
}

void Inventory::addItem(const std::string& itemName)
{
	Sprite* sprite = new Sprite();
	sprite->autorelease();
	if (itemName == "GreenToken")
	{
		sprite->initWithFile("Token/greentoken.png");
	}
	if (itemName == "PurpleToken")
	{
		sprite->initWithFile("Token/purpletoken.png");
	}
	if (itemName == "GreenEssence")
	{
		sprite->initWithFile("Token/greentoken.png");
	}
	if (itemName == "PurpleEssence")
	{
		sprite->initWithFile("Token/purpletoken.png");
	}
	//sprite->setPosition((_director->getVisibleSize().width / 5) + drawPadding, _director->getVisibleSize().height * 0.1);
	sprite->setScale(0.3f);
	this->addChild(sprite);
	items.push_back(itemName);
	itemDraws.push_back(sprite);
	
}

void Inventory::removeItem(const std::string& itemName)
{
	// Remove an item from the inventory.

	auto it = std::find(items.begin(), items.end(), itemName);
	int index = std::distance(items.begin(), it);
	
	if (it != items.end())
	{
		if (it != items.end())
		{
			items.erase(it);
			this->removeChild(itemDraws.at(index));
			//delete itemDraws.at(index);
			itemDraws.at(index) = nullptr;
			itemDraws.erase(std::remove(itemDraws.begin(), itemDraws.end(), nullptr), itemDraws.end());
		
		}
	}
}



void Inventory::displayInventory(Hero* hero)
{
	drawPaddingX = 25;
	drawPaddingY = 25;
	for (auto &item : items)
	{
		
		int displayindex = &item - &items[0];
		if (drawPaddingX > 500)
		{
			drawPaddingX = 25;
			drawPaddingY -= 75;
		}
		itemDraws.at(displayindex)->setPosition(hero->getPosition().x - (_director->getVisibleSize().width / 5) + drawPaddingX, _director->getVisibleSize().height * 0.9 + drawPaddingY);
		drawPaddingX += 75;
		
	}

}