//
//  RigidWorld.cpp
//  Shooter
//
//  Created by Khanh on 3/16/19.
//

#include "RigidWorld.h"
#include "../Objects/ObjectsPool.h"
#include "RigidBody.h"
#include "RigidBodyPolygon.h"
#include "../Characters/Character.h"


RigidWorld::RigidWorld()
{
}

RigidWorld::~RigidWorld()
{
    
}

void RigidWorld::update(float dt)
{
    for(auto& x : _listRigidBodies)
    {
        x->update(dt);
    }
    
    for(auto& x : _listRigidBodies)
    {
        if(x->_object)
        {
            if(x->getTag() == RigidBody::tag::BULLET_PLAYER)
            {
                int a =4;
            }
            
            Vec2 currentObjPos = x->_object->_sprite->getPosition();
            Vec2 nextObjPos = currentObjPos + x->_velocity;
            x->_object->_sprite->setPosition(nextObjPos);
            x->update(dt);
            
            if(checkCollisionOther(x))
            {
                CCLOG("Collider!");
                x->_object->_sprite->setPosition(currentObjPos);
            }
            else
            {
                
            }
        }
    }
}

bool RigidWorld::checkCollisionOther(shared_ptr<RigidBody> body)
{
    for(auto& x : _listRigidBodies)
    {
        if(x->getTag() != body->getTag())
        {
            shared_ptr<RigidBodyPolygon> body1 = dynamic_pointer_cast<RigidBodyPolygon>(body);
            shared_ptr<RigidBodyPolygon> body2 = dynamic_pointer_cast<RigidBodyPolygon>(x);
            
            if(body1 && body2)
            {
                if(body1->_rect.intersectsRect(body2->_rect))
                    return true;
            }
        }
    }
    
    return false;
}

shared_ptr<RigidBodyPolygon> RigidWorld::createRigidBodyPolygon(const Rect& rect)
{
    auto body =  make_shared<RigidBodyPolygon>();
    body->init();
    body->_rect = rect;
    body->_type = RigidBody::type::STATIC;
    _listRigidBodies.push_back((shared_ptr<RigidBody>)body);
    
    return body;
}

shared_ptr<RigidBody> RigidWorld::createRigidBodyPolygon(shared_ptr<Character> character)
{
    shared_ptr<RigidBody> body = RigidBodyPolygon::createRigidBodyPolygon(character);
    character->_rigidBody = body;
    _listRigidBodies.push_back(body);
    return body;
}
