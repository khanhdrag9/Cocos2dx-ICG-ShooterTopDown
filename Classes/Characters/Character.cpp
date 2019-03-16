//
//  Character.cpp
//  Shooter
//
//  Created by Khanh on 3/9/19.
//

#include "Character.h"
#include "../Commands/Command.h"
#include "../Defines/constants.h"
#include "../Physics/RigidBody.h"

Character::Character():
_sprite(nullptr),
_rigidBody(nullptr)
{
}


Character::~Character()
{
    //_commandHandle = nullptr;
    _sprite = nullptr;
    releaseCommands();
    _rigidBody = nullptr;
}


void Character::init()
{
    _type = Character::type::NONE;
    _name = constants::character_none;

}


void Character::update(float dt)
{
    queue<shared_ptr<Command>> tempCommand;
    while (_commandQueue.size() > 0)
    {
        shared_ptr<Command> cmd = _commandQueue.front();
        cmd->update(dt);
        
        if(!cmd->isFinished())
            tempCommand.push(cmd);
        
        _commandQueue.pop();
    }
    
    _commandQueue.swap(tempCommand);
}


void Character::releaseCommands()
{
    while(_commandQueue.size() > 0)
        _commandQueue.pop();
}


void Character::pushCommand(shared_ptr<Command>& command)
{
    command->registAnObject(shared_from_this(), _commandQueue);
}


const string& Character::getName() const
{
    return _name;
}


const Character::type& Character::getType() const
{
    return _type;
}



