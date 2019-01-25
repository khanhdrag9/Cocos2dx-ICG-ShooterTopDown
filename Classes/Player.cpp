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
	/*if (sprite != nullptr)
	{
		sprite->removeFromParentAndCleanup(true);
		CC_SAFE_DELETE(sprite);
	}*/
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
	_type = PLAYER;
	sprite = Sprite::create(PLAYER_SQUARE_PATH);

	auto body = PhysicsBody::createBox(sprite->getContentSize());
	body->setRotationEnable(true);
	body->setContactTestBitmask(PHYSICS_PLAYER);
	body->setCategoryBitmask(PHYSICS_PLAYER);
	body->setCollisionBitmask(PHYSICS_PLAYER);
	sprite->setPhysicsBody(body);
}

void Player::upgrade()
{
}


//upgrades
PlayerSquare::PlayerSquare(const shared_ptr<Character>& player):
	_player(player)
{
	init();
}
PlayerSquare::~PlayerSquare()
{
}

void PlayerSquare::init()
{
	_player->init();
	_type = _player->getType();
	_arrowWorldSpace = _player->getArrowWorldSpace();
	_typeplayer = Player::typeplayer::SQUARE;

	_player->sprite = Sprite::create(PLAYER_SQUARE_PATH);
	sprite = _player->sprite;

	auto body = PhysicsBody::createBox(sprite->getContentSize());
	body->setRotationEnable(true);
	body->setContactTestBitmask(PHYSICS_PLAYER);
	body->setCategoryBitmask(PHYSICS_PLAYER);
	body->setCollisionBitmask(PHYSICS_PLAYER);
	sprite->setPhysicsBody(body);
}

void PlayerSquare::upgrade()
{
	_player->upgrade();

}
