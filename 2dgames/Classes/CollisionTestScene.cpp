#include "CollisionTestScene.h"
#include "CollisionComponent.h"
#define pi 3.1415



USING_NS_CC;



bool CollisionTestScene::init()
{
    //anim   
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("SD/explosion_anim_frames.plist");

    //rand init
    srand(time(0));
    waveTimer = 6.0f;
    waveIncrement = 2.0f;
    enemiesDefeated = 0;
    curGameState = GameState::Menu;

    visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //player init
    player = Player::create();
    addChild(player, 1);
    active = player;
    player->x = Director::getInstance()->getVisibleSize().width / 2.0f + 20;
    player->y = Director::getInstance()->getVisibleSize().height / 2.0f;
    player->addComponent(CollisionComponent::createCircle(player->getBoundingBox().size.width / 3.0f));
    FlipY(player);
    myHealth = new HealthBar();
    
    bulletPool = new BulletPool();
    enemyPool = new EnemyPool();

    this->addChild(myHealth->emptyBar, 1);
    this->addChild(myHealth->fillBar, 1);


    bullet2 = Sprite::create("bullet2.png");
    bullet2->setPosition(Vec2(visibleSize.width / 8.0f, visibleSize.height / 4.0f));
    bullet2->addComponent(CollisionComponent::createCircle(bullet2->getBoundingBox().size.width / 2.0f));


    //Background sprite draw
    Sprite* background = Sprite::create("space.jpg");
    background->setPosition(Director::getInstance()->getVisibleSize().width / 2.0f, Director::getInstance()->getVisibleSize().height / 2.0f);
    background->setStretchEnabled(true);
    background->setContentSize(Size(Director::getInstance()->getVisibleSize().width, Director::getInstance()->getVisibleSize().height));
    addChild(background, 0);

    auto label = Label::create();
    label->setString("Arrow keys to move\n\nm: Mushroom\nb: Bullet\ns: Stone\nh: Hummer\nc: Circle\nd: Debug draw");
    label->setPosition(Vec2(label->getContentSize().width / 2.0f + 10, visibleSize.height - label->getContentSize().height / 2.0f - 10));

    //this->addChild(bullet2, 1);

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
        case EventKeyboard::KeyCode::KEY_F:
            fire = true;
            break;
        case EventKeyboard::KeyCode::KEY_H:
            createEnemy();
            break;
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
        case EventKeyboard::KeyCode::KEY_R:
            r = true;
            break;
        case EventKeyboard::KeyCode::KEY_T:
            myHealth -= 100;
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

void CollisionTestScene::createEnemy()
{
    Enemy* enemy = enemyPool->getOrCreateEnemy();
    enemy->isActive = true;
    
    enemy->setPosition(Vec2(rand() % static_cast<int>(visibleSize.width * 0.2), rand() % static_cast<int>(visibleSize.height * 0.2)));
    enemy->moveDirection.x = -cos((rand() % 360) * (pi / 180)) * 25.0f;
    enemy->moveDirection.y = sin((rand() % 360) * (pi / 180)) * 25.0f;
    enemy->setRotation((atan(
        ((enemy->getPositionX() + enemy->moveDirection.x) - enemy->getPositionX())
        /
        ((enemy->getPositionY() + enemy->moveDirection.y) - enemy->getPositionY())
    ) * 180/pi));

   

    if (enemy->getParent() == nullptr)
    {
        addChild(enemy);
        FlipY(enemy);
    }
    else
    {
        enemy->isActive = true;
        FlipY(enemy);
    }
}

void CollisionTestScene::fireBullet()
{
    Bullet* bullet = bulletPool->getOrCreateBullet();
    bullet->setRotation(player->getRotation());
    bullet->isActive = true;
    bullet->moveDirection.x = -cos(player->rotAngle * (pi / 180)) * 60.0f;
    bullet->moveDirection.y = sin(player->rotAngle * (pi / 180)) * 60.0f;
    
    bullet->setPosition(player->getPosition() + bullet->moveDirection);
    if (bullet->getParent() == nullptr)
    {
        addChild(bullet, 1);
    }
    else
    {
        bullet->isActive = true;
    }
};

void CollisionTestScene::spawnWave(int count)
{
    for (int i = 0; i < count; ++i)
    {
        createEnemy();
    }
}

void CollisionTestScene::update(float dt)
{
    if (curGameState == GameState::Menu)
    {
        if (myHealth->currentHealth <= 0)
        {
            //Handle cleanup before restarting level
            curGameState = GameState::Pause;
        }

        if (fire)
        {
            fireBullet();
        }
        waveTimer -= dt;
        if (waveTimer <= 0.0f)
        {
            waveTimer += waveIncrement;
            waveIncrement += 2.0f;
            spawnWave(waveIncrement);
            myHealth->currentHealth = 100;
        }


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
            player->rotAngle -= 205.0f * dt;

        }
        if (right)
        {
            player->rotAngle += 205.0f * dt;
        }
        if (down)
        {
            player->dy += sin(player->rotAngle * (pi / 180)) * 400.0f * dt;
            player->dx += -cos(player->rotAngle * (pi / 180)) * 400.0f * dt;
        }

        //cap player speed before updating, else accel will infinitely affect velocity
        if (player->dy >= 200.0f)
        {
            player->dy = 200.0f;
        }
        if (player->dy <= -200.0f)
        {
            player->dy = -200.0f;
        }
        if (player->dx >= 200.0f)
        {
            player->dx = 200.0f;
        }
        if (player->dx <= -200.0f)
        {
            player->dx = -200.0f;
        }

        //modify player pos
        player->x += player->dx * dt;
        player->y += player->dy * dt;
        player->setPosition(player->x, player->y);
        player->setRotation(player->rotAngle - 90);

        //screen wrap
        if (player->x > visibleSize.width)
        {
            player->x -= visibleSize.width;
        }
        if (player->x < 0)
        {
            player->x += visibleSize.width;
        }
        if (player->y > visibleSize.height)
        {
            player->y -= visibleSize.height;
        }
        if (player->y < 0)
        {
            player->y += visibleSize.height;
        }

        if (myHealth != nullptr)
        {
            myHealth->emptyBar->setPosition(active->getPositionX(), active->getPositionY() + 50);
            myHealth->fillBar->setScaleX((myHealth->currentHealth / myHealth->maxHealth));
            myHealth->fillBar->setPosition(active->getPositionX() - (myHealth->emptyBar->getContentSize().width / 2 - (myHealth->emptyBar->getContentSize().width / 2) * myHealth->currentHealth / myHealth->maxHealth), active->getPositionY() + 50);
        }

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



                        if ((pos.y + bullet->spriteHeight) > visibleSize.height || (pos.y < 0))
                        {
                            bullet->removeComponent("CollisionComponent");
                            removeChild(bullet);
                            bulletPool->returnBulletToPool(bullet);
                        }
                        if ((pos.x + bullet->spriteWidth) > visibleSize.width || (pos.x < 0))
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

        if (enemyPool != nullptr && bulletPool != nullptr && player != nullptr)
        {


            for (Enemy* enemy : enemyPool->enemyPool) {
                if (enemy != nullptr)
                {
                    auto pos = enemy->getPosition();

                    if (enemy->isActive)
                    {

                        if (enemy->getComponent("CollisionComponent") == NULL)
                        {
                            enemy->addComponent(CollisionComponent::createBox(enemy->getContentSize().width, enemy->getContentSize().height));

                        }
                        enemy->update(dt);

                        if (pos.y > visibleSize.height)
                        {
                            enemy->setPositionY(enemy->getPositionY() - visibleSize.height);
                        }
                        if (pos.y < 0)
                        {
                            enemy->setPositionY(enemy->getPositionY() + visibleSize.height);
                        }
                        if (pos.x > visibleSize.width)
                        {
                            enemy->setPositionX(enemy->getPositionX() - visibleSize.width);
                        }
                        if (pos.x < 0)
                        {
                            enemy->setPositionX(enemy->getPositionX() + visibleSize.width);

                        }

                        auto collisionEnemy = dynamic_cast<CollisionComponent*>((enemy)->getComponent("CollisionComponent"));
                        auto collisionPlayer = dynamic_cast<CollisionComponent*>((player)->getComponent("CollisionComponent"));

                        for (Bullet* bullet : bulletPool->bulletPool)
                        {
                            if (bullet != nullptr && bullet->isActive)
                            {
                                auto collisionBullet = dynamic_cast<CollisionComponent*>((bullet)->getComponent("CollisionComponent"));


                                if (collisionEnemy && collisionBullet && collisionBullet->IsColliding(collisionEnemy))
                                {
                                    //Abomination code to get explosion animation to play without runAction asserting _referenceCount > 0
                                    Vector<SpriteFrame*> animFrames = getAnimation("tile%01d.png", 71);

                                    Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.01f);
                                    Animate* animate = Animate::create(animation);
                                    Sprite* sprite = Sprite::create();
                                    sprite->setPosition(enemy->getPositionX(), enemy->getPositionY());
                                    sprite->runAction(animate);
                                    addChild(sprite);
                                    
                                    //remove enemy from active group, return to available pool
                                    enemy->removeComponent("CollisionComponent");
                                    removeChild(enemy);
                                    enemyPool->returnEnemyToPool(enemy);
                                    ++enemiesDefeated;
                                    
                                    bullet->removeComponent("CollisionComponent");
                                    removeChild(bullet);
                                    bulletPool->returnBulletToPool(bullet);
                                    break;
                                }

                            }

                        }
                        if (enemy->isActive && collisionEnemy && collisionEnemy->IsColliding(collisionPlayer))
                        {
                            Vector<SpriteFrame*> animFrames = getAnimation("tile%01d.png", 71);
                           
                            Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.01f);
                            Animate* animate = Animate::create(animation);
                            Sprite* sprite = Sprite::create();
                            sprite->setPosition(enemy->getPositionX(), enemy->getPositionY());
                            sprite->runAction(animate);
                            addChild(sprite);

                            enemy->removeComponent("CollisionComponent");
                            removeChild(enemy);
                            enemyPool->returnEnemyToPool(enemy);
                            myHealth->currentHealth -= 10;
                            ++enemiesDefeated;

                        }
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
        
    }
    if (curGameState == GameState::Pause)
    {
        if (r)
        {
            player->removeFromParent();
            player = nullptr;
            delete(player);
            debugDrawEnabled = false;
            for (Bullet* bullet : bulletPool->bulletPool)
            {
                bullet->removeFromParent();
                bullet = nullptr;
                delete(bullet);
            }
            bulletPool = nullptr;
            delete(bulletPool);
            for (Enemy* enemy: enemyPool->enemyPool)
            {
                enemy->removeFromParent();
                enemy = nullptr;
                delete(enemy);
            }
            enemyPool = nullptr;
            delete(enemyPool);
            myHealth->emptyBar->removeFromParent();
            myHealth->fillBar->removeFromParent();
            myHealth = nullptr;
            delete(myHealth);
            

            init();
            curGameState = GameState::Menu;
        }

    }
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

void CollisionTestScene::RunAnimOnce(Sprite* sprite)
{
    sprite->setPosition(sprite->getPositionX(), sprite->getPositionY());
    //this causes the assert _referenceCount > 0 if called from update with enemy* as param
    //sprite->runAction(animate);
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

Vector<SpriteFrame*> CollisionTestScene::getAnimation(const char* format, int count)
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