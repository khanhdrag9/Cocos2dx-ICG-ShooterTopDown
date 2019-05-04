//
//  CustomMouse.cpp
//  ShooterTopDown2D
//
//  Created by Khanh on 5/4/19.
//

#include "CustomMouse.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
bool CustomMouse::init()
{
    if(!Layer::init())
        return false;
    

    Director::getInstance()->getOpenGLView()->setCursorVisible(false);
    auto _screenSize = Director::getInstance()->getVisibleSize();
    auto _origin = Director::getInstance()->getVisibleOrigin();
    
    //for AIM
    Vec2 centerpos = Vec2(_origin.x + _screenSize.width * 0.5f, _origin.y + _screenSize.height / 2.f);
    _cursor = Sprite::create("aim/aim1.png");
    _cursor->setScale(0.25f);
    _cursor->setPosition(centerpos);
    this->addChild(_cursor);
    
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = [this](EventMouse* mouse){
        this->_cursor->setPosition(mouse->getCursorX(), mouse->getCursorY());
    };
    mouseListener->onMouseMove = [this](EventMouse* mouse){
        this->_cursor->setPosition(mouse->getCursorX(), mouse->getCursorY());
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
    
    return true;
}
#endif
