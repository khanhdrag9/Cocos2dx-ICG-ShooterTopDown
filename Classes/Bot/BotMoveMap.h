#pragma once
#include "Headers.h"

class CommandMoveTo;
class Bot;

class BotMoveMap
{
	std::queue<shared_ptr<CommandMoveTo>> _queueMoveTo;
	shared_ptr<Bot> _bot;

public:
	BotMoveMap();
	BotMoveMap(shared_ptr<Bot> bot, queue<Vec2>& queuePointTo);
	virtual ~BotMoveMap();

	void init(shared_ptr<Bot> bot, queue<Vec2>& queuePointTo);
	
};
