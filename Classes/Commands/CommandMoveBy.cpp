//
//  CommandMoveBy.cpp
//  Shooter
//
//  Created by Khanh on 3/10/19.
//

#include "CommandMoveBy.h"
#include "../Defines/constants.h"
#include "../Characters/Character.h"
#include "../Physics/RigidBody.h"

CommandMoveBy::CommandMoveBy()
{
}

shared_ptr<CommandMoveBy> CommandMoveBy::createCommandMoveBy(const Vec2& vec, const float& duration)
{
    shared_ptr<CommandMoveBy> cmd = make_shared<CommandMoveBy>();
    cmd->_duration = duration;
    cmd->_counttime = 0.f;
    cmd->_velocity = vec;
    
    if(vec.x > 0)
        cmd->_name = constants::command_move_by_right;
    else if(vec.x < 0)
        cmd->_name = constants::command_move_by_left;
    else if(vec.y < 0)
        cmd->_name = constants::command_move_by_down;
    else if(vec.y > 0)
        cmd->_name = constants::command_move_by_up;
    else
        cmd->_name = constants::command_move_by_none;
    
    return cmd;
}

void CommandMoveBy::update(float dt)
{
    if(!_isFinished && _object)
    {
        if(_counttime >= _duration)
        {
            _isFinished = true;
            _counttime = 0.f;
            _duration = 0.f;
            _velocity = Vec2(0,0);
			_object->_rigidBody->_velocity = _velocity;
        }
        else
        {
            //Vec2 newvec = _velocity;
            _object->_rigidBody->_velocity = _velocity * dt;
            _counttime+=dt;
        }
    }
}
