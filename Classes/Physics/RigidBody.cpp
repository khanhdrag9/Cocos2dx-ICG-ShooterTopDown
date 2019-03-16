//
//  RigidBody.cpp
//  Shooter
//
//  Created by Khanh on 3/13/19.
//

#include "RigidBody.h"
#include "../Characters/Character.h"
#include "../Defines/Templates.h"

RigidBody::RigidBody():
_object(nullptr),
_velocity(Vec2(0,0)),
_type(RigidBody::type::DYNAMIC),
_tag(-1)
{
}

RigidBody::~RigidBody()
{
    _object = nullptr;
}

void RigidBody::init()
{
}

void RigidBody::update(float dt)
{
    
}

shared_ptr<Character> RigidBody::getObject() const
{
    return _object;
}

void RigidBody::setType(const RigidBody::type& type)
{
    _type = type;
}

RigidBody::type RigidBody::getType() const
{
    return _type;
}

void RigidBody::setTag(int tag)
{
    _tag = tag;
}

int RigidBody::getTag() const
{
    return _tag;
}
