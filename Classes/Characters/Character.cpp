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
_rigidBody(nullptr),
_destroy(false)
{
}


Character::~Character()
{
    CCLOG("Delete Character");
    //_commandHandle = nullptr;
    _sprite->runAction(RemoveSelf::create());
}


void Character::init()
{
    _destroy = false;
    _type = Character::type::NONE;
    _name = constants::character_none;

}


void Character::update(float dt)
{
    if(_destroy)return;
        
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


void Character::pushCommand(shared_ptr<Command>& command, bool replace)
{
    if(_destroy)return;
    
   // command->registAnObject(shared_from_this(), _commandQueue);
	queue<shared_ptr<Command>> queueTemp;
	bool isUsed = false;

	while (_commandQueue.size() > 0)
	{
		if (_commandQueue.front()->getName() == command->getName())
		{
			isUsed = true;
		}

		if (!isUsed)
			queueTemp.push(_commandQueue.front());
		else if (!replace)
			queueTemp.push(_commandQueue.front());
		else
			isUsed = false;

		_commandQueue.pop();
	}

	_commandQueue.swap(queueTemp);

	if (!isUsed)
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

void Character::destroy()
{
    releaseCommands();
    if(_rigidBody)
        _rigidBody->_object = nullptr;
    _rigidBody = nullptr;
    _destroy = true;
}

bool Character::isDestroyed()
{
    return _destroy;
}
