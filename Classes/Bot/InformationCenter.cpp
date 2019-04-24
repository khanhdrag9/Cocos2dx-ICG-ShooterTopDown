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
#include "../Physics/RigidBodyCircle.h"
#include "BotManager.h"
#include "Game.h"
//#include "../Physics/RigidWorld.h"


InformationCenter::InformationCenter():
	_isStop(false)
{
#if DEBUG_GRAHP
	_draw = DrawNode::create();
	_draw->retain();
#endif
}

InformationCenter::~InformationCenter()
{
	CC_SAFE_DELETE(_draw);
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

            _graph.emplace_back(x, y);
		}
	}

#if DEBUG_GRAHP
	_draw->clear();
	_draw->removeFromParentAndCleanup(true);
	Game::getInstance()->getCurrentState()->addChild(_draw);
#endif
}

void InformationCenter::startThreads()
{
    _threadDetectAround = thread(&InformationCenter::threadAI, this);
}

void InformationCenter::threadAI()
{
    while(!_isStop)
    {
        
    }
    
    _listBot.clear();
    _graph.clear();
}

void InformationCenter::update(float dt)
{
}

queue<Vec2> InformationCenter::findWayToPoint(const Vec2& target)
{
    
}

void InformationCenter::pushBot(shared_ptr<Bot> bot)
{
	_listBot.push_back(bot);
}

void InformationCenter::clear()
{
	_isStop = true;
	_threadDetectAround.join();

	/*_listBot.clear();*/
	
}

