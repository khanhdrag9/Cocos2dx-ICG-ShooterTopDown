#include "Bot.h"
#include "Defines/constants.h"
#include "Commands/Command.h"
#include "../Physics/RigidBody.h"

Bot::Bot() : Character()
{
}

void Bot::init()
{
	Character::init();

	_type = Character::type::ENEMY;
	_name = constants::character_enemy;
	_sprite = Sprite::create("sprite.png");

	

	_speedMove = 100.f;
}

void Bot::update(float dt)
{
	Character::update(dt);
}

void Bot::pushCommand(shared_ptr<Command>& command)
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

	if (!isUsed)
		command->registAnObject(shared_from_this(), _commandQueue);
}
