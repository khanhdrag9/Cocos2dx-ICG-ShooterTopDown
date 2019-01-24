#include "Command.h"
#include "Player.h"

Command::Command():
	_countshoottime(0.f),
	_shootinterval(BULLET1_INTERVAL)
{

}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
void Command::remote(shared_ptr<Character> character, const EventKeyboard::KeyCode& code, bool value)
{
	switch (code)
	{
	case EventKeyboard::KeyCode::KEY_W:
		character->actions[command::MOVE_UP] = value;
		break;
	case EventKeyboard::KeyCode::KEY_S:
		character->actions[command::MOVE_DOWN] = value;
		break;
	case EventKeyboard::KeyCode::KEY_A:
		character->actions[command::MOVE_LEFT] = value;
		break;
	case EventKeyboard::KeyCode::KEY_D:
		character->actions[command::MOVE_RIGHT] = value;
		break;
	default:
		break;
	}
}
void Command::remote(shared_ptr<Character> character, const Vec2& touchpos, const EventMouse::MouseEventType& code, bool value)
{
	switch (code)
	{
	case EventMouse::MouseEventType::MOUSE_DOWN:
		character->actions[command::SHOOT] = value;
		_touchPos = touchpos;
		break;
	case EventMouse::MouseEventType::MOUSE_MOVE:
		_touchPos = touchpos;
		break;
	case EventMouse::MouseEventType::MOUSE_UP:
		character->actions[command::SHOOT] = value;
		_touchPos = Vec2(0, 0);
		break;
	default:
		break;
	}
}
#else

#endif

void Command::handleActionsCharacter(shared_ptr<Character>& character, float dt)
{
	character->setArrowWorldSpace(character->sprite->getParent()->convertToWorldSpace(character->sprite->getPosition()));	//update to rotate this
	float incre = 5.f;
	if (character->actions[command::MOVE_UP])
	{
		move(character, Vec2(0, incre));
	}
	if (character->actions[command::MOVE_DOWN])
	{
		move(character, Vec2(0, -incre));
	}
	if (character->actions[command::MOVE_LEFT])
	{
		move(character, Vec2(-incre, 0));
	}
	if (character->actions[command::MOVE_RIGHT])
	{
		move(character, Vec2(incre, 0));
	}
	if (character->actions[command::SHOOT])
	{
		if (_countshoottime >= _shootinterval)
		{
			shot(character);
			_countshoottime = 0.f;
		}
	}

	_countshoottime += dt;
}

void Command::move(shared_ptr<Character>& character, const Vec2& speed)
{
    Vec2 newpos = character->sprite->getPosition() + speed;
    character->sprite->setPosition(newpos);
}

void Command::shot(shared_ptr<Character>& character)
{
	Vec2 poscreate = character->sprite->getPosition();

	//shoot the bullet
	Vec2 offset = _touchPos - character->sprite->getPosition();

	/*if (offset.x < 0)
	{
		return;
	}*/

	auto bullet = Sprite::create(BULLET1);
	//bullet->setRotation(character->sprite->getRotation());
	bullet->setPosition(poscreate);
	float ratio = 0.25;
	bullet->setScale(ratio);
	character->sprite->getParent()->addChild(bullet);

	//physic for bullet
	Size bulletsize = bullet->getContentSize();
	PhysicsBody* body = PhysicsBody::createBox(bulletsize);
	body->setRotationEnable(true);
	//body->setGravityEnable(false);
	if (character->_type == Character::typecharacter::PLAYER)
	{
		body->setContactTestBitmask(PHYSICS_BULLET_PLAYER);
		body->setCategoryBitmask(PHYSICS_BULLET_PLAYER);
		body->setCollisionBitmask(PHYSICS_BULLET_PLAYER);
		
	}
	bullet->setPhysicsBody(body);

	bullet->setRotation(character->sprite->getRotation());	//set rotate after set physicsbody to physicsbody right with rotate of bullet sprite
	//calculate target of bullet

	offset.normalize();
	Vec2 shootAmount = offset * 1000;
	Vec2 realPosTo = shootAmount + poscreate;

	auto move = MoveTo::create(BULLET1_SPEED, realPosTo);
	auto release = RemoveSelf::create();
	auto finalaction = Sequence::createWithTwoActions(move, release);
	finalaction->setTag(actiontag::SHOOT_MOVE);
	bullet->runAction(finalaction);

	//body->setVelocity(shootAmount);	//if want to colli
}

void Command::handleCollisionWithScreen(shared_ptr<Character> &character) { 
    if(character->actions[command::MOVE_LEFT])
        character->actions[command::MOVE_LEFT] = false;
    if(character->actions[command::MOVE_RIGHT])
        character->actions[command::MOVE_RIGHT] = false;
    if(character->actions[command::MOVE_UP])
        character->actions[command::MOVE_UP] = false;
    if(character->actions[command::MOVE_DOWN])
        character->actions[command::MOVE_DOWN] = false;
}

