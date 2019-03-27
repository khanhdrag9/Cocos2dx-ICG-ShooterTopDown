#include "BotManager.h"
#include "Bot.h"
#include "../Game.h"
#include "../Physics/RigidBody.h"
#include "../Physics/RigidWorld.h"

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
}

void BotManager::addBot(shared_ptr<Bot> bot)
{
	_listBots.push_back(bot);
}

int BotManager::countBots() const
{
	return _listBots.size();
}

void BotManager::update(float dt)
{
	for (auto& bot : _listBots)
	{
		bot->update(dt);
	}
}

void BotManager::initBots()
{
	TMXObjectGroup* objg = Game::getInstance()->getTileMap()->getObjectGroup("Enemy");
	int countEnemies = (int)objg->getObjects().size();

	for (int i = 1; i <= countEnemies; i++)
	{
		auto pos = objg->getObject("enemyPos" + to_string(i));
		float x = pos.at("x").asFloat();
		float y = pos.at("y").asFloat();

		auto bot = createBot(); //create bot here, use player for test, use Bot instead of
		Game::getInstance()->getRigidWord()->createRigidBodyCircle(bot);
		bot->_rigidBody->setTag(RigidBody::tag::ENEMY);
		bot->_sprite->setPosition(x, y);

		Game::getInstance()->getCurrentState()->addChild(bot->_sprite);
	}
}

shared_ptr<Bot> BotManager::createBot()
{
	auto bot = make_shared<Bot>();
	bot->init();
	_listBots.push_back(bot);

	return bot;
}

shared_ptr<Bot> BotManager::getBot(const int & index)
{
	if (index >= _listBots.size())
		return nullptr;

	return _listBots[index];
}
