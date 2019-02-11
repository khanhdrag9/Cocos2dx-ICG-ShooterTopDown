#pragma once
#include "Headers.h"

class Command;
class Character;

class BotManager : public Singleton<BotManager>
{
public:
	BotManager();
	virtual ~BotManager();

	shared_ptr<Character> createBot(const Vec2& position, Node* parrent);
	void update(float dt, unique_ptr<Command>& command);

	shared_ptr<Character> getBot(const int& index) const;
private:
	void init();

	float _countTime;
	float _interval;

	vector<shared_ptr<Character>> _bots;
};