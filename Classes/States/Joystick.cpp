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
    
    Size sz = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    float scaleBG = 0.5;
    float scaleThumb = 1.1;
    //Joystick Left
    _velocityLeft = Vec2::ZERO;
    _angleLeft = 0.f;
    _isPressLeft = false;
    
    _bgLeft = Sprite::create("OptionAssets/joystickBG.png");
    _bgLeft->setScale(scaleBG);
    this->addChild(_bgLeft, 0);
        
    _thumbLeft = Sprite::create("OptionAssets/joystickThumb.png");
    _thumbLeft->setScale(scaleThumb);
    this->addChild(_thumbLeft, 1);
    
    Joystick::radius = _bgLeft->getBoundingBox().size.width / 2.f;
    Joystick::thumb_radius = _thumbLeft->getBoundingBox().size.width / 2.f;
    _centerLeft = Vec2(origin.x + Joystick::radius + Joystick::offset_x, origin.y + Joystick::radius + Joystick::offset_y);
    
    _bgLeft->setPosition(_centerLeft);
    _thumbLeft->setPosition(_centerLeft);
    
    //Joystick2
    _velocityRight = Vec2::ZERO;
    _angleRight = 0.f;
    _isPressRight = false;
    
    _bgRight = Sprite::create("OptionAssets/joystickBG.png");
    _bgRight->setScale(scaleBG);
    this->addChild(_bgRight, 0);
    
    _thumbRight = Sprite::create("OptionAssets/joystickThumb.png");
    _thumbRight->setScale(scaleThumb);
    this->addChild(_thumbRight, 1);
    
    _centerRight = Vec2(origin.x + sz.width - Joystick::radius - Joystick::offset_x, _centerLeft.y);
    
    _bgRight->setPosition(_centerRight);
    _thumbRight->setPosition(_centerRight);
    
    
//    auto touch = EventListenerTouchOneByOne::create();
//    touch->onTouchBegan = CC_CALLBACK_2(Joystick::touchBegan, this);
//    touch->onTouchMoved = CC_CALLBACK_2(Joystick::touchMoved, this);
//    touch->onTouchEnded = CC_CALLBACK_2(Joystick::touchEnded, this);
//    touch->onTouchCancelled = CC_CALLBACK_2(Joystick::onTouchCancelled, this);
    auto touch = EventListenerTouchAllAtOnce::create();
    touch->onTouchesBegan = CC_CALLBACK_2(Joystick::touchBegan, this);
    touch->onTouchesMoved = CC_CALLBACK_2(Joystick::touchMoved, this);
    touch->onTouchesEnded = CC_CALLBACK_2(Joystick::touchEnded, this);
    touch->onTouchesCancelled = CC_CALLBACK_2(Joystick::touchCancelled, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touch, this);
    
    
    return true;
}

void Joystick::updateVelocity(Vec2 point, side s)
{
    // calculate Angle and length
    Vec2 center = (s == side::LEFT ? _centerLeft : _centerRight);
    
    Vec2 offset = point - center;
    
    float distance = offset.length();
    float angle = atan2(offset.y ,offset.x); // in radians
    
    if(distance > Joystick::radius){
        offset.x = cos(angle) * Joystick::radius;
        offset.y = sin(angle) * Joystick::radius;
    }
    
    if(s == side::LEFT)
    {
        _velocityLeft = Vec2(offset.x/(float)Joystick::radius, offset.y/(float)Joystick::radius);
        _angleLeft = atan2(_velocityLeft.y, _velocityLeft.x);
        _angleLeft = CC_RADIANS_TO_DEGREES(-angle) + 90.f;
    }
    else if (s == side::RIGHT)
    {
        _velocityRight = Vec2(offset.x/(float)Joystick::radius, offset.y/(float)Joystick::radius);
        _angleRight = atan2(_velocityRight.y, _velocityRight.x);
        _angleRight = CC_RADIANS_TO_DEGREES(-angle) + 90.f;
    }
    
    if(distance > Joystick::thumb_radius)
    {
        point.x = center.x + cos(angle) * Joystick::thumb_radius;
        point.y = center.y + sin(angle) * Joystick::thumb_radius;
    }
    
    if(s == side::LEFT)_thumbLeft->setPosition(point);
    else if(s == side::RIGHT)_thumbRight->setPosition(point);
}

