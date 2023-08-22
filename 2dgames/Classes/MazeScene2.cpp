#include "MazeScene2.h"
#include <vector>


Scene* MazeScene2::createScene()
{
    return MazeScene2::create();
}

bool MazeScene2::isValid(const std::pair<int, int>& pos) 
{
    int x = pos.first;
    int y = pos.second;
    return x >= 0 && x < dfsVisited.size() && y >= 0 && y < dfsVisited[0].size() && !dfsVisited[x][y];
}
bool MazeScene2::isValidBFS(const std::pair<int, int>& pos)
{
    int x = pos.first;
    int y = pos.second;
    return x >= 0 && x < bfsVisited.size() && y >= 0 && y < bfsVisited[0].size() && !bfsVisited[x][y];
}

bool MazeScene2::init()
{
    if (!MazeScene::init())
    {
        return false;
    }
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("SD/explosion_anim_frames.plist");


    dfsVisited.resize((int)mapSize->width);
    enemydfsVisited.resize((int)mapSize->width);


    for (int i = 0; i < (int)mapSize->width; ++i)
    {
        dfsVisited[i].resize((int)mapSize->height);
        enemydfsVisited[i].resize((int)mapSize->height);
    }

    dfsPath.reserve((int)mapSize->width * (int)mapSize->height);
    enemydfsPath.reserve((int)mapSize->width * (int)mapSize->height);

    bfsVisited.resize((int)mapSize->width);
    bfsParent.resize((int)mapSize->width);
   
    for (int i = 0; i < (int)mapSize->width; ++i)
    {
       bfsVisited[i].resize((int)mapSize->height);
       bfsParent[i].resize((int)mapSize->height);
    }
    bfsPath.reserve((int)mapSize->width * (int)mapSize->height);
    //bfsParent.reserve((int)mapSize->width * (int)mapSize->height);



    moveit = 2;

   auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event)
    {
        switch (keyCode)
        {
        case EventKeyboard::KeyCode::KEY_R:
            r = !r;
            break;
        case EventKeyboard::KeyCode::KEY_T:
            t = !t;
            break;
        case EventKeyboard::KeyCode::KEY_D:
            d = !d;
            break;
        }
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    this->scheduleUpdate();
    
    r = true;
    d = false;
    return true;
}


void MazeScene2::update(float dt)
{
    time += dt;
    
    MazeScene::update(dt);

    Vec2 size = { tileSize->width, tileSize->height };

    if (MazeScene::enemy != nullptr)
    {
        if (time > 0.25f && r == true)
        {
            time = 0.0f;
           if (int j = moveEnemyWithDFS(FlipY(enemyPosition), FlipY(playerPosition), 0); j != -1)
            {
                
                
                if (moveit < enemydfsPath.size())
                {
                    moveit++;
                    setPosition(enemy, FlipY(enemydfsPath.end()[-moveit]), FlipY(enemyPosition));
                }

                else
                {
                    moveit = 0;
                    r = false;
                    Vector<SpriteFrame*> animFrames = getAnimation("tile%01d.png", 71);

                    Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.01f);
                    Animate* animate = Animate::create(animation);
                    Sprite* sprite = Sprite::create();
                    auto flipPos = FlipY(playerPosition);
                    sprite->setPosition(Vec2(flipPos.first, flipPos.second));
                    sprite->runAction(animate);
                    addChild(sprite, 4);
                }
                if (d)
                {
                    drawNode->clear();
                    drawVisited(enemydfsVisited, Color4F::BLUE, { 0.0f,0.0f }, size, false);
                    drawPath(enemydfsPath, { 1.0f, 0.0f }, Color4F::GREEN);
                }
                reset(enemydfsVisited, enemydfsPath, false);
            }
            else
            {
                setPosition(enemy, enemydfsPath[0], enemyPosition);
                if (d)
                {
                    drawPath(enemydfsPath, { 1.0f, 0.0f }, Color4F::GREEN);
                    drawVisited(enemydfsVisited, Color4F::BLUE, { 0.0f,0.0f }, size, false);
                }
            }
            
            

        }
    }


    if (t)
    {
        auto bfsOffset = std::pair{ 5.0f, 5.0f };
        drawNode->clear();

        if (bfs(FlipY(playerPosition), FlipY(endPosition)))
        {
            drawVisited(bfsVisited, Color4F::GREEN, bfsOffset, size, false);
            drawPath(bfsPath, bfsOffset, Color4F::RED);
        }
        else
        {
            drawVisited(bfsVisited, Color4F::GREEN, bfsOffset, size, false);
            drawPath(bfsPath, bfsOffset, Color4F::ORANGE);
        }
        reset(bfsVisited, bfsPath, false);
    }
   
       

}



