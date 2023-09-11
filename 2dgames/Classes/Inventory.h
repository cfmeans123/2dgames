
#ifndef __INVENTORY_H__
#define __INVENTORY_H__

#include "cocos2d.h"
#include "Item.h"
#include "Hero.h"

class Hero;

class Inventory : public cocos2d::Node
{
public:
    Inventory();

    // Add an item to the inventory.
    void addItem(const std::string& itemName);

    // Remove an item from the inventory.
    void removeItem(const std::string& itemName);

    // Display the inventory on the screen.
    void displayInventory(Hero* hero);

    std::vector<Sprite*> itemDraws;
private:
    std::vector<std::string> items;
    int drawPadding = 75;

};

#endif // __INVENTORY_H__#pragma once
