#include "BotManager.h"
#include "Bot.h"
#include "../Characters/Player.h"
#include "../Game.h"
#include "../Physics/RigidBody.h"
#include "../Physics/RigidWorld.h"
#include "BotMoveMap.h"
#include "../Commands/CommandMoveTo.h"
#include "InformationCenter.h"

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
	//TMXObjectGroup* objg = Game::getInstance()->getTileMap()->getObjectGroup("Enemy");
	//int countEnemies = (int)objg->getObjects().size();
	int countEnemies = 1;

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

	for (int i = 1; i <= countEnemies; i++)
	{
		/*auto pos = objg->getObject("enemyPos" + to_string(i));
		float x = pos.at("x").asFloat();
		float y = pos.at("y").asFloat();*/

		auto bot = createBot(); //create bot here, use player for test, use Bot instead of
		Game::getInstance()->getRigidWord()->createRigidBodyCircle(bot);
		bot->_rigidBody->setTag(RigidBody::tag::ENEMY);
		//bot->_sprite->setPosition(x, y);
		bot->_sprite->setPosition(history[i-1]);
        
        bot->setWalk(true);

		Game::getInstance()->getCurrentState()->addChild(bot->_sprite);
	}
}

shared_ptr<Bot> BotManager::createBot()
{
	auto bot = make_shared<Bot>();
	bot->init();
	_listBots.push_back(bot);
    
    shared_ptr<description> des = make_shared<des_init>();
    shared_ptr<InformationMoveAround> information = make_shared<InformationMoveAround>(des);
    InformationCenter::getInstance()->pushInformation(bot, information);

	return bot;
}

shared_ptr<Bot> BotManager::getBot(const int & index)
{
	if (index >= _listBots.size())
		return nullptr;

	return _listBots[index];
}
