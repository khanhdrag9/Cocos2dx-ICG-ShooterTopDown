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
    _damge(0),
    _liveDuration(999)
{
}

BulletBasic::~BulletBasic()
{
    
}

void BulletBasic::init(CharacterCreation* creation)
{
    Character::init(creation);
}

void BulletBasic::update(float dt)
{
    _liveDuration -= dt;
    if(_liveDuration <= 0)
    {
        CCLOG("damage : %d", _damge);
        this->destroy();
    }
}

shared_ptr<BulletBasic> BulletBasic::createBulletBasic(const string& sprite, const Vec2& position, const float& angle, const Vec2& speed, bool forceShoot)
{
    shared_ptr<BulletBasic> obj = make_shared<BulletBasic>();
    obj->_name = constants::object_bullet_basic;
    
    obj->_sprite = Sprite::create(sprite);
    
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

void BulletBasic::setDamege(int damge)
{
    _damge = damge;
}

int BulletBasic::getDamge() const
{
    return _damge;
}

void BulletBasic::setParticle(const string& particleStr)
{
    ParticleSystemQuad* particle = ParticleSystemQuad::create(particleStr.c_str());
    if(particle)
    {
        Size objSize = _sprite->getContentSize();
        particle->setPosition(objSize.width / 2.f, objSize.height / 2.f);
        particle->setOpacity(200.f);
        particle->setScale(0.7f);
        _sprite->addChild(particle);
    }
}
