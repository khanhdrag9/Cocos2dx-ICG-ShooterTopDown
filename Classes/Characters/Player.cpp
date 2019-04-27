//
//  Player.cpp
//  Shooter
//
//  Created by Khanh on 3/9/19.
//

#include "Player.h"
#include "../Commands/Command.h"
#include "../Defines/constants.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Resources.h"
#include "../Defines/Templates.h"

Player::Player() : Character()
{
}

Player::~Player()
{
    
}

void Player::init(CharacterCreation* creation)
{
    Character::init(creation);
    
    _type = Character::type::PLAYER;
    _name = constants::character_player;
    
//    res::define asset = res::define::PLAYER_1_RED;
//    _sprite = Sprite::create(ResourceManager::getInstance()->at(asset).c_str());
    
    _speedMove = creation->_speed;
    _bulletMag = make_unique<Mag>(creation->_timeReload, creation->_maxBullet, 1.5f);
}
void Player::update(float dt)
{
    Character::update(dt);
	if (_destroy)return;

    _bulletMag->update(dt);
}

const unique_ptr<Mag>& Player::getMag() const
{
    return _bulletMag;
}

bool Player::pushCommand(shared_ptr<Command>& command, bool replace)
{
	bool r = Character::pushCommand(command, replace);
	return r;
}

