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
    _body(nullptr),
    _object(nullptr)
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
    switch (type)
    {
        case STATIC:
            _body->SetType(b2_staticBody);
            break;
        case DYNAMIC:
            _body->SetType(b2_dynamicBody);
            break;
        case KINEMATIC:
            _body->SetType(b2_kinematicBody);
            break;
        default:
            break;
    }
}

RigidBody::type RigidBody::getType() const
{
    RigidBody::type type = type::DYNAMIC;
    switch (_body->GetType())
    {
        case b2_staticBody:
            type = STATIC;
            break;
        case b2_dynamicBody:
            type = DYNAMIC;
            break;
        case b2_kinematicBody:
            type = KINEMATIC;
            break;
        default:
            break;
    }
    
    return type;
}

void RigidBody::setVelocity(const Vec2& vec)
{
    _body->SetLinearVelocity(b2Vec2(vec.x, vec.y));
}

Vec2 RigidBody::getVelocity() const
{
    b2Vec2 vec = _body->GetLinearVelocity();
    return Vec2(vec.x, vec.y);
}
