//
//  Joystick.cpp
//  ShooterTopDown2D
//
//  Created by Khanh on 5/4/19.
//

#include "Joystick.h"

#if USE_JOYSTICK

float Joystick::offset_x = 5.f;
float Joystick::offset_y = 5.f;
float Joystick::radius = 64.f;
float Joystick::thumb_radius = 26.f;

bool Joystick::init()
{
    if(!Layer::init())
        return false;
    
    _velocity = Vec2::ZERO;
    _angle = 0.f;
    Sprite* bg = Sprite::create("OptionAssets/joystickBG.png");
    bg->setScale(0.45f);
    this->addChild(bg, 0);
        
    _thumb = Sprite::create("OptionAssets/joystickThumb.png");
//    _thumb->setScale(0.9f);
    this->addChild(_thumb, 1);
    
    Joystick::radius = bg->getBoundingBox().size.width / 2.f;
    Joystick::thumb_radius = _thumb->getBoundingBox().size.width / 2.f;
    _center = Vec2(Joystick::radius + Joystick::offset_x, Joystick::radius + Joystick::offset_y);
    
    bg->setPosition(_center);
    _thumb->setPosition(_center);
    
    auto touch = EventListenerTouchOneByOne::create();
    touch->onTouchBegan = CC_CALLBACK_2(Joystick::touchBegan, this);
    touch->onTouchMoved = CC_CALLBACK_2(Joystick::touchMoved, this);
    touch->onTouchEnded = CC_CALLBACK_2(Joystick::touchEnded, this);
    touch->onTouchCancelled = CC_CALLBACK_2(Joystick::onTouchCancelled, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touch, this);
    
    return true;
}

void Joystick::updateVelocity(Vec2 point)
{
    // calculate Angle and length
    Vec2 offset = point - _center;
    
    float distance = offset.length();
    float angle = atan2(offset.y ,offset.x); // in radians
    
    if(distance > Joystick::radius){
        offset.x = cos(angle) * Joystick::radius;
        offset.y = sin(angle) * Joystick::radius;
    }
    
    _velocity = Vec2(offset.x/(float)Joystick::radius, offset.y/(float)Joystick::radius);
    _angle = atan2(_velocity.y, _velocity.x);
    _angle = CC_RADIANS_TO_DEGREES(-angle) + 90.f;
    CCLOG("angle : %f", _angle);
    
    if(distance > Joystick::thumb_radius)
    {
        point.x = _center.x + cos(angle) * Joystick::thumb_radius;
        point.y = _center.y + sin(angle) * Joystick::thumb_radius;
    }
    
    _thumb->setPosition(point);
}

void Joystick::resetJoystick()
{
    this->updateVelocity(_center);
}

bool Joystick::handleLastTouch()
{
    bool wasPressed = _isPress;
    if(wasPressed)
    {
        this->resetJoystick();
        _isPress = false;
    }
    
    return wasPressed;
}

bool isPointInCircle(const Vec2& point, const Vec2& center, const float& radius)
{
    return (point - center).length() <= radius;
}

bool Joystick::touchBegan(Touch* touch, Event*)
{
    Vec2 touchPos = touch->getLocationInView();
    touchPos = Director::getInstance()->convertToGL(touchPos);
    
    if(isPointInCircle(touchPos, _center, Joystick::radius))
    {
        _isPress = true;
        this->updateVelocity(touchPos);
    }
    
    return true;
}

void Joystick::touchMoved(Touch* touch, Event*)
{
    if(_isPress)
    {
        Vec2 touchPos = touch->getLocationInView();
        touchPos = Director::getInstance()->convertToGL(touchPos);
        this->updateVelocity(touchPos);
    }
}

void Joystick::touchEnded(Touch* touch, Event*)
{
    this->handleLastTouch();
}

void Joystick::touchCancelled(Touch* touch, Event*)
{
    this->handleLastTouch();
}

#endif
