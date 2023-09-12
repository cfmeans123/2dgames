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

    //auto mousecontroller = new MouseControllerComponent
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
      
      
    }
    else
    {
      leftKeyCode = EventKeyboard::KeyCode::KEY_LEFT_ARROW;
      rightKeyCode = EventKeyboard::KeyCode::KEY_RIGHT_ARROW;
      upKeyCode = EventKeyboard::KeyCode::KEY_UP_ARROW;
      downKeyCode = EventKeyboard::KeyCode::KEY_DOWN_ARROW;
    }
    rKeyCode = EventKeyboard::KeyCode::KEY_R;
    oneKeyCode = EventKeyboard::KeyCode::KEY_1;
    twoKeyCode = EventKeyboard::KeyCode::KEY_2;
    threeKeyCode = EventKeyboard::KeyCode::KEY_3;
    fourKeyCode = EventKeyboard::KeyCode::KEY_4;
    shiftKeyCode = EventKeyboard::KeyCode::KEY_LEFT_SHIFT;
    escapeKeyCode = EventKeyboard::KeyCode::KEY_ESCAPE;
    leftClickKeyCode = EventMouse::MouseButton::BUTTON_LEFT;
    rightClickKeyCode = EventMouse::MouseButton::BUTTON_RIGHT;
  }

  virtual bool init()
  {
    _name = "KeyboardControllerComponent";
    return true;
  }

  void initInput()
  {
    auto keyboardListener = EventListenerKeyboard::create();
    auto mouseListener = EventListenerMouse::create();
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
      else if (keyCode == escapeKeyCode)
      {
          escape = true;
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
      else if (keyCode == escapeKeyCode)
      {
          escape = false;
      }
    };

    mouseListener->onMouseDown = [=](EventMouse* buttonCode)
    {
        if (buttonCode->getMouseButton() == leftClickKeyCode)
        {
            leftClick = true;
        }
        if (buttonCode->getMouseButton() == rightClickKeyCode)
        {
            rightClick = true;
        }
    };

    mouseListener->onMouseUp = [=](EventMouse* buttonCode)
    {
        if (buttonCode->getMouseButton() == leftClickKeyCode)
        {
            leftClick = false;
        }
        if (buttonCode->getMouseButton() == rightClickKeyCode)
        {
            rightClick = false;
        }
    };

    auto scene = this->getOwner()->getScene();
    auto dispatcher = Director::getInstance()->getEventDispatcher();

    dispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, scene);
    dispatcher->addEventListenerWithSceneGraphPriority(mouseListener, scene);
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
  bool IsEscapePressed()
  {
      return escape;
  }
  bool IsLeftClickPressed()
  {
      return leftClick;
  }
  bool IsRightClickPressed()
  {
      return rightClick;
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
  bool leftClick = false;
  bool rightClick = false;
  bool escape = false;
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
  EventKeyboard::KeyCode escapeKeyCode;
  EventMouse::MouseButton leftClickKeyCode;
  EventMouse::MouseButton rightClickKeyCode;


};