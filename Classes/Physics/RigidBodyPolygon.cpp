//
//  RigidBodyPolygon.cpp
//  Shooter
//
//  Created by Khanh on 3/13/19.
//

#include "RigidBodyPolygon.h"
#include "../Defines/Templates.h"
#include "../Characters/Character.h"
#include "../Game.h"

shared_ptr<RigidBodyPolygon> RigidBodyPolygon::createRigidBodyPolygon(shared_ptr<Character> object)
{
    unique_ptr<RigidBodyPolygon> body = pointer::make_unique<RigidBodyPolygon>();
    body->init();   //init func haven't been used yet
    body->_rect = object->_sprite->getBoundingBox();
    body->_object = object;
    
    return body;
}

void RigidBodyPolygon::init()
{
    RigidBody::init();
}

void RigidBodyPolygon::update(float dt)
{
    RigidBody::update(dt);
    
    if(_object)
        _rect = _object->_sprite->getBoundingBox();
}
