#include "BotManager.h"
#include "Bot.h"
#include "../Characters/Player.h"
#include "../Game.h"
#include "../Physics/RigidBody.h"
#include "../Physics/RigidWorld.h"
#include "BotMoveMap.h"
#include "../Commands/CommandMoveTo.h"
#include "InformationCenter.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Creations.h"

BotManager::BotManager()
{
}

BotManager::~BotManager()
{
	clear();
}

void BotManager::init()
{
}

void BotManager::clear()
{
    for(auto& bot : _listBots)
        bot->destroy();
	_listBots.clear();
    _botCreations.clear();
}

void BotManager::addBot(shared_ptr<Bot> bot)
{
	_listBots.push_back(bot);
}

int BotManager::countBots() const
{
	return (int)_listBots.size();
}

void BotManager::update(float dt)
{
	for (auto& bot : _listBots)
	{
		if (bot->isCanTriggerShoot())
			bot->setStatus(Bot::Status::SHOOT);

		if (bot->isCanTriggerWalk())
			bot->setStatus(Bot::Status::WALK);
		
		if(!bot->isCanTriggerWalk() && !bot->isCanTriggerShoot())
			bot->setStatus(Bot::Status::STOP);
		
		bot->update(dt);
	}
}

void BotManager::initBots()
{
	int countEnemies = (int)_botCreations.size();

	//get position to init bots
	Vec2 playerPosition = Game::getInstance()->getPlayer()->_sprite->getPosition();
	vector<Vec2> history;
	for (int i = 0; i < countEnemies;)
	{
		Vec2 pos = Game::getInstance()->getRandomPosition();
		auto find = std::find(history.begin(), history.end(), pos);
		if (find == history.end() && pos != playerPosition)
		{
			history.push_back(pos);
			++i;
		}
	}

	for (int i = 0; i < countEnemies; i++)
	{
		auto bot = createBot(&_botCreations[i]); //create bot here, use player for test, use Bot instead of
		Game::getInstance()->getRigidWord()->createRigidBodyCircle(bot);
		bot->_rigidBody->setTag(RigidBody::tag::ENEMY);
        bot->_sprite->setPosition(history[i-1]);
//        bot->_sprite->setPosition(Vec2(192, 192));

        bot->setWalk(true);

		Game::getInstance()->getCurrentState()->addChild(bot->_sprite);
	}
}

shared_ptr<Bot> BotManager::createBot(CharacterCreation* creation)
{
	auto bot = make_shared<Bot>();
	bot->init(creation);
	_listBots.push_back(bot);

	InformationCenter::getInstance()->pushBot(bot);

	return bot;
}

shared_ptr<Bot> BotManager::getBot(const int & index)
{
	if (index >= _listBots.size())
		return nullptr;

	return _listBots[index];
}

void BotManager::setBotCreations(const vector<CharacterCreation>& creations)
{
    for(auto& c : creations)
        _botCreations.emplace_back(c);
}
