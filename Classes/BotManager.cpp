#include "BotManager.h"
#include "Bot.h"
#include "Player.h"

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
	shared_ptr<Bot> bot = make_shared<Bot>();
	bot->sprite->setTag(objecttag::ENEMY);
	bot->sprite->setPosition(position);
	float ratio = 0.5f;
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
			//move(bot, Command::command::MOVE_LEFT);
			auto character = static_pointer_cast<Character>(bot);
			command->handleActionsCharacter(character, dt);
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

void BotManager::stop(shared_ptr<Bot> bot)
{
	bot->_states.emplace_back(statetype::STATE_STOP, Value(0));

	bot->actions.SetCommand(Command::command::MOVE_DOWN, false);
	bot->actions.SetCommand(Command::command::MOVE_RIGHT, false);
	bot->actions.SetCommand(Command::command::MOVE_UP, false);
	bot->actions.SetCommand(Command::command::MOVE_LEFT, false);
}

void BotManager::move(shared_ptr<Bot> bot, Command::command cmd)
{
	bot->_states.emplace_back(StateMap<Value>(statetype::STATE_ROTATE, Value((int)cmd)));

	bot->actions.SetCommand(cmd, true);
}

void BotManager::rotate(shared_ptr<Bot> bot, const float & angle)
{
	bot->_states.emplace_back(StateMap<Value>(statetype::STATE_ROTATE, Value(angle)));

	bot->sprite->stopAllActions();
	bot->sprite->runAction(RotateTo::create(0.5f, angle));
}

void BotManager::shot(shared_ptr<Bot> bot)
{
	
}

void BotManager::Revert(shared_ptr<Bot> bot, const int& times)
{
	auto states = bot->_states.back();

}


