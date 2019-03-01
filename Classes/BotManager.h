#pragma once
#include "Headers.h"
#include "Command.h"

class Bot;

class BotManager : public Singleton<BotManager>
{
public:
	BotManager();
	virtual ~BotManager();

	shared_ptr<Character> createBot(const Vec2& position, Node* parrent);
	void update(float dt, unique_ptr<Command>& command);

	shared_ptr<Character> getBot(const int& index) const;
    int getBotsCount()const;
private:
	void init();

	float _countTime;
	float _interval;

	vector<shared_ptr<Bot>> _bots;
	
private:
	void stop(shared_ptr<Bot> bot);
	void move(shared_ptr<Bot> bot, Command::command cmd);
	void rotate(shared_ptr<Bot> bot, const float& angle);
	void shot(shared_ptr<Bot> bot);
	void Revert(shared_ptr<Bot> bot, const int& times = 1);
};
