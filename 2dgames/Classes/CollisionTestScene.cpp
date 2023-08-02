#include "CollisionTestScene.h"
#include "CollisionComponent.h"
#define pi 3.1415



USING_NS_CC;



bool CollisionTestScene::init()
{

    visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();



    active = stone = Sprite::create("fighter.png");
    
    player = Player::create();
    addChild(player);
    stone->setPosition(Vec2(visibleSize.width / 2.0f, visibleSize.height / 2.0f));
    stone->addComponent(CollisionComponent::createBox(stone->getContentSize().width, stone->getContentSize().height));

    //player->setPosition(Director::getInstance()->getVisibleSize().width / 2.0f + 20, Director::getInstance()->getVisibleSize().height / 2.0f);
    player->x = Director::getInstance()->getVisibleSize().width / 2.0f + 20;
    player->y = Director::getInstance()->getVisibleSize().height / 2.0f;

    bulletPool = new BulletPool();

    myHealth = new HealthBar();
    this->addChild(myHealth->emptyBar, 1);
    this->addChild(myHealth->fillBar, 1);

    //mushroom = Sprite::create("mushroom.png");
    //mushroom->setPosition(Vec2(visibleSize.width / 4.0f, visibleSize.height / 4.0f));
    //mushroom->setScale(3.0f);
    /// TODO: ADD CODE Here -- Requires CollisionComponent
    //mushroom->addComponent(CollisionComponent::createBox(mushroom->getBoundingBox().size.width, mushroom->getBoundingBox().size.height));
    //mushroom->addComponent(CollisionComponent::createCircle(mushroom->getBoundingBox().size.width / 2.0f));

    bullet2 = Sprite::create("bullet2.png");
    bullet2->setPosition(Vec2(visibleSize.width / 8.0f, visibleSize.height / 4.0f));
    /// TODO: ADD CODE Here -- Requires CollisionComponent
    bullet2->addComponent(CollisionComponent::createCircle(bullet2->getBoundingBox().size.width / 2.0f));

    //hummer = Sprite::create("hummer.png");
    //hummer->setPosition(Vec2(7.0f * visibleSize.width / 8.0f, visibleSize.height / 4.0f));
    /// TODO: ADD CODE Here -- Requires CollisionComponent
    //hummer->addComponent(CollisionComponent::createBox(hummer->getBoundingBox().size.width, hummer->getBoundingBox().size.height / 2));

    //circle = Sprite::create("crosshair.png");
    //circle->setPosition(20, 20);
    //circle->addComponent(CollisionComponent::createDot());

    auto label = Label::create();
    label->setString("Arrow keys to move\n\nm: Mushroom\nb: Bullet\ns: Stone\nh: Hummer\nc: Circle\nd: Debug draw");
    label->setPosition(Vec2(label->getContentSize().width / 2.0f + 10, visibleSize.height - label->getContentSize().height / 2.0f - 10));

    this->addChild(stone, 0);
    //this->addChild(mushroom, 0);
    this->addChild(bullet2, 0);
    //this->addChild(hummer, 0);
    //this->addChild(circle, 0);
    //this->addChild(label, 0);

    debug = DrawNode::create(5);
    this->addChild(debug, 1);

    this->scheduleUpdate();



    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event)
    {
        switch (keyCode)
        {
        case EventKeyboard::KeyCode::KEY_SPACE:
            fireBullet();
            break;
        case EventKeyboard::KeyCode::KEY_S:
            active = stone;
            break;
        /*case EventKeyboard::KeyCode::KEY_M:
            active = mushroom;
            break;*/
        case EventKeyboard::KeyCode::KEY_B:
            active = bullet2;
            break;
        /*case EventKeyboard::KeyCode::KEY_H:
            active = hummer;
            break;
        case EventKeyboard::KeyCode::KEY_C:
            active = circle;
            break;*/
        case EventKeyboard::KeyCode::KEY_D:
            debugDrawEnabled = !debugDrawEnabled;
            break;
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            left = true;
            break;
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            right = true;
            break;
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            up = true;
            break;
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
            down = true;
            break;
        };
    };

    keyboardListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event)
    {
        switch (keyCode)
        {
        case EventKeyboard::KeyCode::KEY_SPACE:
            fire = false;
            break;
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            left = false;
            break;
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            right = false;
            break;
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            up = false;
            break;
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
            down = false;
            break;
        }
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    return true;
}


