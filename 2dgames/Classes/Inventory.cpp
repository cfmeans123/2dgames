#include "Inventory.h"

using namespace cocos2d;

Inventory::Inventory()
{
    // Initialize the inventory as empty.
    items.clear();
    myitem.initWithFile("Token/greentoken.png");
    addChild(&myitem);
}

void Inventory::addItem(const std::string& itemName)
{
    // Add an item to the inventory.
    items.push_back(itemName);
}

void Inventory::removeItem(const std::string& itemName)
{
    // Remove an item from the inventory.
    auto it = std::find(items.begin(), items.end(), itemName);
    if (it != items.end())
    {
        items.erase(it);
    }
}

void Inventory::displayInventory()
{
    drawPadding = 25;
    // Display the inventory items on the screen (you can customize this).
    for (const auto& item : items)
    {
        // Create a label to display each item.
        auto label = Label::createWithTTF(item, "fonts/arial.ttf", 24);
        if (item == "GreenToken")
        {            
            //myitem.initWithFile("Token/greentoken.png");            
            myitem.setVisible(true);
            myitem.setPosition(_director->getVisibleSize().width + drawPadding, _director->getVisibleSize().height - (_director->getVisibleSize().height * 0.1));
            drawPadding += 25;
        }
        // Position the label on the screen as per your design.
        // Add the label to your scene or a specific layer.
    }
}