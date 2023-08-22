#pragma once

#include "cocos2d.h"
#include "MazeScene.h"

USING_NS_CC;

template<typename T, typename T2> /// Template with 2 arguments
inline auto operator+(std::pair<T, T2> const& p1, std::pair<T, T2> const& p2) /// operator overloading 
{
    auto [x1, y1] = p1;
    auto [x2, y2] = p2; /// C++17 Structured Bindings -- tuple deduction

    return std::pair(x1 + x2, y1 + y2); /// C++17 Template Argument Deduction for Class, and Constructor template argument deduction
}

template<typename T, typename T2>
inline decltype(auto) operator+=(std::pair<T, T2>& p1, std::pair<T, T2> const& p2)
{
    auto& [x1, y1] = p1;
    auto& [x2, y2] = p2;

    x1 += x2;
    y1 += y2;

    return p1;
}

template<typename T, typename T2, typename V>
inline decltype(auto) operator+=(std::pair<T, T2>& p, V const& v)
{
    auto& [x1, y1] = p;
    auto& [x2, y2] = v;

    x1 += x2;
    y1 += y2;

    return p;
}

inline Vec2 operator+(Vec2 const& v, std::pair<float, float> const& p) /// operator overloading allowing Vec2 + Pair 
{
    auto [x1, y1] = p;
    auto [x2, y2] = v;

    x1 += x2;
    y1 += y2;

    return { x1, y1 };
}

template<typename P>
inline decltype(auto) operator+=(Vec2& v, P const& p)
{
    auto [x1, y1] = v;
    auto [x2, y2] = p;

    x1 += x2;
    y1 += y2;

    return v;
}

class MazeScene2 : public MazeScene
{
public:
    virtual bool init();

    virtual void update(float dt);
    CREATE_FUNC(MazeScene2)
    static Scene* createScene();
    bool isValid(const std::pair<int, int>& pos);
    bool isValidBFS(const std::pair<int, int>& pos);

    Vector<SpriteFrame*> getAnimation(const char* format, int count);


    //debug draw variables
    bool r;
    bool t;
    bool d;
    float time;

private:
    int dfs(const std::pair<int, int>& current, const std::pair<int, int>& target, int depth);

    int moveEnemyWithDFS(const std::pair<int, int>& current, const std::pair<int, int>& target, int depth);

    std::vector<std::vector<bool>> dfsVisited;
    std::vector<std::vector<bool>> enemydfsVisited;
    std::vector<std::pair<int, int>> dfsPath;
    std::vector<std::pair<int, int>> enemydfsPath;

    int moveit;

    bool bfs(std::pair<int, int> current, std::pair<int, int>const& target);
    
    std::vector<std::vector<std::pair<int, int>>> bfsParent;


    bool bfsVisitChild(std::pair<int, int> const& parent, std::pair<int, int> const& c, std::pair<int, int> const& t, std::vector<std::pair<int, int>>& children);

    void bfsBacktrack();

    std::vector<std::vector<bool>> bfsVisited;
    std::vector<std::pair<int, int>> bfsPath;

    auto toScreenSpace(std::pair<int, int>const& position)
    {
        return toScreenSpaceT<std::pair<float, float>>(position);
    }
    template<typename T>
    T toScreenSpaceT(std::pair<int, int>const& position)
    {
        return
        {
          tileSize->width * (float)position.first,
          tileSize->height * (float)((int)mapSize->height - position.second - 1)
        };
    }

    template<typename T>
    void reset(std::vector<std::vector<T>>& v, std::vector<std::pair<int, int>>& p, T defaultT)
    {
        Vec2 v2;
        std::pair<float, float>p2;

        p.clear();

        for (int i = 0; i < mapSize->width; ++i)
        {
            for (int j = 0; j < mapSize->height; ++j)
            {
                v[i][j] = defaultT;
            }
        }
    }
    void drawPath(std::vector<std::pair<int, int>>const& p, std::pair<float, float> offset, Color4F const& color)
    {
        if (p.size() > 0)
        {
            drawNode->setLineWidth(5.0f);

            auto tileCenter = std::pair(tileSize->width / 2.0f, tileSize->height / 2.0f);
            auto previous = toScreenSpace(p[0]) + tileCenter + offset;

            for (size_t i = 1; i < p.size(); ++i)
            {
                auto current = toScreenSpace(p[i]) + tileCenter + offset;

                drawNode->drawLine({ previous.first, previous.second }, { current.first, current.second }, color);

                std::swap(previous, current);
            }
        }
    }

    template<typename T, typename S>
    void drawVisited(std::vector<std::vector<T>> const& v, Color4F const& c, std::pair<float, float> offset, S const& size, T defaultT)
    {
        for (int i = 0; i < mapSize->width; ++i)
        {
            for (int j = 0; j < mapSize->height; ++j)
            {
                if (v[i][j] != defaultT)
                {
                    auto origin = toScreenSpaceT<S>({ i, j });

                    origin += offset;                   ///operator  += overloaded
                    auto dest = size + origin;          /// operator + overloaded

                    drawNode->drawSolidRect(origin, dest, c);
                }
            }
        }
    }
};