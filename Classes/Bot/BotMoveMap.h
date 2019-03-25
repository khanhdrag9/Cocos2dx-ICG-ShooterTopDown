#pragma once
#include "Headers.h"

class CommandMoveTo;
class Bot;

class LinkPosition
{
public:
    LinkPosition(){}
    LinkPosition(const Vec2& first, const vector<Vec2>& others);
    ~LinkPosition();
    
    Vec2 first;
    vector<Vec2> others;
};

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
