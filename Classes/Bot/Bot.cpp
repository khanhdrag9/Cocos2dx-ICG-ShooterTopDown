#include "Bot.h"
#include "Defines/constants.h"
#include "Commands/Command.h"
#include "../Physics/RigidBody.h"
#include "../Resource/ResourceManager.h"
#include "BotMoveMap.h"
#include "../Commands/CommandMoveTo.h"
#include "BotManager.h"

Bot::Bot() : Character(),
_linkPos(nullptr),
_speedMove(0)
{
}

void Bot::init()
{
	Character::init();

	_type = Character::type::ENEMY;
	_name = constants::character_enemy;
    
    _sprite = Sprite::create(ResourceManager::getInstance()->at(res::define::ENEMY_1_RED));

	

	_speedMove = 150.f;
}

void Bot::update(float dt)
{
	Character::update(dt);

	if (_commandQueue.empty())
	{
		_linkPos = BotManager::getInstance()->getNextLinkPosition(dynamic_pointer_cast<Bot>(shared_from_this()), true);

		if (_linkPos)
		{
			shared_ptr<Command> cmd = CommandMoveTo::createCommandMoveTo(_speedMove, _linkPos->get());
			pushCommand(cmd);
		}
	}
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

void Bot::setLinkPosition(shared_ptr<LinkPosition> newlink)
{
	_linkPos = newlink;
}

shared_ptr<LinkPosition> Bot::getLinkPosition() const
{
	return _linkPos;
}
