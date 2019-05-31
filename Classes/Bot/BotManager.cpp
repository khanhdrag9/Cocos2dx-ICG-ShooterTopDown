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
    Game* game = Game::getInstance();
    bool isDestroy = false;
    for (auto begin = _listBots.begin(); begin != _listBots.end();)
	{
        shared_ptr<Bot> bot = *begin;
        if(bot->isDestroyed())
        {
            begin = _listBots.erase(begin);
            isDestroy = true;
            game->setPlayerKills(game->getPlayerKills() + 1);
            continue;
        }
#if DISIBLE_AI
        bot->update(dt);
#endif
        ++begin;
	}
    
    if(isDestroy && game->isEnableVolumn())
    {
        SimpleAudioEngine::getInstance()->stopAllEffects();
        ResourceManager* resMgr = ResourceManager::getInstance();
        int playerKills = game->getPlayerKills();
        switch (playerKills)
        {
            case 1:
                SimpleAudioEngine::getInstance()->playBackgroundMusic(resMgr->at(res::define::SOUND_FIRSTBLOOD).c_str());
                break;
            case 2:
                SimpleAudioEngine::getInstance()->playBackgroundMusic(resMgr->at(res::define::SOUND_DOUBLEKILL).c_str());
                break;
            case 3:
                SimpleAudioEngine::getInstance()->playBackgroundMusic(resMgr->at(res::define::SOUND_TRIPLEKILL).c_str());
                break;
            case 4:
                SimpleAudioEngine::getInstance()->playBackgroundMusic(resMgr->at(res::define::SOUND_QUADRAKILL).c_str());
                break;
            default:
                break;
        }
        
        
    }
    if(isDestroy && game->getPlayer()){
        int playerKills = game->getPlayerKills();
        Vec2 playerPosition = game->getPlayer()->_sprite->getPosition();
        for(int i = 0; i < playerKills; ++i){
            auto bot = createBot(&_botCreations[RandomHelper::random_int(0, (int)_botCreations.size()-1)]);
            Vec2 position = playerPosition;
            while((position - playerPosition).length() < 1000)position = game->getRandomPosition();
            bot->_sprite->setPosition(position);
        }
    }
}

void BotManager::initBots()
{
	int countEnemies = (int)_botCreations.size();
//    countEnemies = 1;
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
        bot->_sprite->setPosition(history[i]);
	}
}

shared_ptr<Bot> BotManager::createBot(CharacterCreation* creation)
{
	auto bot = make_shared<Bot>();
	bot->init(creation);
	_listBots.push_back(bot);

#if !DISIBLE_AI
    InformationCenter::getInstance()->pushBot(bot);
#endif
    Game::getInstance()->getRigidWord()->createRigidBodyCircle(bot);
    bot->_rigidBody->setTag(RigidBody::tag::ENEMY);
    Game::getInstance()->getCurrentState()->addChild(bot->_sprite);

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

const vector<CharacterCreation>& BotManager::getBotCreations() const
{
    return _botCreations;
}
