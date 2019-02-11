#include "Bot.h"
#include "Command.h"

Bot::Bot()
{
	init();
}

Bot::~Bot()
{
	CC_SAFE_DELETE(sprite);
}

void Bot::init()
{
	_type = ENEMY;
	sprite = Sprite::create(BOT_SQUARE_PATH);
	_typeplayer = Character::typeplayer::SQUARE;

	auto body = PhysicsBody::createBox(sprite->getContentSize(), PhysicsMaterial(0.1f, 0.5f, 1.f));
	body->setRotationEnable(true);
	body->setContactTestBitmask(PHYSICS_ENEMY);
	body->setCategoryBitmask(PHYSICS_ENEMY);
	body->setCollisionBitmask(PHYSICS_ENEMY);
	sprite->setPhysicsBody(body);

	actions.AddCommand(Command::command::SHOOT, false);
}

void Bot::upgrade()
{
}
