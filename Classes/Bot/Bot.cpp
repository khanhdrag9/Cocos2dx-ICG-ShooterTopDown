#include "Bot.h"
#include "Defines/constants.h"
#include "Commands/Command.h"
#include "../Physics/RigidBody.h"
#include "../Resource/ResourceManager.h"
#include "BotMoveMap.h"
#include "../Commands/CommandMoveTo.h"
#include "BotManager.h"
#include "Game.h"
#include "InformationCenter.h"

Bot::Bot() : Character(),
_speedMove(0),
_ableWalk(false),
_ableShoot(false)
{
}

void Bot::init(CharacterCreation* creation)
{
	Character::init(creation);

	_type = Character::type::ENEMY;
	_name = constants::character_enemy;
    
//    _sprite = Sprite::create(ResourceManager::getInstance()->at(res::define::ENEMY_1_YELLOW));
    
	_speedMove = creation->_speed;
    
    _bulletMag = make_unique<Mag>(creation->_timeReload, creation->_maxBullet, 1.5f);
}

void Bot::update(float dt)
{
    if(containStatus(Status::STOP) || _destroy)return;

	Character::update(dt);
    _bulletMag->update(dt);
    
    if(containStatus(Status::WALK))
    {
        
//        if (_commandQueue.empty())
//        {
//            auto nextlink = BotManager::getInstance()->getNextLinkPosition(dynamic_pointer_cast<Bot>(shared_from_this()), true);
//            if(&(*nextlink) != &(*_linkPos))isMoved = true;
//
//            if(nextlink->getName() != _linkPos->getName())
//            {
//                _linkPos = nextlink;
//                if (_linkPos)
//                {
//                    shared_ptr<Command> cmd = CommandMoveTo::createCommandMoveTo(_speedMove, _linkPos->get());
//                    pushCommand(cmd);
//                }
//            }
//        }
    }

	if (containStatus(Status::SHOOT) && _bulletMag->canShoot())
	{
		Game::getInstance()->handleShootCharacter(shared_from_this(), 1000);
	}
    
    //update rotation

	_currentStatus.clear();
}

bool Bot::pushCommand(shared_ptr<Command>& command, bool replace)
{
	bool r = Character::pushCommand(command, replace);
	return r;
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
