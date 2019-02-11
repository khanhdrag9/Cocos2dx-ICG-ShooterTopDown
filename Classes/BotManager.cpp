#include "BotManager.h"
#include "Command.h"
#include "Bot.h"

BotManager::BotManager():
	_command(nullptr)
{
	init();
}

BotManager::~BotManager()
{
	_command = nullptr;
	for (auto& b : _bots)b = nullptr;
	_bots.clear();
}

void BotManager::createBot(const Vec2& position, Node* parrent)
{
	shared_ptr<Bot> bot = make_shared<Bot>();
	bot->sprite->setTag(objecttag::ENEMY);
	bot->sprite->setPosition(position);
	float ratio = 0.8f;
	bot->sprite->setScale(ratio);

	
	bot->sprite->retain();
	bot->sprite->removeFromParentAndCleanup(true);
	parrent->addChild(bot->sprite);
	bot->sprite->autorelease();

	_bots.push_back(bot);
}

void BotManager::init()
{
	_command = make_unique<Command>();
}


