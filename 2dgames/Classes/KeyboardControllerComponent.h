#pragma once

#include "cocos2d.h"

USING_NS_CC;

class KeyboardControllerComponent : public Component
{
public:
  typedef int KeyboardInputType;
  static constexpr int WASD = 0;
  static constexpr int ARROWS = 1;

  static KeyboardControllerComponent* create(const KeyboardInputType& type)
  {
    auto controller = new KeyboardControllerComponent(type);
    controller->init();
    controller->autorelease();
    return controller;
  }

  KeyboardControllerComponent(const KeyboardInputType& type)
  {
    if (type == WASD)
    {
      leftKeyCode = EventKeyboard::KeyCode::KEY_A;
      rightKeyCode = EventKeyboard::KeyCode::KEY_D;
      upKeyCode = EventKeyboard::KeyCode::KEY_W;
      downKeyCode = EventKeyboard::KeyCode::KEY_S;
      shiftKeyCode = EventKeyboard::KeyCode::KEY_LEFT_SHIFT;
      
    }
    else
    {
      leftKeyCode = EventKeyboard::KeyCode::KEY_LEFT_ARROW;
      rightKeyCode = EventKeyboard::KeyCode::KEY_RIGHT_ARROW;
      upKeyCode = EventKeyboard::KeyCode::KEY_UP_ARROW;
      downKeyCode = EventKeyboard::KeyCode::KEY_DOWN_ARROW;
      shiftKeyCode = EventKeyboard::KeyCode::KEY_RIGHT_CTRL;
    }
    rKeyCode = EventKeyboard::KeyCode::KEY_R;
    oneKeyCode = EventKeyboard::KeyCode::KEY_1;
    twoKeyCode = EventKeyboard::KeyCode::KEY_2;
    threeKeyCode = EventKeyboard::KeyCode::KEY_3;
    fourKeyCode = EventKeyboard::KeyCode::KEY_4;
  }

  virtual bool init()
  {
    _name = "KeyboardControllerComponent";
    return true;
  }

  void initInput()
  {
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event)
    {
      if (keyCode == leftKeyCode)
      {
        left = true;
      }
      else if (keyCode == rightKeyCode)
      {
        right = true;
      }
      else if (keyCode == upKeyCode)
      {
        up = true;
      }
      else if (keyCode == downKeyCode)
      {
        down = true;
      }
      else if (keyCode == rKeyCode)
      {
          rkey = true;
      }
      else if (keyCode == shiftKeyCode)
      {
          shiftkey = true;
      }
      else if (keyCode == shiftKeyCode)
      {
          shiftkey = true;
      }
      else if (keyCode == oneKeyCode)
      {
          one = true;
      }
      else if (keyCode == twoKeyCode)
      {
          two = true;
      }
      else if (keyCode == threeKeyCode)
      {
          three = true;
      }
      else if (keyCode == fourKeyCode)
      {
          four = true;
      }
    };
    keyboardListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event)
    {
      if (keyCode == leftKeyCode)
      {
        left = false;
      }
      else if (keyCode == rightKeyCode)
      {
        right = false;
      }
      else if (keyCode == upKeyCode)
      {
        up = false;
      }
      else if (keyCode == downKeyCode)
      {
        down = false;
      }
      else if (keyCode == rKeyCode)
      {
          rkey = false;
      }
      else if (keyCode == shiftKeyCode)
      {
          shiftkey = false;
      }
      else if (keyCode == oneKeyCode)
      {
          one = false;
      }
      else if (keyCode == twoKeyCode)
      {
          two = false;
      }
      else if (keyCode == threeKeyCode)
      {
          three = false;
      }
      else if (keyCode == fourKeyCode)
      {
          four = false;
      }
    };

    auto scene = this->getOwner()->getScene();
    auto dispatcher = Director::getInstance()->getEventDispatcher();

    dispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, scene);
  }

  bool IsLeftPressed()
  {
    return left;
  }

  bool IsRightPressed()
  {
    return right;
  }

  bool IsUpPressed()
  {
    return up;
  }

  bool IsDownPressed()
  {
    return down;
  }
  bool IsRPressed()
  {
      return rkey;
  }
  bool IsShiftPressed()
  {
      return shiftkey;
  }
  bool IsOnePressed()
  {
      return one;
  }
  bool IsTwoPressed()
  {
      return two;
  }
  bool IsThreePressed()
  {
      return three;
  }
  bool IsFourPressed()
  {
      return four;
  }

  bool left = false;
  bool right = false;
  bool up = false;
  bool down = false;
  bool rkey = false;
  bool shiftkey = false;
  bool one = false;
  bool two = false;
  bool three = false;
  bool four = false;
private:
  EventKeyboard::KeyCode leftKeyCode;
  EventKeyboard::KeyCode rightKeyCode;
  EventKeyboard::KeyCode upKeyCode;
  EventKeyboard::KeyCode downKeyCode;
  EventKeyboard::KeyCode rKeyCode;
  EventKeyboard::KeyCode shiftKeyCode;
  EventKeyboard::KeyCode oneKeyCode;
  EventKeyboard::KeyCode twoKeyCode;
  EventKeyboard::KeyCode threeKeyCode;
  EventKeyboard::KeyCode fourKeyCode;
};