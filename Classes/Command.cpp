#include "Command.h"
#include "Player.h"

Command::Command()
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
void Command::remote(shared_ptr<Character> character, const EventMouse::MouseEventType& code, bool value)
{
	switch (code)
	{
	case EventMouse::MouseEventType::MOUSE_DOWN:
		character->actions[command::SHOOT] = value;
		break;
	case EventMouse::MouseEventType::MOUSE_MOVE:
		
		break;
	case EventMouse::MouseEventType::MOUSE_UP:
		character->actions[command::SHOOT] = value;
		break;
	default:
		break;
	}
}
#else

#endif

void Command::handleActionsCharacter(shared_ptr<Character>& character)
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
		shot(character);
	}
}

void Command::move(shared_ptr<Character>& character, const Vec2& speed)
{
	Vec2 newpos = character->sprite->getPosition() + speed;
	character->sprite->setPosition(newpos);
}

void Command::shot(shared_ptr<Character>& character)
{
	auto bullet = Sprite::create(BULLET1);
	bullet->setRotation(character->sprite->getRotation());
	bullet->setPosition(character->sprite->getPosition());
	float ratio = 0.25;
	bullet->setScale(ratio);

	character->sprite->getParent()->addChild(bullet);
}