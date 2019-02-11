#include "Bot.h"
#include "Command.h"

Bot::Bot()
{
}

Bot::~Bot()
{
}

void Bot::init()
{
	_type = BOT;
	sprite = Sprite::create(BOT_SQUARE_PATH);

	auto body = PhysicsBody::createBox(sprite->getContentSize());
	body->setRotationEnable(true);
	body->setContactTestBitmask(PHYSICS_ENEMY);
	body->setCategoryBitmask(PHYSICS_ENEMY);
	body->setCollisionBitmask(PHYSICS_ENEMY);
	sprite->setPhysicsBody(body);
}

void Bot::upgrade()
{
}