int MazeScene2::dfs(const std::pair<int, int>& current, const std::pair<int, int>& target, int depth) 
{
    auto& [x1, y1] = current;
    if (!path->getTileAt(Vec2(x1, y1)) || dfsVisited[x1][y1])
    {
        return -1;
    }
    if (current == target) 
    {
        return depth;
    }

    dfsVisited[current.first][current.second] = true;

    std::pair<int, int> neighbors[] = 
    {
        {current.first + 1, current.second},
        {current.first - 1, current.second},
        {current.first, current.second + 1},
        {current.first, current.second - 1}
    };

    for (const std::pair<int, int>& neighbor : neighbors) 
    {
        if (isValid(neighbor)) 
        {
            int result = dfs(neighbor, target, depth + 1);
            if (result != -1) 
            {
                dfsPath.push_back(current);
                return result;
            }
        }
    }

    return -1;
}

int MazeScene2::moveEnemyWithDFS(const std::pair<int, int>& current, const std::pair<int, int>& target, int depth)
{

    auto& [x1, y1] = current;
    if (!path->getTileAt(Vec2(x1, y1)) || enemydfsVisited[x1][y1])
    {
        return -1;
    }
    if (current == target)
    {
        return depth;
    }

    enemydfsVisited[current.first][current.second] = true;

    std::pair<int, int> neighbors[] =
    {
        {current.first + 1, current.second},
        {current.first - 1, current.second},
        {current.first, current.second + 1},
        {current.first, current.second - 1}
    };

    for (const std::pair<int, int>& neighbor : neighbors)
    {
        if (isValid(neighbor))
        {
            int result = moveEnemyWithDFS(neighbor, target, depth + 1);
            if (result != -1)
            {
                enemydfsPath.push_back(current);
                return result;
            }
        }
    }

    return -1;
}


bool MazeScene2::bfsVisitChild(std::pair<int, int> const& parent, std::pair<int, int> const& c, std::pair<int, int> const& t, std::vector<std::pair<int, int>>& children)
{
    /// TODO: Fill in
    return false;
}

void MazeScene2::bfsBacktrack()
{
    /// TODO: Fill in
}

bool MazeScene2::bfs(std::pair<int, int> current, const std::pair<int, int>& target) 
{
    std::queue<std::pair<std::pair<int, int>, int>> q; // pair<position, depth>
    q.push({ current, 0 });
    std::pair<int, int> currentBegin = q.front().first;
    bfsVisited[current.first][current.second] = true;
    bool found = false;
    while (!q.empty()) 
    {
        current = q.front().first;
        int depth = q.front().second;
        q.pop();

        if (current == target) 
        {
            do
            {   
                current = bfsParent[current.first][current.second];
                bfsPath.push_back(current);
                
            } while (current != currentBegin);
            return true;
        }

        std::pair<int, int> neighbors[] = 
        {
            {current.first + 1, current.second},
            {current.first - 1, current.second},
            {current.first, current.second + 1},
            {current.first, current.second - 1}
        };
        
        for (const std::pair<int, int>& neighbor : neighbors) 
        {
            auto p = FlipY(neighbor);
            if (!isValidBFS(neighbor) || !path->getTileAt(Vec2(neighbor.first, neighbor.second)))
            {
                continue;
            }
            q.push({ neighbor, depth + 1 });
            bfsVisited[neighbor.first][neighbor.second] = true;
            bfsParent[neighbor.first][neighbor.second] = current;
        }
    }

    return false;
}


Vector<SpriteFrame*> MazeScene2::getAnimation(const char* format, int count)
{
    auto spritecache = SpriteFrameCache::getInstance();
    Vector<SpriteFrame*> animFrames;
    char str[100];
    for (int i = 1; i <= count; i++)
    {
        sprintf(str, format, i);
        animFrames.pushBack(spritecache->getSpriteFrameByName(str));
    }
    return animFrames;
}
