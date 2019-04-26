//
//  CommandMoveTo.cpp
//  Shooter
//
//  Created by Khanh on 3/12/19.
//

#include "CommandMoveTo.h"
#include "../Defines/constants.h"
#include "../Characters/Character.h"
#include "../Physics/RigidBody.h"

CommandMoveTo::CommandMoveTo():
_speed(0.f),
_velocity(Vec2(0,0)),
_target(Vec2(0,0))
{
    
}

shared_ptr<CommandMoveTo> CommandMoveTo::createCommandMoveTo(const float& speed, const Vec2& toPoint)
{
    shared_ptr<CommandMoveTo> cmd = make_shared<CommandMoveTo>();
    cmd->_speed = speed;
    cmd->_target = toPoint;
    cmd->_name = constants::command_move_to;
	cmd->_isFinished = false;
    
    return cmd;
}

void CommandMoveTo::update(float dt)
{
    auto object = _object.lock();
    if(!_isFinished && object)
    {
        
        Vec2 currentPos = object->_sprite->getPosition();
        if(currentPos == _target)
        {
            _isFinished = true;
        }
        else
        {
			Vec2 offset = _target - currentPos;
			offset.normalize();
			offset = offset * _speed;
			if((_target - currentPos).length() <= (offset * dt).length())
			{
				object->_sprite->setPosition(_target);
				_isFinished = true;
				offset = Vec2(0,0);
			}

            object->_rigidBody->_velocity = offset;
        }
    }
}

Vec2 CommandMoveTo::getTarget() const
{
	return _target;
}
