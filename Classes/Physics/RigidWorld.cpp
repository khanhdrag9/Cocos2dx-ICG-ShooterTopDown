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
#include "../Bot/InformationCenter.h"
#include "../Bot/Bot.h"


RigidWorld::RigidWorld()
{
}

RigidWorld::~RigidWorld()
{
    _listLines.clear();
    _listRigidBodies.clear();
    
}

void RigidWorld::update(float dt)
{
	//clean body
	for (auto begin = _listRigidBodies.begin(); begin != _listRigidBodies.end();) 
	{
		auto& body = *begin;
		if (body->getTag() != RigidBody::tag::WALL)
		{
			if (body->getObject() == nullptr || body->getObject()->isDestroyed())
				begin = _listRigidBodies.erase(begin);
			else
				++begin;
		}
		else
			++begin;
	}

    //for(auto& x : _listRigidBodies)
    //{
    //    x->update(dt);
    //}
    
    for(auto& x : _listRigidBodies)
    {
        if(x->_object != nullptr)
        {
            Vec2 currentObjPos = x->_object->_sprite->getPosition();
            Vec2 nextObjPos = currentObjPos + x->_velocity * dt;
            x->_object->_sprite->setPosition(nextObjPos);
            x->update(dt);
            
            if(checkCollisionOther(x))
            {
				if (x->_object)
				{
					x->_object->_sprite->setPosition(currentObjPos + (-1 * x->_velocity * dt));
					//x->_object->_sprite->setPosition(nextObjPos - x->_velocity * dt);
					//x->_velocity = Vec2::ZERO;
				}
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
                    return onCollision(body1, body2);
                    //return true;
                }
            }
            else
            {
                if(body1 && !body2)
                {
                    shared_ptr<RigidBodyCircle> bodyCircle = dynamic_pointer_cast<RigidBodyCircle>(x);
                    Vec2 circlePos = bodyCircle->_object->_sprite->getPosition();
                    if(body1->_rect.intersectsCircle(circlePos, bodyCircle->_radius * 1.1f))
                    {
                        return onCollision(body1, bodyCircle);
                        //return true;
                    }
                }
                else if(!body1 && body2)
                {
                    shared_ptr<RigidBodyCircle> bodyCircle = dynamic_pointer_cast<RigidBodyCircle>(body);
                    Vec2 circlePos = bodyCircle->_object->_sprite->getPosition();
                    if(body2->_rect.intersectsCircle(circlePos, bodyCircle->_radius))
                    {
                        return onCollision(body2, bodyCircle);
                        //return true;
                    }
                }
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

//    pushLineByRect(body->_rect);
    
    return body;
}

shared_ptr<RigidBody> RigidWorld::createRigidBodyPolygon(shared_ptr<Character> character)
{
    shared_ptr<RigidBody> body = RigidBodyPolygon::createRigidBodyPolygon(character);
    character->_rigidBody = body;
    _listRigidBodies.push_back(body);

//    auto bodyCast = dynamic_pointer_cast<RigidBodyPolygon>(character->_rigidBody);
//    if(bodyCast)
//        pushLineByRect(bodyCast->_rect);

    return body;
}

shared_ptr<RigidBody> RigidWorld::createRigidBodyCircle(shared_ptr<Character> character)
{
    shared_ptr<RigidBody> body = RigidBodyCircle::createRigidBodyCircle(character);
    character->_rigidBody = body;
    _listRigidBodies.push_back(body);
    return body;
}

bool RigidWorld::onCollision(shared_ptr<RigidBody> body1, shared_ptr<RigidBody> body2)
{
    shared_ptr<Character> obj1 = body1->_object;
    shared_ptr<Character> obj2 = body2->_object;
    
    if(obj1 && obj2)
    {
        if(obj1->getName() == constants::object_bullet_basic && obj2->getName() == obj1->getName())
        {
            shared_ptr<BulletBasic> bullet1 = dynamic_pointer_cast<BulletBasic>(obj1);
            shared_ptr<BulletBasic> bullet2 = dynamic_pointer_cast<BulletBasic>(obj2);
            int damge1 = bullet1->getDamge();
            int damge2 = bullet2->getDamge();
            bullet1->setDamege(damge1 - damge2);
            bullet2->setDamege(damge2 - damge1);
            
            if(bullet1->getDamge() <= 0)
                bullet1->destroy();
            if(bullet2->getDamge() <= 0)
                bullet2->destroy();
            
            return false;
        }
    }
    
    //check bullet
    if(obj1)
    {
        if(obj1->getName() == constants::object_bullet_basic)
        {
            shared_ptr<BulletBasic> bullet = dynamic_pointer_cast<BulletBasic>(obj1);
			if (bullet)
			{
				bullet->destroy();
                if(auto character = dynamic_pointer_cast<Character>(obj2))
                {
                    character->decreHP(bullet->getDamge());
                    if(character->getCurrentHP() <= 0)character->releaseCommands();
                }
				return false;
			}
        }
    }
    
    if(obj2)
    {
        if(obj2->getName() == constants::object_bullet_basic)
        {
            shared_ptr<BulletBasic> bullet = dynamic_pointer_cast<BulletBasic>(obj2);
			if (bullet)
			{
				bullet->destroy();
                if(auto character = dynamic_pointer_cast<Character>(obj2))
                {
                    character->decreHP(bullet->getDamge());
                    if(character->getCurrentHP() <= 0)character->releaseCommands();
                }
				return false;
			}
        }
    }

	if (body1->getTag() == RigidBody::tag::WALL && obj2)
	{
		auto bot = InformationCenter::getInstance()->findBotWayByBot(obj2);
		if (bot && bot->bot && bot->status == InformationCenter::statusBot::WALK && bot->countDetectOK())
		{
			bot->status = InformationCenter::statusBot::COLLISION;
		}
		obj2->releaseMoveCommands();
	}
	else if (body2->getTag() == RigidBody::tag::WALL && obj1)
	{
		auto bot = InformationCenter::getInstance()->findBotWayByBot(obj1);
		if (bot && bot->bot && bot->status == InformationCenter::statusBot::WALK && bot->countDetectOK())
		{
			bot->status = InformationCenter::statusBot::COLLISION;
		}
		obj2->releaseMoveCommands();
	}

	return true;
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
