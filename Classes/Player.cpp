#include "Player.h"

//base class
Character::Character()
{
	actions.AddCommand(Command::command::MOVE_UP, false);
	actions.AddCommand(Command::command::MOVE_DOWN, false);
	actions.AddCommand(Command::command::MOVE_RIGHT, false);
	actions.AddCommand(Command::command::MOVE_LEFT, false);
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
	_typeplayer = Character::typeplayer::SQUARE;

	auto body = PhysicsBody::createBox(sprite->getBoundingBox().size);
	body->setRotationEnable(true);
	body->setContactTestBitmask(PHYSICS_PLAYER);
	body->setCategoryBitmask(PHYSICS_PLAYER);
	body->setCollisionBitmask(PHYSICS_PLAYER);
	sprite->setPhysicsBody(body);

	actions.AddCommand(Command::command::SHOOT, false);
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
	//_player->init();
	_type = _player->getType();
	_arrowWorldSpace = _player->getArrowWorldSpace();
	_typeplayer = Character::typeplayer::SQUARE;

	_player->sprite = Sprite::create(PLAYER_SQUARE_PATH);
	sprite = _player->sprite;

	Vec2 position = sprite->getPosition();
	Size size = sprite->getBoundingBox().size;
	auto body = PhysicsBody::createBox(size);
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
