#pragma once
#include "Headers.h"

class Character;
class GamePlay;

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

	enum actiontag
	{
		SHOOT_MOVE
	};

private:
	Vec2 _touchPos;

	//count time for shoot
	float _countshoottime;
	float _shootinterval;

	GamePlay* _gameplay;
public:
	Command();
	void setGamePlay(GamePlay* gameplay);
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
	void remote(shared_ptr<Character> character, const EventKeyboard::KeyCode& code, bool value);
	void remote(shared_ptr<Character> character, const Vec2& touchpos, const EventMouse::MouseEventType& code, bool value);
#else
	
#endif
    void moveToPoint(shared_ptr<Character>& character, const Vec2& point, const float& time);
    void moveFollowPoints(shared_ptr<Character>& character, std::vector<Vec2>& points, const float& totalTime);
	void move(shared_ptr<Character>& character , const Vec2& speed);
    void rotate(shared_ptr<Character>& character, const Vec2& loc);
	void shot(shared_ptr<Character>& character);
	void handleActionsCharacter(shared_ptr<Character>& character, float dt);
    void handleCollisionWithScreen(shared_ptr<Character>& character);
    bool checkcollisionatpos(const shared_ptr<Character>& character, const Vec2& pos);
};

struct Commands
{
private:
	map<Command::command, bool> commands;
public:
	bool operator[](Command::command cmd) const
	{
		for (auto& c : commands)
		{
			if (c.first == cmd)
			{
				return c.second;
			}
		}

		//CCLOG("[GET Command]Character does not contain this command!");
		return false;
	}

	void AddCommand(Command::command cmd, bool value = false)
	{
		commands[cmd] = value;
	}

	void SetCommand(Command::command cmd, bool value)
	{
		for (auto& c : commands)
		{
			if (c.first == cmd)
			{
				c.second = value;
			}
		}

		CCLOG("[SET Command]Character does not contain this command!");
	}

	void clear()
	{
		commands.clear();
	}
};