void CollisionTestScene::fireBullet()
{
    Bullet* bullet = bulletPool->getOrCreateBullet();
    bullet->setRotation(active->getRotation() + 45);
    bullet->isActive = true;
    bullet->setPosition(active->getPosition());
    bullet->moveDirection.x = movedirX;
    bullet->moveDirection.y = movedirY;
    if (bullet->getParent() == nullptr)
    {
        addChild(bullet);
    }
    else
    {
        //bulletPool->returnBulletToPool(bullet);
        bullet->isActive = true;
    }

};

void CollisionTestScene::update(float dt)
{
        auto p = active->getPosition();

    if (active != NULL && (left || right || up || down))
    {
        //old movement
       /* if (right && up)
        {
            p.x += 200 * dt;
            p.y -= 200 * dt;
            movedirX = 200;
            movedirY = -200;
            active->setRotation(135.0f);


        }
        else if (right && down)
        {
            p.x += 200 * dt;
            p.y += 200 * dt;
            movedirX = 200;
            movedirY = 200;
            active->setRotation(45.0f);
        }
        else if (left && up)
        {
            p.x -= 200 * dt;
            p.y -= 200 * dt;

            movedirX = -200;
            movedirY = -200;
            active->setRotation(225.0f);
        }
        else if (left && down)
        {
            p.x -= 200 * dt;
            p.y += 200 * dt;
            movedirX = -200;
            movedirY = 200;
            active->setRotation(315.0f);
        }
        else if (left)
        {
            p.x -= 200 * dt;
            movedirX = -200.0f;
            movedirY = 0.0f;
            active->setRotation(270.0f);
        }
        else if (right)
        {
            p.x += 200 * dt;
            movedirX = 200;
            movedirY = 0;
            active->setRotation(90.0f);
        }

        else if (up)
        {
            p.y -= 200 * dt;
            movedirX = 0;
            movedirY = -200;
            active->setRotation(180.0f);
        }

        else if (down)
        {
            p.y += 200 * dt;
            movedirX = 0;
            movedirY = 200;
            active->setRotation(0.0f);
        }*/
    }
        if (left)
        {
            player->rotAngle -= 55.0f * dt;
            
        }
        if (right)
        {
            player->rotAngle += 55.0f * dt;
        }
        if (down) 
        {
            player->dx += sin((player->rotAngle * pi) / 180) * 35.0f * dt;
            player->dy += -cos((player->rotAngle * pi) / 180) * 35.0f * dt;
         }
       

        player->x += player->dx * dt;
        player->y += player->dy * dt;   
        player->setPosition(player->x, player->y);
        player->setRotation(player->rotAngle);
    myHealth->emptyBar->setPosition(active->getPositionX(), active->getPositionY() + 50);
    myHealth->fillBar->setPosition(active->getPositionX(), active->getPositionY() + 50);
    if (bulletPool != nullptr)
    {
        for (Bullet* bullet : bulletPool->bulletPool) {
            if (bullet != nullptr)
            {
                auto pos = bullet->getPosition();
                
                if (bullet->isActive) 
                {

                    if (bullet->getComponent("CollisionComponent") == NULL)
                    {
                        bullet->addComponent(CollisionComponent::createCircle(bullet->getBoundingBox().size.width / 2.0f));
                    }
                    bullet->update(dt);
                    if ((pos.y + bullet->spriteHeight) > visibleSize.height || (pos.y < visibleSize.height - visibleSize.height))
                    {
                        bullet->removeComponent("CollisionComponent");
                        removeChild(bullet);
                        bulletPool->returnBulletToPool(bullet);
                    }
                    if ((pos.x + bullet->spriteWidth) > visibleSize.width || (pos.x < visibleSize.width - visibleSize.width))
                    {
                        bullet->removeComponent("CollisionComponent");
                        removeChild(bullet);
                        bulletPool->returnBulletToPool(bullet);
                    }
                    // chech for bullet collisions and handle bullet despawn here
                }
            }
        }
    }



    debug->clear();
    debug->setLineWidth(5);

    /// Reset all collision
    for (auto it : this->getChildren())
    {
        if (auto collision = dynamic_cast<CollisionComponent*>(it->getComponent("CollisionComponent")))
        {
            collision->SetColliding(false);
        }
    }

    auto& children = this->getChildren();

    //remember that this iterates over EVERY OBJECT IN THE SCENE**Not efficient for collision checks at all
    for (auto it = children.begin(); it != children.end(); ++it)
    {
        for (auto it2 = it + 1; it2 != children.end(); ++it2)
        {
            auto collision = dynamic_cast<CollisionComponent*>((*it)->getComponent("CollisionComponent"));
            auto collision2 = dynamic_cast<CollisionComponent*>((*it2)->getComponent("CollisionComponent"));
            if (collision && collision2 && collision->IsColliding(collision2))
            {
                collision->SetColliding(true);
                collision2->SetColliding(true);
            }
        }
    }
    /// Determine all colliding objects
    /// TODO: ADD CODE Here

    ///

    if (debugDrawEnabled)
    {
        for (auto it : this->getChildren())
        {
            auto collision = dynamic_cast<CollisionComponent*>(it->getComponent("CollisionComponent"));

            if (collision != NULL)
            {
                auto position = it->getPosition();

                auto color = collision->IsColliding() ? Color4F::RED : Color4F::GREEN;

                switch (collision->GetCollisionType())
                {
                case CollisionComponent::Box:
                {
                    auto dx = collision->GetWidth() / 2.0f;
                    auto dy = collision->GetHeight() / 2.0f;
                    debug->drawRect(Vec2(position.x - dx, position.y + dy), Vec2(position.x + dx, position.y + dy), Vec2(position.x + dx, position.y - dy), Vec2(position.x - dx, position.y - dy), color);
                }
                break;
                case CollisionComponent::Circle:
                {
                    auto radius = collision->GetRadius();
                    debug->drawCircle(position, radius, 10, 360, false, color);
                }
                break;
                case CollisionComponent::Point:
                    debug->drawDot(position, 3, color);
                    break;
                }
            }
        }
    }
}

