#pragma once

#include "cocos2d.h"

USING_NS_CC;

class MazeScene : public Scene
{
public:
    virtual bool init();

    virtual void update(float dt);

    void initialize(Sprite* spr, TMXLayer* layer, std::pair<int, int>& position, bool draw = true);

    std::pair<int, int> FlipY(std::pair<int, int> position);
    
    int flippedY;

    void setPosition(Sprite* spr, std::pair<int, int> playerPosition, std::pair<int, int>& oldPosition, bool draw = true);

    bool canSetPosition(std::pair<int, int> playerPosition);

    void changeActiveSprite(Sprite* newActive);

    CREATE_FUNC(MazeScene)
protected:
    const Size* mapSize;
    const Size* tileSize;

    std::pair<int, int> playerPosition;
    std::pair<int, int> endPosition;

    TMXLayer* path = NULL;
    DrawNode * drawNode = NULL;

private:
    enum GameState
    {
        Start,
        Running,
        FoundCheese,
        Victory
    } gameState = Start;

    

    Sprite* active;
    Sprite* ratDown;
    Sprite* ratLeft;
    Sprite* ratUp;
    Sprite* ratRight;
    Sprite* cheese;


    float cheeseAnimationTimer = 0;

    void ResetInput();

    bool down = false;
    bool left = false;
    bool up = false;
    bool right = false;
};
