#include "Bot.h"
#include "Defines/constants.h"
#include "Commands/Command.h"
#include "../Physics/RigidBody.h"
#include "../Resource/ResourceManager.h"
#include "BotMoveMap.h"
#include "../Commands/CommandMoveTo.h"
#include "BotManager.h"
#include "Game.h"

Bot::Bot() : Character(),
_linkPos(nullptr),
_speedMove(0),
_currentStatus(Bot::Status::STOP),
_ableWalk(false),
_countShoot(pair<float, float>(0.f, 0.25f))
{
}

void Bot::init()
{
	Character::init();

	_type = Character::type::ENEMY;
	_name = constants::character_enemy;
    
    _sprite = Sprite::create(ResourceManager::getInstance()->at(res::define::ENEMY_1_YELLOW));

	

	_speedMove = 200.f;
}

void Bot::update(float dt)
{
    if(_currentStatus == Status::STOP)return;
    
	Character::update(dt);
    
    if(_currentStatus == Status::WALK)
    {
        if (_commandQueue.empty())
        {
            auto nextlink = BotManager::getInstance()->getNextLinkPosition(dynamic_pointer_cast<Bot>(shared_from_this()), true);
            
            if(nextlink->getName() != _linkPos->getName())
            {
                _linkPos = nextlink;
                if (_linkPos)
                {
                    shared_ptr<Command> cmd = CommandMoveTo::createCommandMoveTo(_speedMove, _linkPos->get());
                    pushCommand(cmd);
                }
            }
		
        }
    }
	else if (_currentStatus == Status::SHOOT && _countShoot.first >= _countShoot.second)
	{
		Game::getInstance()->handleShootCharacter(shared_from_this(), 1000);
		_countShoot.first = 0.f;
	}
	_countShoot.first += dt;
    
    //update rotation
    if (_linkPos)
    {
        Vec2 offset = _linkPos->get() - _sprite->getPosition();
        auto angle = atan2(offset.y, offset.x);
        _sprite->setRotation(CC_RADIANS_TO_DEGREES(-angle) + 90);
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

void Bot::setStatus(Status status)
{
    _currentStatus = status;
}

Bot::Status Bot::getStatus() const
{
    return _currentStatus;
}

bool Bot::isCanTriggerWalk()
{
	return _ableWalk;
}

bool Bot::isCanTriggerShoot()
{

	return false;
}

void Bot::setWalk(bool enable)
{
	_ableWalk = enable;
}
