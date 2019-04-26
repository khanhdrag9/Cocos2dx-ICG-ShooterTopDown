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
#include "../Defines/constants.h"
#include "../Characters/Player.h"
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
	for (auto begin = _listBot.begin(); begin != _listBot.end();)
	{
		BotFindWay& bot = *begin;
		if (bot.bot->isDestroyed())
		{
			bot.clear();
			begin = _listBot.erase(begin);
			continue;
		}

		if (bot.isFinish && bot.isReady)	//start thread
		{
			Vec2 botPosition = bot.bot->_sprite->getPosition();
			bot.isFinish = false;
			bot.isReady = false;
			auto lamda = [this](Vec2 position, Vec2 target, BotFindWay* bf) -> queue<Vec2>
			{
#if DEBUG_GRAHP
                //_canMovePointDrawer->clear();
#endif
				float radius = 0.f;
				if (bf)
				{
					if (auto body = dynamic_pointer_cast<RigidBodyCircle>(bf->bot->_rigidBody))
						radius = body->getRadius();
				}

				vector<Vec2> grahpAvaiable = _graph;
				grahpAvaiable.push_back(target);
				std::random_shuffle(grahpAvaiable.begin(), grahpAvaiable.end());
				queue<Vec2> way;
				findWayToPoint(position, target, grahpAvaiable, way, radius);
				
				if (bf)
				{
					std::lock_guard<mutex> gruard(_m);
					bf->isFinish = true;
					bf->isThreadAvaiable = true;
				}
				return way;
			};

			Vec2 target = _graph.at(random(0, (int)_graph.size() - 1));
			bot.task = std::async(launch::async, lamda, botPosition, target, &bot);
		}

		if (bot.isFinish && bot.isThreadAvaiable)
		{
			auto way = bot.task.get();
			bot.isThreadAvaiable = false;
			//Move Bot
			if (way.size() == 0)
			{
				bot.isReady = true;
			}
			else
			{
				while (way.size() > 0)
				{
					shared_ptr<Command> cmd = CommandMoveTo::createCommandMoveTo(bot.bot->getSpeedMove(), way.front());
					bot.commands.push(cmd);
					way.pop();
				}

				bot.status = statusBot::WALK;
			}
			//bot.isReady = true;	//for test
		}

		if (bot.status == statusBot::WALK)
		{
			std::lock_guard<mutex> guard(_m);

			if (bot.commands.size() > 0)
			{
				auto cmd = bot.commands.front();
				if (cmd->getName() == constants::command_move_to)
				{
					if (bot.bot->pushCommand(cmd))
					{
						if (auto moveTo = dynamic_pointer_cast<CommandMoveTo>(cmd))
						{
							bot.bot->_sprite->setRotation(getRotateForwardAPoint(bot.bot, moveTo->getTarget()));
						}
						bot.commands.pop();
					}
				}
			}
			else if(bot.bot->_rigidBody->_velocity == Vec2::ZERO)
			{
				bot.status = statusBot::NONE;
				bot.isReady = true;
			}
		}

		if (bot.status == statusBot::COLLISION)
		{
			bot.status = statusBot::NONE;
			bot.isReady = true;
			while (bot.commands.size() > 0)
				bot.commands.pop();
			bot.bot->releaseCommands();
		}

		bot.bot->update(dt);
		bot.countDetect.first += dt;
		++begin;
	}
}

list<Vec2> InformationCenter::findPointAvaiableAroud(Vec2 position, vector<Vec2>& arrayFind, float radius)
{
	vector<Line> lines;
	{
		std::lock_guard<mutex> gruard(_m);
		lines = Game::getInstance()->getRigidWord()->getListLines();
	}

	for (auto begin = arrayFind.begin(); begin != arrayFind.end();)
	{
		if (*begin == position)
			begin = arrayFind.erase(begin);
		else
			++begin;
	}

	radius *= 1.5f;
	list<Vec2> result;
	for (auto begin = arrayFind.begin(); begin != arrayFind.end(); ++begin)
	{
		auto& pointGrahp = *begin;

		if ((pointGrahp - position).length() > 1000.f)
			continue;

		pair<Vec2, Vec2> checkAvaiable[]
		{
			pair<Vec2, Vec2>(position, pointGrahp),
			pair<Vec2, Vec2>(Vec2(position.x - radius, position.y),Vec2(pointGrahp.x - radius, pointGrahp.y)),	//left
			pair<Vec2, Vec2>(Vec2(position.x + radius, position.y),Vec2(pointGrahp.x + radius, pointGrahp.y)),	//right
			pair<Vec2, Vec2>(Vec2(position.x, position.y + radius),Vec2(pointGrahp.x, pointGrahp.y + radius)),	//top
			pair<Vec2, Vec2>(Vec2(position.x, position.y - radius),Vec2(pointGrahp.x, pointGrahp.y - radius))	//bot
		};

		bool isIntersect = false;
		for (auto& line : lines)
		{
			for (auto& pair : checkAvaiable)
			{
				if (Vec2::isSegmentIntersect(pair.first, pair.second, line.start, line.end))
				{
					isIntersect = true;
					break;
				}
			}
			if (isIntersect)break;
		}

		if(!isIntersect)result.push_back(Vec2(pointGrahp));
	}


	return result;
}

bool InformationCenter::findWayToPoint(Vec2 start, Vec2 target, vector<Vec2>& grahp, queue<Vec2>& result, float radius)
{
	result.push(start);

	list<Vec2> around = findPointAvaiableAroud(start, grahp, radius);
	if (around.size() == 0)return false;

	auto findTarget = std::find(around.begin(), around.end(), target);
	if (findTarget != around.end())
	{
		result.push(Vec2(*findTarget));
		return true;
	}
	else
	{
		for (auto& point : around)
		{
			if (findWayToPoint(point, target, grahp, result, radius))
			{
				return true;
			}
		}
	}

	return false;
}

float InformationCenter::getRotateForwardAPoint(shared_ptr<Character> character, const Vec2& point) const
{
	Vec2 vectorAngle = point - character->_sprite->getPosition();
	float angle = atan2(vectorAngle.y, vectorAngle.x);
	return CC_RADIANS_TO_DEGREES(-angle) + 90;
}

void InformationCenter::pushBot(shared_ptr<Bot> bot)
{
	_listBot.emplace_back(bot);
}

InformationCenter::BotFindWay & InformationCenter::findBotWayByBot(const shared_ptr<Character>& character)
{
	for (auto& bot : _listBot)
	{
		if (&(*bot.bot) == &(*character))
			return bot;
	}
	return BotFindWay();
}

void InformationCenter::clear()
{
	_isStop = true;
	//_threadDetectAround.join();

	/*_listBot.clear();*/
	for (auto& bot : _listBot)
		bot.clear();
	_listBot.clear();
	_graph.clear();
}

