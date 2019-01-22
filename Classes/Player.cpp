#include "Player.h"

//base class
Character::Character()
{
	actions[Command::command::MOVE_UP] = false;
	actions[Command::command::MOVE_DOWN] = false;
	actions[Command::command::MOVE_RIGHT] = false;
	actions[Command::command::MOVE_LEFT] = false;
}

Character::~Character()
{
	if (sprite)
	{
		sprite->removeFromParentAndCleanup(true);
		CC_SAFE_DELETE(sprite);
	}
	actions.clear();
}

void Character::addParrent(Node* parrent)
{
	if (sprite)
	{
		sprite->removeFromParentAndCleanup(true);
		parrent->addChild(sprite);
	}
	else
	{
		CCLOG("sprite of character is null!");
	}
}

//derrived
Player::Player()
{
	init();
}

Player::~Player()
{
}

void Player::init()
{
	sprite = Sprite::create(PLAYER_PATH);
	sprite->retain();
}

void Player::update()
{
	
}