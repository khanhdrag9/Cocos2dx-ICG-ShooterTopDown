#include "BotManager.h"
#include "Command.h"
#include "Bot.h"

BotManager::BotManager() :
	_countTime(0.f),
	_interval(BOT_HANDLE_INTERVAL)
{
	init();
}

BotManager::~BotManager()
{
	for (auto& b : _bots)b = nullptr;
	_bots.clear();
}

shared_ptr<Character> BotManager::createBot(const Vec2& position, Node* parrent)
{
	shared_ptr<Character> bot = make_shared<Bot>();
	bot->sprite->setTag(objecttag::ENEMY);
	bot->sprite->setPosition(position);
	float ratio = 0.8f;
	bot->sprite->setScale(ratio);
	
	bot->sprite->retain();
	bot->sprite->removeFromParentAndCleanup(true);
	parrent->addChild(bot->sprite);
	bot->sprite->autorelease();

	_bots.push_back(bot);

	return bot;
}

void BotManager::update(float dt, unique_ptr<Command>& command)
{
	if (_countTime >= _interval)
	{
		for (auto& bot : _bots)
		{
			command->handleActionsCharacter(bot, dt);
		}
		_countTime = 0.f;
	}
	_countTime += dt;
}

shared_ptr<Character> BotManager::getBot(const int & index) const
{
	if (index < _bots.size())
		return _bots[index];
	return shared_ptr<Character>();
}

void BotManager::init()
{
}


