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
    
    _speedMove = 10.f;
}
void Player::update(float dt)
{
    Character::update(dt);
}

void Player::pushCommand(shared_ptr<Command>& command)
{
    for(int i = 0; i < _commandQueue.size(); i++)
    {
        auto x = *(&_commandQueue.front() + i);
        if(x->getName() == command->getName())
            return;
    }
    
    command->registAnObject(shared_from_this(), _commandQueue);
}

