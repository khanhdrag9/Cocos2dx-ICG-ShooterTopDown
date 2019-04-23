//
//  InformationCenter.cpp
//  ShooterTopDown2D
//
//  Created by Khanh on 4/13/19.
//

#include "InformationCenter.h"
#include "Informations.h"
#include "../Characters/Character.h"
#include "Bot.h"
#include "../Commands/CommandMoveBy.h"
#include "../Commands/CommandMoveTo.h"
#include "../Physics/RigidBody.h"
#include "BotManager.h"
#include "Game.h"
#include "../Physics/RigidWorld.h"



InformationCenter::InformationCenter()
{
#if DEBUG_GRAHP
	_draw = DrawNode::create();
	_draw->retain();
#endif
}

InformationCenter::~InformationCenter()
{
   
}

void InformationCenter::initGraph(TMXTiledMap * tileMap)
{
	auto grahp = tileMap->getObjectGroup("Graph");
	if (grahp)
	{
		for (int i = 1; i <= grahp->getObjects().size(); i++)
		{
			string name = to_string(i);
			auto pointInTile1 = grahp->getObject(name);
			if (pointInTile1.size() == 0)continue;

			float x = pointInTile1.at("x").asFloat();
			float y = pointInTile1.at("y").asFloat();

			//_graph.emplace_back(x, y);
		}
	}

	/*BotManager* botMgr = BotManager::getInstance();
	for (int i = 0; i < botMgr->countBots(); i++)
		pushBot(botMgr->getBot(i));*/

#if DEBUG_GRAHP
	_draw->clear();
	_draw->removeFromParentAndCleanup(true);
	Game::getInstance()->getCurrentState()->addChild(_draw);
	for (auto& point : _graph)
		_draw->drawDot(point, 10.f, Color4F::GREEN);
	_draw->autorelease();
#endif
}

void InformationCenter::update(float dt)
{
	for (auto begin = _listBot.begin(); begin != _listBot.end(); )
	{
		shared_ptr<Bot>& bot = *begin;
		if (bot->isDestroyed())
		{
			begin = _listBot.erase(begin);
			continue;
		}
		
//        float speedBot = bot->getSpeedMove();
//        Vec2 velocity = getRandomVec(speedBot);
//        //shared_ptr<Command> cmd = CommandMoveTo::createCommandMoveTo(speedBot, _graph[0]);
//        shared_ptr<Command> cmd = CommandMoveBy::createCommandMoveBy(velocity, 100.f);
//        bot->pushCommand(cmd);
        
        float dimention = 300.f;
        float rotationBot = bot->_sprite->getRotation();
        auto boundingBox = bot->_sprite->getBoundingBox();
        Vec2 startPoint[3]{
            bot->_sprite->getPosition(),
            Vec2(boundingBox.getMinX(), boundingBox.getMaxY()),
            Vec2(boundingBox.getMaxX(), boundingBox.getMaxY())
        };
        Vec2 endPoint[3];
        for (int i = 0; i < 3; i++)
        {
            float x = sin(CC_DEGREES_TO_RADIANS(rotationBot) * dimention);
            float y = cos(CC_DEGREES_TO_RADIANS(rotationBot) * dimention);
            
        }

        auto listLine = Game::getInstance()->getRigidWord()->getListLines();
        

		++begin;
	}
}

void InformationCenter::pushBot(shared_ptr<Bot> bot)
{
	_listBot.push_back(bot);
}

list<Vec2> InformationCenter::findAroundPoint(const Vec2 & point, shared_ptr<Bot> bot)
{
	auto boundingBox = bot->_sprite->getBoundingBox();
	Vec2 startPoint[3]{
		bot->_sprite->getPosition(),
		Vec2(boundingBox.getMinX(), boundingBox.getMaxY()),
		Vec2(boundingBox.getMaxX(), boundingBox.getMaxY())
	};

    auto listLine = Game::getInstance()->getRigidWord()->getListLines();

	for (auto& point : startPoint)
	{

	}

	return list<Vec2>();
}

list<Vec2> InformationCenter::findNearestToPoint(const Vec2 & point)
{

	return list<Vec2>();
}

void InformationCenter::clear()
{
	_listBot.clear();
	_graph.clear();
}

Vec2 InformationCenter::getRandomVec(const float& speed)
{
    Vec2 listVec[]{
        Vec2(0, speed),
        Vec2(0, -speed),
        Vec2(speed, 0),
        Vec2(-speed, 0)
    };
    
    return listVec[random(0, 3)];
}
