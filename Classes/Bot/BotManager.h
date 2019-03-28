#pragma once
#include "../Defines/Templates.h"
#include "../Headers.h"

class Bot;
class LinkPosition;

class BotManager : public pattern::Singleton<BotManager>
{
	vector<shared_ptr<Bot>> _listBots;
    map<string, shared_ptr<LinkPosition>> _mapPosition;

public:
	BotManager();
	~BotManager();

	void init();
	void clear();

	void addBot(shared_ptr<Bot> bot);
	int countBots() const;
	void update(float dt);

	void initBots();
    void initMovePosition();
	shared_ptr<Bot> createBot();
	shared_ptr<Bot> getBot(const int& index);

	shared_ptr<LinkPosition> getNextLinkPosition(shared_ptr<Bot> bot, bool isRandom);
};
