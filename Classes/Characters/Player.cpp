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

Player::Player() : Character()
{
}

void Player::init()
{
    Character::init();
    
    _type = Character::type::PLAYER;
    _name = constants::character_player;
    
    res::define asset = res::define::PLAYER_1_RED;
    _sprite = Sprite::create(ResourceManager::getInstance()->at(asset).c_str());
    
    _speedMove = 300.f;
}
void Player::update(float dt)
{
    Character::update(dt);
}

void Player::pushCommand(shared_ptr<Command>& command, bool replace)
{
	Character::pushCommand(command, replace);
}

