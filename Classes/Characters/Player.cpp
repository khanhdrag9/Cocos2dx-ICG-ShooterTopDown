//
//  Player.cpp
//  Shooter
//
//  Created by Khanh on 3/9/19.
//

#include "Player.h"
#include "../Commands/Command.h"
#include "../Defines/constants.h"

Player::Player() : Character()
{
}

void Player::init()
{
    Character::init();
    
    _type = Character::type::PLAYER;
    _name = constants::character_player;
    _sprite = Sprite::create("sprite.png");

	_rigidBody = PhysicsBody::createBox(_sprite->getContentSize());
	_sprite->setPhysicsBody(_rigidBody);
    
    _speedMove = 100.f;
}
void Player::update(float dt)
{
    Character::update(dt);
}

void Player::pushCommand(shared_ptr<Command>& command)
{
	queue<shared_ptr<Command>> queueTemp;
	bool isUsed = false;

	while (_commandQueue.size() > 0)
	{
		if (_commandQueue.front()->getName() == command->getName())
		{
			isUsed = true;
		}
		queueTemp.push(_commandQueue.front());
		_commandQueue.pop();
	}

	_commandQueue.swap(queueTemp);
    
	if(!isUsed)
		command->registAnObject(shared_from_this(), _commandQueue);
}

