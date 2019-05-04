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
_speedMove(0)
{
}

Bot::~Bot()
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
    if(_destroy)return;

	Character::update(dt);
    _bulletMag->update(dt);
}

bool Bot::pushCommand(shared_ptr<Command>& command, bool replace)
{
	bool r = Character::pushCommand(command, replace);
	return r;
}

const unique_ptr<Mag>& Bot::getMag() const
{
    return _bulletMag;
}
