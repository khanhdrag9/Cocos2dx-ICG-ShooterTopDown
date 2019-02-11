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

void BotManager::createBot(const Vec2& position)
{
	shared_ptr<Bot> bot = make_shared<Bot>();
	bot->sprite->setTag(objecttag::ENEMY);

}

void BotManager::init()
{
	_command = make_unique<Command>();

}


