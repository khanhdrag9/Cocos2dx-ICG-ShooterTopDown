#pragma once
#include "Headers.h"

class Command;
class Bot;

class BotManager : public Singleton<BotManager>
{
public:
	BotManager();
	virtual ~BotManager();

	void createBot(const Vec2& position, Node* parrent);
private:
	void init();
	unique_ptr<Command> _command;

	vector<shared_ptr<Bot>> _bots;
};