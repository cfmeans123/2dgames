
#ifndef __INVENTORY_H__
#define __INVENTORY_H__

#include "cocos2d.h"
#include "Item.h"



class Inventory : public cocos2d::Node
{
public:
    Inventory();

    // Add an item to the inventory.
    void addItem(const std::string& itemName);

    // Remove an item from the inventory.
    void removeItem(const std::string& itemName);

    // Display the inventory on the screen.
    void displayInventory();

private:
    std::vector<std::string> items;
    int drawPadding = 25;

};

#endif // __INVENTORY_H__#pragma once