void Joystick::resetJoystick(side s)
{
    Vec2 center = (s == side::LEFT ? _centerLeft : _centerRight);
    this->updateVelocity(center, s);
}

bool Joystick::handleLastTouch(side s)
{
    bool wasPressed = (s == side::LEFT ? _isPressLeft : _isPressRight);
    if(wasPressed)
    {
        this->resetJoystick(s);
        if(s == side::LEFT)_isPressLeft = false;
        else if(s == side::RIGHT)_isPressRight = false;
    }
    
    return wasPressed;
}

bool isPointInCircle(const Vec2& point, const Vec2& center, const float& radius)
{
    return (point - center).length() <= radius;
}

Vec2 convertPointTouchToGLView(Touch* touch)
{
    return Director::getInstance()->convertToGL(touch->getLocationInView());
}

Touch* getTouchInJoystick(const vector<Touch*>& touches, const Rect& box)
{
    Touch* touchPos = nullptr;
    for(auto& touch : touches)
    {
        Vec2 pos = touch->getLocation();
        if(box.containsPoint(pos))
        {
            touchPos = touch;
            break;
        }
    }
    
    return touchPos;
}

//bool Joystick::touchBegan(Touch* touch, Event*)
void Joystick::touchBegan(const vector<Touch*>& touches, Event*)
{
    auto touchLeft = getTouchInJoystick(touches, _bgLeft->getBoundingBox());
    auto touchRight = getTouchInJoystick(touches, _bgRight->getBoundingBox());
    
    if(touchLeft)
    {
        Vec2 touchPosLeft = convertPointTouchToGLView(touchLeft);
        if(isPointInCircle(touchPosLeft, _centerLeft, Joystick::radius))
        {
            _isPressLeft = true;
            this->updateVelocity(touchPosLeft, side::LEFT);
        }
    }
    
    if(touchRight)
    {
        Vec2 touchPosRight = convertPointTouchToGLView(touchRight);
        if(isPointInCircle(touchPosRight, _centerRight, Joystick::radius))
        {
            _isPressRight = true;
            this->updateVelocity(touchPosRight, side::RIGHT);
        }
    }
}

//void Joystick::touchMoved(Touch* touch, Event*)
void Joystick::touchMoved(const vector<Touch*>& touches, Event*)
{
    if(_isPressLeft)
    {
        auto touch = getTouchInJoystick(touches, _bgLeft->getBoundingBox());
        if(touch)
        {
            Vec2 touchPos = convertPointTouchToGLView(touch);
            this->updateVelocity(touchPos, side::LEFT);
        }
    }
    
    if(_isPressRight)
    {
        auto touch = getTouchInJoystick(touches, _bgRight->getBoundingBox());
        if(touch)
        {
            Vec2 touchPos = convertPointTouchToGLView(touch);
            this->updateVelocity(touchPos, side::RIGHT);
        }
    }
}

//void Joystick::touchEnded(Touch* touch, Event*)
void Joystick::touchEnded(const vector<Touch*>& touches, Event*)
{
    Size sz = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    bool left = false, right = false;
    for(auto& touch : touches)
    {
        float locationX = touch->getLocation().x;
        float centerX = origin.x + sz.width / 2.f;
        if(locationX <= centerX)left = true;
        if(locationX >= centerX)right = true;
    }
    
    if(left)handleLastTouch(side::LEFT);
    if(right)handleLastTouch(side::RIGHT);
}

//void Joystick::touchCancelled(Touch* touch, Event*)
void Joystick::touchCancelled(const vector<Touch*>& touches, Event* e)
{
    touchEnded(touches, e);
}

#endif
