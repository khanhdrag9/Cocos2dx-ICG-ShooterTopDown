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
_destroy(false),
_maxHP(100),
_currentHP(_maxHP)
{
}


Character::~Character()
{
    CCLOG("Delete Character");
    //_commandHandle = nullptr;
    if(_sprite)
    {
        //_sprite->removeFromParent();
        _sprite->removeFromParentAndCleanup(true);
    }
}


void Character::init()
{
    _destroy = false;
    _type = Character::type::NONE;
    _name = constants::character_none;

}


void Character::update(float dt)
{
    if(_destroy || _currentHP <= 0)return;
        
	for (auto cmd : _commandQueue)
		cmd->update(dt);
}


void Character::releaseCommands()
{
	_commandQueue.clear();
}


void Character::pushCommand(shared_ptr<Command>& command, bool replace)
{
    if(_destroy || _currentHP <= 0)return;
    
   // command->registAnObject(shared_from_this(), _commandQueue);
	//release cmd has finished
	for (auto begin = _commandQueue.begin(); begin != _commandQueue.end();)
	{
		shared_ptr<Command> cmd = *begin;
		if (cmd->isFinished())
		{
			begin = _commandQueue.erase(begin);
		}
		else
			++begin;
	}

	list<shared_ptr<Command>> queueTemp;
	bool isUsed = false;

	while (_commandQueue.size() > 0)
	{
        if(_commandQueue.front())
        {
            if (_commandQueue.front()->getName() == command->getName())
            {
                isUsed = true;
            }

            if (!isUsed)
                queueTemp.push_back(_commandQueue.front());
            else if (!replace)
                queueTemp.push_back(_commandQueue.front());
            else
                isUsed = false;
        }

		_commandQueue.pop_front();
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

int Character::getMaxHP() const
{
    return _maxHP;
}

int Character::getCurrentHP()
{
    return _currentHP;
}

void Character::decreHP(int decre)
{
    _currentHP -= decre;
    if(_currentHP < 0)
    {
        _currentHP = 0;
    }
}

void Character::heal(int heal)
{
    _currentHP += heal;
    if(_currentHP > _maxHP)_currentHP = _maxHP;
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
