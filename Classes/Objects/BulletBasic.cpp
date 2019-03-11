//
//  BulletBasic.cpp
//  Shooter
//
//  Created by Khanh on 3/11/19.
//

#include "BulletBasic.h"
#include "../Defines/constants.h"
#include "../Game.h"

BulletBasic::BulletBasic():
    _sprite(nullptr),
    _rigidBody(nullptr),
    _speed(Vec2(0,0))
{
}

BulletBasic::~BulletBasic()
{
    _rigidBody = nullptr;
    CC_SAFE_DELETE(_sprite);
}

shared_ptr<BulletBasic> BulletBasic::createBulletBasic(const Vec2& position, const float& angle, const Vec2& speed, bool forceShoot)
{
    shared_ptr<BulletBasic> obj = make_shared<BulletBasic>();
    obj->_name = constants::object_bullet_basic;
    
    obj->_sprite = Sprite::create("bullet/bullet1.png");
    
    //physics - note*:set to player before set character's rotation
    PhysicsBody* body = PhysicsBody::createBox(obj->_sprite->getContentSize());
    obj->_sprite->setPhysicsBody(body);
    obj->_rigidBody = body;
    
    obj->_sprite->setPosition(position);
    obj->_sprite->setRotation(angle);
    obj->_speed = speed;
    
    if(forceShoot)
        obj->_rigidBody->setVelocity(obj->_speed);
    
    Game::getInstance()->getCurrentState()->addChild(obj->_sprite);
    
    return obj;
}

void BulletBasic::Shoot(const Vec2& velocity)
{
    _speed = velocity;
    _rigidBody->setVelocity(_speed);
}

void BulletBasic::Shoot()
{
    _rigidBody->setVelocity(_speed);
}

Vec2 BulletBasic::getSpeed() const
{
    return _speed;
}

const string& BulletBasic::getName() const
{
    return _name;
}
