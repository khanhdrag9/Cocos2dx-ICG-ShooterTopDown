#pragma once
#include "Headers.h"

class Character;

class Command
{
public:
	enum class command : int
	{
		MOVE_UP,
		MOVE_DOWN,
		MOVE_RIGHT,
		MOVE_LEFT,
		SHOOT
	};

private:

public:
	Command();
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
	void remote(shared_ptr<Character> character, const EventKeyboard::KeyCode& code, bool value);
	void remote(shared_ptr<Character> character, const EventMouse::MouseEventType& code, bool value);
#else
	
#endif

	void move(shared_ptr<Character>& character , const Vec2& speed);
	void shot(shared_ptr<Character>& character);
	void handleActionsCharacter(shared_ptr<Character>& character);
};
