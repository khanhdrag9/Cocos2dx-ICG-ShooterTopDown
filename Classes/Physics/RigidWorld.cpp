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
#include "RigidBodyCircle.h"
#include "../Characters/Character.h"
#include "../Defines/constants.h"
#include "../Objects/BulletBasic.h"


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
        if(x->_object != nullptr)
        {
            Vec2 currentObjPos = x->_object->_sprite->getPosition();
            Vec2 nextObjPos = currentObjPos + x->_velocity;
            x->_object->_sprite->setPosition(nextObjPos);
            x->update(dt);
            
            if(checkCollisionOther(x))
            {
                if(x->_object)
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
                {
                    onCollision(body1, body2);
                    return true;
                }
            }
            else
            {
                if(body1 && !body2)
                {
                    shared_ptr<RigidBodyCircle> bodyCircle = dynamic_pointer_cast<RigidBodyCircle>(x);
                    Vec2 circlePos = bodyCircle->_object->_sprite->getPosition();
                    if(body1->_rect.intersectsCircle(circlePos, bodyCircle->_radius))
                    {
                        onCollision(body1, bodyCircle);
                        return true;
                    }
                }
                else if(!body1 && body2)
                {
                    shared_ptr<RigidBodyCircle> bodyCircle = dynamic_pointer_cast<RigidBodyCircle>(body);
                    Vec2 circlePos = bodyCircle->_object->_sprite->getPosition();
                    if(body2->_rect.intersectsCircle(circlePos, bodyCircle->_radius))
                    {
                        onCollision(body2, bodyCircle);
                        return true;
                    }
                }
            }
        }
    }
    
    return false;
}

void RigidWorld::pushLineByRect(const Rect & rect)
{
	//push 4 line of a rect
	pushLine(Line(Vec2(rect.getMinX(), rect.getMaxY()), Vec2(rect.getMaxX(), rect.getMaxY())));
	pushLine(Line(Vec2(rect.getMaxX(), rect.getMaxY()), Vec2(rect.getMaxX(), rect.getMinY())));
	pushLine(Line(Vec2(rect.getMaxX(), rect.getMinY()), Vec2(rect.getMinX(), rect.getMinY())));
	pushLine(Line(Vec2(rect.getMinX(), rect.getMinY()), Vec2(rect.getMinX(), rect.getMaxY())));
}

shared_ptr<RigidBodyPolygon> RigidWorld::createRigidBodyPolygon(const Rect& rect)
{
    auto body =  make_shared<RigidBodyPolygon>();
    body->init();
    body->_rect = rect;
    body->_type = RigidBody::type::STATIC;
    _listRigidBodies.push_back((shared_ptr<RigidBody>)body);

	pushLineByRect(body->_rect);
    
    return body;
}

shared_ptr<RigidBody> RigidWorld::createRigidBodyPolygon(shared_ptr<Character> character)
{
    shared_ptr<RigidBody> body = RigidBodyPolygon::createRigidBodyPolygon(character);
    character->_rigidBody = body;
    _listRigidBodies.push_back(body);

	auto bodyCast = dynamic_pointer_cast<RigidBodyPolygon>(character->_rigidBody);
	if(bodyCast)
		pushLineByRect(bodyCast->_rect);

    return body;
}

shared_ptr<RigidBody> RigidWorld::createRigidBodyCircle(shared_ptr<Character> character)
{
    shared_ptr<RigidBody> body = RigidBodyCircle::createRigidBodyCircle(character);
    character->_rigidBody = body;
    _listRigidBodies.push_back(body);
    return body;
}

void RigidWorld::onCollision(shared_ptr<RigidBody> body1, shared_ptr<RigidBody> body2)
{
    shared_ptr<Character> obj1 = body1->_object;
    shared_ptr<Character> obj2 = body2->_object;
    
    //check bullet
    if(obj1)
    {
        if(obj1->getName() == constants::object_bullet_basic)
        {
            shared_ptr<BulletBasic> bullet = dynamic_pointer_cast<BulletBasic>(obj1);
            if(bullet)bullet->destroy();
        }
    }
    
    if(obj2)
    {
        if(obj2->getName() == constants::object_bullet_basic)
        {
            shared_ptr<BulletBasic> bullet = dynamic_pointer_cast<BulletBasic>(obj2);
            if(bullet)bullet->destroy();
        }
    }
}

const vector<shared_ptr<RigidBody>>& RigidWorld::getListBodies() const
{
    return _listRigidBodies;
}

const vector<Line>& RigidWorld::getListLines() const
{
	return _listLines;
}

void RigidWorld::pushLine(const Line & line)
{
	auto isExits = std::find(_listLines.begin(), _listLines.end(), line);
	if (isExits == _listLines.end())
		_listLines.push_back(line);
}

void RigidWorld::generateLineAgain()
{
	
}
