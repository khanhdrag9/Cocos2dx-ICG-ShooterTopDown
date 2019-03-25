#include "BotMoveMap.h"
#include "Commands/Command.h"
#include "Commands/CommandMoveTo.h"
#include "Bot.h"


LinkPosition::LinkPosition(const Vec2& f, const vector<Vec2>& os):
    first(f),
    others(os)
{
    
}

LinkPosition::~LinkPosition()
{
    others.clear();
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
