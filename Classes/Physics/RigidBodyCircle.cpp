//
//  RigidBodyCircle.cpp
//  Shooter
//
//  Created by Khanh on 3/16/19.
//

#include "RigidBodyCircle.h"
#include "../Characters/Character.h"
#include "../Game.h"

shared_ptr<RigidBodyCircle> RigidBodyCircle::createRigidBodyCircle(shared_ptr<Character> object)
{
    shared_ptr<RigidBodyCircle> body = make_shared<RigidBodyCircle>();
    body->init();   //init func haven't been used yet
    body->_radius = (object->_sprite->getContentSize().width / 2.f) * 0.9f;
    body->_object = object;
    
    return body;
}

void RigidBodyCircle::init()
{
    RigidBody::init();
}

void RigidBodyCircle::update(float dt)
{
    RigidBody::update(dt);
}
