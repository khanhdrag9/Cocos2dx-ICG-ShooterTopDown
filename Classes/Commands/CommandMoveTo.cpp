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
    
    return cmd;
}

void CommandMoveTo::update(float dt)
{
    if(!_isFinished && _object)
    {
        Vec2 currentPos = _object->_sprite->getPosition();
        if(currentPos == _target)
        {
            _isFinished = true;
        }
        else
        {
            Vec2 offset = _target - currentPos;
            offset.normalize();
            offset *= _speed * dt;
            
            _object->_rigidBody->_velocity = offset;
        }
    }
}
