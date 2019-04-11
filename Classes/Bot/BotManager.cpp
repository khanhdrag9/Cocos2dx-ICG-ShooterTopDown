#include "BotManager.h"
#include "Bot.h"
#include "../Characters/Player.h"
#include "../Game.h"
#include "../Physics/RigidBody.h"
#include "../Physics/RigidWorld.h"
#include "BotMoveMap.h"
#include "../Commands/CommandMoveTo.h"

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
	int countEnemies = 5;

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
        bot->setLinkPosition(_mapPosition.begin()->second);

		Game::getInstance()->getCurrentState()->addChild(bot->_sprite);
	}
}

void BotManager::initMovePosition()
{
    TMXObjectGroup* movemapgroup = Game::getInstance()->getTileMap()->getObjectGroup("EnemyMoveMap");
    int count = (int)movemapgroup->getObjects().size();
    
    for(int i = 1; i <= count; i++)
    {
        string name = "enemystep" + to_string(i);
        auto pos = movemapgroup->getObject(name);
        float x = pos.at("x").asFloat();
        float y = pos.at("y").asFloat();
        
        shared_ptr<LinkPosition> linkpos = make_shared<LinkPosition>(Vec2(x, y), name);
        
        auto linknames = pos.at("LinkPosition").asString();
        
        std::istringstream iss(linknames);
        vector<string> listLink(istream_iterator<string>{iss}, istream_iterator<string>());
        for(auto link : listLink)linkpos->push(link);
        
        _mapPosition[name] = linkpos;
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

int getRandom(int start, int end)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<> dist(start, end - 1);

	return dist(mt);
}

shared_ptr<LinkPosition> BotManager::getNextLinkPosition(shared_ptr<Bot> bot, bool isRandom)
{
	if (bot->getLinkPosition() == nullptr)
	{
        return nullptr;
	}

	auto currentLinkPos = bot->getLinkPosition();

	if (int count = currentLinkPos->otherSize() > 0)
	{
		int randomIndex = getRandom(0, currentLinkPos->otherSize());
		string nameNextLink = currentLinkPos->otherAt(randomIndex);
		shared_ptr<LinkPosition> nextlink = _mapPosition[nameNextLink];
        if(nextlink == nullptr)
            return currentLinkPos;
        
		return nextlink;
	}

	return currentLinkPos;
}
