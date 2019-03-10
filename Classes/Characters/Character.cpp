//
//  Character.cpp
//  Shooter
//
//  Created by Khanh on 3/9/19.
//

#include "Character.h"
#include "../Commands/Command.h"

Character::Character():
//_commandHandle(nullptr),
_sprite(nullptr),
_rigidBody(nullptr)
{
}


Character::~Character()
{
    //_commandHandle = nullptr;
    _sprite = nullptr;
    releaseCommands();
}


void Character::init()
{
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

