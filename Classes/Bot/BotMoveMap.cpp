#include "BotMoveMap.h"
#include "Commands/Command.h"
#include "Commands/CommandMoveTo.h"
#include "Bot.h"


LinkPosition::LinkPosition(const Vec2& f, const string& n):
    first(f),
    name(n)
{
    
}

LinkPosition::~LinkPosition()
{
    others.clear();
}

const Vec2 & LinkPosition::get() const
{
	return first;
}

const string& LinkPosition::getName() const
{
	return name;
}

void LinkPosition::push(string other)
{
	others.push_back(other);
}

string LinkPosition::otherAt(const int & index) const
{
	if (index >= others.size())
        return string("");

	return others[index];
}

int LinkPosition::otherSize() const
{
	return (int)others.size();
}

BotMoveMap::BotMoveMap()
{

}

BotMoveMap::BotMoveMap(shared_ptr<Bot> bot, queue<Vec2>& queuePointTo)
{
	init(bot, queuePointTo);
}

BotMoveMap::~BotMoveMap()
{
	while (_queueMoveTo.size() > 0)
		_queueMoveTo.pop();
}


void BotMoveMap::init(shared_ptr<Bot> bot, queue<Vec2>& queuePointTo)
{
	_bot = bot;

	while (queuePointTo.size() > 0)
	{
		Vec2 point = queuePointTo.front();
		
		shared_ptr<CommandMoveTo> cmdMove = CommandMoveTo::createCommandMoveTo(_bot->getSpeedMove(), point);
		_queueMoveTo.push(cmdMove);
		queuePointTo.pop();
	}
	  
}
