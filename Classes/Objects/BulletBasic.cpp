//
//  BulletBasic.cpp
//  Shooter
//
//  Created by Khanh on 3/11/19.
//

#include "BulletBasic.h"
#include "../Defines/constants.h"
#include "../Game.h"
#include "../Physics/RigidBody.h"
#include "../Physics/RigidWorld.h"
#include "../Resource/ResourceManager.h"

BulletBasic::BulletBasic(): Character(),
    _speed(Vec2(0,0)),
    _isDestroy(false),
    _damge(20)
{
}

BulletBasic::~BulletBasic()
{
    
}

void BulletBasic::init()
{
    Character::init();
}

void BulletBasic::update(float dt)
{
    
}

shared_ptr<BulletBasic> BulletBasic::createBulletBasic(const Vec2& position, const float& angle, const Vec2& speed, bool forceShoot)
{
    shared_ptr<BulletBasic> obj = make_shared<BulletBasic>();
    obj->_name = constants::object_bullet_basic;
    
    res::define asset = res::define::BULLET_RED_1;
    obj->_sprite = Sprite::create(ResourceManager::getInstance()->at(asset));
    
    //physics - note*:set to player before set character's rotation
    obj->_rigidBody = Game::getInstance()->getRigidWord()->createRigidBodyPolygon(obj);
    obj->_rigidBody->setTag(RigidBody::tag::BULLET);
    
    obj->_sprite->setPosition(position);
    obj->_sprite->setRotation(angle);
    obj->_speed = speed;
    
    if(forceShoot)
        obj->_rigidBody->_velocity = obj->_speed;
    
    Game::getInstance()->getCurrentState()->addChild(obj->_sprite);
    
    return obj;
}

void BulletBasic::Shoot(const Vec2& velocity)
{
    _speed = velocity;
    _rigidBody->_velocity = _speed;
}

void BulletBasic::Shoot()
{
    _rigidBody->_velocity = _speed;
}

Vec2 BulletBasic::getSpeed() const
{
    return _speed;
}

int BulletBasic::getDamge() const
{
    return _damge;
}
