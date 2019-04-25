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
	_canMovePointDrawer = DrawNode::create();
	_canMovePointDrawer->retain();
#endif
}

InformationCenter::~InformationCenter()
{
#if DEBUG_GRAHP
	CC_SAFE_DELETE(_draw);
	CC_SAFE_DELETE(_canMovePointDrawer);
#endif
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
	for (auto& point : _graph)
		_draw->drawPoint(point, 10, Color4F::RED);

	_canMovePointDrawer->clear();
	_canMovePointDrawer->removeFromParentAndCleanup(true);
	Game::getInstance()->getCurrentState()->addChild(_canMovePointDrawer);
#endif
}

void InformationCenter::startThreads()
{
    //_threadDetectAround = thread(&InformationCenter::threadAI, this);
}

void InformationCenter::threadAI()
{
    while(!_isStop)
    {

		//for (auto begin = _listBot.begin(); begin != _listBot.end(); )
		//{
		//	shared_ptr<Bot> bot = *begin;
		//	if (bot->isDestroyed() || !bot->_sprite)
		//	{
		//		begin = _listBot.erase(begin);
		//		continue;
		//	}

		//	Vec2 botPosition = bot->_sprite->getPosition();
		//	auto pointAround = findPointAvaiableAroud(botPosition, list<Vec2>());


		//}


		////sleep thread base on dt
		//int dt = int(Director::getInstance()->getDeltaTime() * 1000.f);
		//std::chrono::milliseconds ms(dt);
		//std::this_thread::sleep_for(ms);
    }
    
    //_listBot.clear();
    //_graph.clear();
}

void InformationCenter::update(float dt)
{
	for (auto begin = _listBot.begin(); begin != _listBot.end(); ++begin)
	{
		BotFindWay& bot = *begin;

		if (bot.isFinish && bot.isReady)	//start thread
		{
			Vec2 botPosition = bot.bot->_sprite->getPosition();
			bot.isFinish = false;
			bot.isReady = false;
			auto lamda = [this](Vec2 position, Vec2 target, BotFindWay* bf) -> queue<Vec2>
			{
#if DEBUG_GRAHP
                _canMovePointDrawer->clear();
#endif
				auto way = findWayToPoint(position, target);
				
				if (bf)
				{
					std::lock_guard<mutex> gruard(_m);
					bf->isFinish = true;
					bf->isThreadAvaiable = true;
				}
				return way;
			};

			bot.task = std::async(launch::async, lamda, botPosition, _graph[9], &bot);
		}

		if (bot.isFinish && bot.isThreadAvaiable)
		{
			auto way = bot.task.get();
			bot.isThreadAvaiable = false;
			bot.isReady = true;	//for test
		}
	}
}

list<Vec2> InformationCenter::findPointAvaiableAroud(Vec2 position, const list<Vec2>& unless)
{
	vector<Line> lines;
	{
		std::lock_guard<mutex> gruard(_m);
		lines = Game::getInstance()->getRigidWord()->getListLines();
	}

	vector<Vec2> grahpTemp;
	vector<Vec2> grahpAvaiable = _graph;

	for (auto& line : lines)
	{
		for (auto& pointGrahp : grahpAvaiable)
		{
			if (!Vec2::isSegmentIntersect(position, pointGrahp, line.start, line.end) &&
				std::find(unless.begin(), unless.end(), pointGrahp) == unless.end())
			{
				grahpTemp.push_back(pointGrahp);

			}
		}

		grahpTemp.swap(grahpAvaiable);
		grahpTemp.clear();
	}

	list<Vec2> result;
	for (auto point : grahpAvaiable)
		result.push_back(point);

	return result;
}

queue<Vec2> InformationCenter::findWayToPoint(Vec2 start, Vec2 target)
{
	list<Vec2> unless;
	list<Vec2> aroud = findPointAvaiableAroud(start, unless);

	for (auto& point : aroud)
    {
#if DEBUG_GRAHP
		_canMovePointDrawer->drawPoint(point, 10, Color4F::GREEN);
#endif
    }

	return queue<Vec2>();
}

void InformationCenter::pushBot(shared_ptr<Bot> bot)
{
	_listBot.emplace_back(bot);
}

void InformationCenter::clear()
{
	_isStop = true;
	//_threadDetectAround.join();

	/*_listBot.clear();*/
	_listBot.clear();
	_graph.clear();
}