/// Point to AABB
bool CollisionComponent::checkCollision(const Vec2& p, const AABB& b)
{
    if (p.x < b._x || p.x > b._x + b._width)
    {
        return false;
    }
    if (p.y < b._y || p.y > b._y + b._height)
    {
        return false;
    }
    return true;
}

/// Point to Circle
bool CollisionComponent::checkCollision(const Vec2& p, const Vec2& c, float r)
{
    if ((p.x - c.x) * (p.x - c.x) + (p.y - c.y) * (p.y - c.y) <= (r * r))
    {
        return true;
    }
    /// TODO: ADD CODE Here
    return false;
}

/// AABB to AABB
bool CollisionComponent::checkCollision(const AABB& b1, const AABB& b2)
{
    if (b1._x + b1._width < b2._x || b1._x > b2._x + b2._width)
    {
        return false;
    }
    if (b1._y + b1._height < b2._y || b1._y > b2._y + b2._height)
    {
        return false;
    }
    return true;
}

/// Circle to Circle
bool CollisionComponent::checkCollision(const Vec2& c1, float r1, const Vec2& c2, float r2)
{
    float r = (r1 + r2);
    float x = (c1.x - c2.x);
    float y = (c1.y - c2.y);
    return (x * x) + (y * y) <= (r * r);
}

/// Circle to AABB
bool CollisionComponent::checkCollision(const AABB& b1, const Vec2& c2, float r2)
{
    return checkCollision(c2, b1) || /// center of rectangle is in circle
        intersectCircle(c2, r2, Vec2(b1._x, b1._y), Vec2(b1._x + b1._width, b1._y)) ||
        intersectCircle(c2, r2, Vec2(b1._x, b1._y), Vec2(b1._x, b1._y + b1._height)) ||
        intersectCircle(c2, r2, Vec2(b1._x + b1._width, b1._y + b1._height), Vec2(b1._x, b1._y + b1._height)) ||
        intersectCircle(c2, r2, Vec2(b1._x + b1._width, b1._y + b1._height), Vec2(b1._x + b1._width, b1._y));
}

bool CollisionComponent::intersectCircle(const Vec2& c, float r, Vec2 p1, Vec2 p2)
{
    float min;
    float d1 = c.distance(p1);
    float d2 = c.distance(p2);
    float max = d1 > d2 ? d1 : d2;

    if (Vec2(c.x - p1.x, c.y - p1.y).dot(Vec2(p2.x - p1.x, p2.y - p1.y)) > 0 && Vec2(c.x - p2.x, c.y - p2.y).dot(Vec2(p1.x - p2.x, p1.y - p2.y)) > 0)
    {
        min = 2.0f * TriangleArea(c, p1, p2) / p1.distance(p2);
    }
    else
    {
        min = d1 < d2 ? d1 : d2;
    }

    if (min <= r && max >= r)
    {
        return true;
    }

    return false;
}

float CollisionComponent::TriangleArea(Vec2 const& a, Vec2 const& b, Vec2 const& c)
{
    Vec2 ab = Vec2(b.x - a.x, b.y - a.y);
    Vec2 ac = Vec2(c.x - a.x, c.y - a.y);

    return abs(ab.cross(ac)) / 2.0f;
}