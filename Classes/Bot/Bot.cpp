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
_ableWalk(false),
_ableShoot(false)
{
}

void Bot::init()
{
	Character::init();

	_type = Character::type::ENEMY;
	_name = constants::character_enemy;
    
    _sprite = Sprite::create(ResourceManager::getInstance()->at(res::define::ENEMY_1_YELLOW));
    
	_speedMove = 200.f;
    
    _bulletMag = make_unique<Mag>(0.2, 30, 1.5f);
}

void Bot::update(float dt)
{
    if(containStatus(Status::STOP))return;
    
	Character::update(dt);
    _bulletMag->update(dt);
    
    bool isMoved = false;
    if(containStatus(Status::WALK))
    {
        if (_commandQueue.empty())
        {
            auto nextlink = BotManager::getInstance()->getNextLinkPosition(dynamic_pointer_cast<Bot>(shared_from_this()), true);
            if(&(*nextlink) != &(*_linkPos))isMoved = true;
            
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

	if (containStatus(Status::SHOOT) && _bulletMag->canShoot())
	{
		Game::getInstance()->handleShootCharacter(shared_from_this(), 1000);
	}
    
    //update rotation
    if (_linkPos && isMoved)
    {
        Vec2 offset = _linkPos->get() - _sprite->getPosition();
        auto angle = atan2(offset.y, offset.x);
        _sprite->setRotation(CC_RADIANS_TO_DEGREES(-angle) + 90);
    }

	_currentStatus.clear();
}

void Bot::pushCommand(shared_ptr<Command>& command, bool replace)
{
	Character::pushCommand(command, replace);

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
	_currentStatus.push_back(status);
}

list<Bot::Status>& Bot::getStatus()
{
    return _currentStatus;
}

bool Bot::containStatus(Status status)
{
	auto check = std::find(_currentStatus.begin(), _currentStatus.end(), status);
	return check != _currentStatus.end();
}

bool Bot::isCanTriggerWalk()
{
	return _ableWalk;
}

bool Bot::isCanTriggerShoot()
{
	return _ableShoot;
}

void Bot::setWalk(bool enable)
{
	_ableWalk = enable;
}

void Bot::setShoot(bool enable)
{
	_ableShoot = enable;
}

const unique_ptr<Mag>& Bot::getMag() const
{
    return _bulletMag;
}
