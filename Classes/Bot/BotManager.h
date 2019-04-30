#pragma once
#include "../Defines/Templates.h"
#include "../Headers.h"
#include "../Characters/Character.h"

class Bot;
class LinkPosition;

class BotManager : public pattern::Singleton<BotManager>
{
	vector<shared_ptr<Bot>> _listBots;
    vector<CharacterCreation> _botCreations;
public:
	BotManager();
	~BotManager();

	void init();
	void clear();

	void addBot(shared_ptr<Bot> bot);
	int countBots() const;
	void update(float dt);

	void initBots();
	shared_ptr<Bot> createBot(CharacterCreation* creation);
	shared_ptr<Bot> getBot(const int& index);
    void setBotCreations(const vector<CharacterCreation>& creations);
    const vector<CharacterCreation>& getBotCreations() const;
};
