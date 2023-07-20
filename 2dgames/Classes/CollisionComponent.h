#pragma once

#include "cocos2d.h"
#include <limits>


USING_NS_CC;

class CollisionComponent : public Component
{
public:
  enum Type
  {
    Circle = (1 << 0),
    Box = (1 << 1),
    Point = (1 << 2)
  };

  static CollisionComponent* createCircle(float radius)
  {
    auto collisionComponent = new CollisionComponent(radius);
    collisionComponent->init();
    collisionComponent->autorelease();
    return collisionComponent;
  }

  static CollisionComponent* createBox(float width, float height)
  {
    auto collisionComponent = new CollisionComponent(width, height);
    collisionComponent->init();
    collisionComponent->autorelease();
    return collisionComponent;
  }

  static CollisionComponent* createDot()
  {
    auto collisionComponent = new CollisionComponent();
    collisionComponent->init();
    collisionComponent->autorelease();
    return collisionComponent;
  }

  virtual bool init()
  {
    _name = "CollisionComponent";
    return true;
  }

  CollisionComponent() : _type(Type::Point), _radius(0), _height(0), _width(0)
  {
  }
  CollisionComponent(float radius) : _type(Type::Circle), _radius(radius), _height(0), _width(0)
  {
  }

  CollisionComponent(float width, float height) : _type(Type::Box), _radius(0), _width(width), _height(height)
  {
  }

  bool IsColliding(CollisionComponent* other)
  {
    auto collisionMask = this->_type | other->_type;

    if (collisionMask & Point)
    {
      if (collisionMask & Box)
      {
        if (this->_type & Point)
        {
          auto& c2 = other->getOwner()->getPosition();
          auto b2 = AABB(c2.x - other->_width / 2.0f, c2.y - other->_height / 2.0f, other->_width, other->_height);

          return checkCollision(this->getOwner()->getPosition(), b2);
        }
        else
        {
          auto& c1 = this->getOwner()->getPosition();
          auto b1 = AABB(c1.x - this->_width / 2.0f, c1.y - this->_height / 2.0f, this->_width, this->_height);

          return checkCollision(other->getOwner()->getPosition(), b1);
        }
      }
      else if (collisionMask & Circle)
      {
        if (this->_type & Point)
        {
          return checkCollision(this->getOwner()->getPosition(), other->getOwner()->getPosition(), other->GetRadius());
        }
        else
        {
          return checkCollision(other->getOwner()->getPosition(), this->getOwner()->getPosition(), this->GetRadius());
        }
      }
      else
      {
        return false;
      }
    }
    else if (collisionMask & Box)
    {
      if (collisionMask & Circle)
      {
        if (this->_type & Box)
        {
          auto& c1 = this->getOwner()->getPosition();
          auto b1 = AABB(c1.x - this->_width / 2.0f, c1.y - this->_height / 2.0f, this->_width, this->_height);
          return checkCollision(b1, other->getOwner()->getPosition(), other->GetRadius());
        }
        else
        {
          auto& c2 = other->getOwner()->getPosition();
          auto b2 = AABB(c2.x - other->_width / 2.0f, c2.y - other->_height / 2.0f, other->_width, other->_height);
          return checkCollision(b2, this->getOwner()->getPosition(), this->GetRadius());
        }
      }
      else
      {
        auto& c1 = this->getOwner()->getPosition();
        auto& c2 = other->getOwner()->getPosition();

        auto b1 = AABB(c1.x - this->_width / 2.0f, c1.y - this->_height / 2.0f, this->_width, this->_height);
        auto b2 = AABB(c2.x - other->_width / 2.0f, c2.y - other->_height / 2.0f, other->_width, other->_height);

        return checkCollision(b1, b2);
      }
    }
    else if (collisionMask & Circle)
    {
      return checkCollision(this->getOwner()->getPosition(), this->GetRadius(), other->getOwner()->getPosition(), other->GetRadius());
    }

    return false;
  }

  void SetColliding(bool colliding)
  {
    _isColliding = colliding;
  }

  Type GetCollisionType() const
  {
    return _type;
  }

  float GetRadius() const
  {
    return _radius;
  }

  float GetWidth() const
  {
    return _width;
  }

  float GetHeight() const
  {
    return _height;
  }

  bool IsColliding() const
  {
    return _isColliding;
  }

private:
  Type _type;
  float _radius;
  float _width;
  float _height;
  bool _isColliding;

  struct AABB
  {
    float _x;
    float _y;
    float _width;
    float _height;

    AABB(float x, float y, float width, float height) : _x(x), _y(y), _width(width), _height(height) {}
  };

  /// Point to AABB
  static bool checkCollision(const Vec2& p, const AABB& b);

  /// Point to Circle
  static bool checkCollision(const Vec2& p, const Vec2& c, float r);

  /// AABB to AABB
  static bool checkCollision(const AABB& b1, const AABB& b2);

  /// Circle to Circle
  static bool checkCollision(const Vec2& c1, float r1, const Vec2& c2, float r2);

  /// Circle to AABB
  static bool checkCollision(const AABB& b1, const Vec2& c2, float r2);

  static bool intersectCircle(const Vec2& c, float r, Vec2 p1, Vec2 p2);

  static float TriangleArea(Vec2 const& a, Vec2 const& b, Vec2 const& c);
};