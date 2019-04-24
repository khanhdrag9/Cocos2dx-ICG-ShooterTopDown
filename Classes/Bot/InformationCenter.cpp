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


float InformationCenter::dimension_detect_around = 300.f;
float InformationCenter::dimension_detect_collision = 64.f;


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

			//_graph.emplace_back(x, y);
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
	_threadDetectAround = thread(&InformationCenter::detectAroundAvaiable, this);
}

void InformationCenter::update(float dt)
{

	/*for (auto begin = _listBot.begin(); begin != _listBot.end(); )
	{
		shared_ptr<Bot>& bot = *begin;
		if (bot->isDestroyed())
		{
			begin = _listBot.erase(begin);
			continue;
		}

		++begin;
	}*/


}

void InformationCenter::pushBot(shared_ptr<Bot> bot)
{
	_listBot.push_back(make_unique<RemoteBot>(bot));
}

void InformationCenter::clear()
{
	_isStop = true;
	_threadDetectAround.join();

	/*_listBot.clear();*/
	
}

void InformationCenter::detectAroundAvaiable()
{
	while (!_isStop)
	{
#if DEBUG_GRAHP
		_draw->clear();
#endif
		for (auto begin = _listBot.begin(); begin != _listBot.end(); )
		{
			unique_ptr<RemoteBot>& remote = *begin;
			shared_ptr<Bot> bot = remote->bot;
			if (bot->isDestroyed() || !bot->_sprite)
			{
				begin = _listBot.erase(begin);
				continue;
			}

			status_remote status = remote->status;

			if (status == status_remote::WALKING)
			{

			}

			//if (status == status_remote::NONE)
			{
				//push command to move a bot
				Vec2 velocity = detectNewRoadACharacter(remote);
				shared_ptr<Command> cmd = CommandMoveBy::createCommandMoveBy(velocity, 100.f);
				_m.lock();
				bot->pushCommand(cmd);
				_m.unlock();
				remote->status = status_remote::WALKING;
			}
			
			


			if (_isStop)
				break;
			++begin;
		}


#if DEBUG_GRAHP
		for (auto& point : _graph)
			_draw->drawDot(point, 10.f, Color4F::GREEN);
#endif

		int dt = int(Director::getInstance()->getDeltaTime() * 1000.f);
		std::chrono::milliseconds ms(dt);
		std::this_thread::sleep_for(ms);
	}

	_listBot.clear();
	_graph.clear();
}

Vec2 InformationCenter::detectNewRoadACharacter(unique_ptr<RemoteBot>& remote) const
{
	shared_ptr<Bot> bot = remote->bot;
	float radiusBody = 0.f;
	if (auto circleBody = dynamic_pointer_cast<RigidBodyCircle>(bot->_rigidBody))
		radiusBody = circleBody->getRadius();
	else
		radiusBody = bot->_sprite->getContentSize().width;
	float border = 0.f;
	Vec2 position = bot->_sprite->getPosition();
	Vec2 startPoint[]
	{
		Vec2(position.x - radiusBody - border, position.y), //left
		Vec2(position.x + radiusBody + border, position.y), //right
		Vec2(position.x, position.y + radiusBody + border), //top
		Vec2(position.x, position.y - radiusBody - border) //bot
	};

	float dimension = dimension_detect_around;
	Vec2 dleft = Vec2(-1, 0) * dimension;
	Vec2 dright = Vec2(1, 0) * dimension;
	Vec2 dtop = Vec2(0, 1) * dimension;
	Vec2 dbot = Vec2(0, -1) * dimension;

	Line lineChecks[]
	{
		//from add more border
		////left
		//Line(startPoint[0], Vec2(startPoint[0] + dleft)),
		//Line(startPoint[2], Vec2(startPoint[2] + dleft)),
		//Line(startPoint[3], Vec2(startPoint[3] + dleft)),
		////right
		//Line(startPoint[1], Vec2(startPoint[1] + dright)),
		//Line(startPoint[2], Vec2(startPoint[2] + dright)),
		//Line(startPoint[3], Vec2(startPoint[3] + dright)),
		////top
		//Line(startPoint[0], Vec2(startPoint[0] + dtop)),
		//Line(startPoint[1], Vec2(startPoint[1] + dtop)),
		//Line(startPoint[2], Vec2(startPoint[2] + dtop)),
		////bot
		//Line(startPoint[0], Vec2(startPoint[0] + dbot)),
		//Line(startPoint[1], Vec2(startPoint[1] + dbot)),
		//Line(startPoint[3], Vec2(startPoint[3] + dbot))

		//from center to...
		////left
		//Line(position, Vec2(startPoint[0] + dleft)),
		//Line(position, Vec2(startPoint[2] + dleft)),
		//Line(position, Vec2(startPoint[3] + dleft)),
		////right
		//Line(position, Vec2(startPoint[1] + dright)),
		//Line(position, Vec2(startPoint[2] + dright)),
		//Line(position, Vec2(startPoint[3] + dright)),
		////top
		//Line(position, Vec2(startPoint[0] + dtop)),
		//Line(position, Vec2(startPoint[1] + dtop)),
		//Line(position, Vec2(startPoint[2] + dtop)),
		////bot
		//Line(position, Vec2(startPoint[0] + dbot)),
		//Line(position, Vec2(startPoint[1] + dbot)),
		//Line(position, Vec2(startPoint[3] + dbot))

		//from border
		//left
		Line(Vec2(position.x - radiusBody, position.y), Vec2(startPoint[0] + dleft)),
		Line(Vec2(position.x, position.y + radiusBody), Vec2(startPoint[2] + dleft)),
		Line(Vec2(position.x, position.y - radiusBody), Vec2(startPoint[3] + dleft)),
		//right
		Line(Vec2(position.x + radiusBody, position.y), Vec2(startPoint[1] + dright)),
		Line(Vec2(position.x, position.y + radiusBody), Vec2(startPoint[2] + dright)),
		Line(Vec2(position.x, position.y - radiusBody), Vec2(startPoint[3] + dright)),
		//top
		Line(Vec2(position.x - radiusBody, position.y), Vec2(startPoint[0] + dtop)),
		Line(Vec2(position.x + radiusBody, position.y), Vec2(startPoint[1] + dtop)),
		Line(Vec2(position.x, position.y + radiusBody), Vec2(startPoint[2] + dtop)),
		//bot
		Line(Vec2(position.x - radiusBody, position.y), Vec2(startPoint[0] + dbot)),
		Line(Vec2(position.x + radiusBody, position.y), Vec2(startPoint[1] + dbot)),
		Line(Vec2(position.x, position.y - radiusBody), Vec2(startPoint[3] + dbot))
	};

	float speedMove = bot->getSpeedMove();
	Direction_des listDirection[]
	{
		Direction_des(true, Vec2(0, -speedMove)),	//left
		Direction_des(true, Vec2(0, speedMove)),	//right
		Direction_des(true, Vec2(speedMove, 0)),	//top
		Direction_des(true, Vec2(-speedMove, 0))	//bot
	};

	auto& listLine = Game::getInstance()->getRigidWord()->getListLines();
	auto lamdaCheckMinLine = [](const Line& check, const Line& checkWith, const Vec2& prePoint)	//lay ra collision point cua 2 Line roi so sanh voi prePoint -> lay cai nho hhon
	{
		Vec2 collisionPoint = Vec2::getIntersectPoint(check.start, check.end, checkWith.start, checkWith.end);
		return ((collisionPoint - check.start).length() < (prePoint - check.start).length()) ? collisionPoint : prePoint;
	};

	for (int i = 0; i < 12; i += 3)
	{
		Vec2 collisionPoint1 = lineChecks[i].end;
		Vec2 collisionPoint2 = lineChecks[i + 1].end;
		Vec2 collisionPoint3 = lineChecks[i + 2].end;

		bool isIntersect = false;
		for (auto& line : listLine)
		{
			/*if (Vec2::isSegmentIntersect(lineChecks[i].start, lineChecks[i].end, line.start, line.end) ||
				Vec2::isSegmentIntersect(lineChecks[i + 1].start, lineChecks[i + 1].end, line.start, line.end) ||
				Vec2::isSegmentIntersect(lineChecks[i + 2].start, lineChecks[i + 2].end, line.start, line.end))
				isIntersect = true;*/

			if (Vec2::isSegmentIntersect(lineChecks[i].start, lineChecks[i].end, line.start, line.end))
			{
				isIntersect = true;
				collisionPoint1 = lamdaCheckMinLine(lineChecks[i], line, collisionPoint1);
			}

			if (Vec2::isSegmentIntersect(lineChecks[i + 1].start, lineChecks[i + 1].end, line.start, line.end))
			{
				isIntersect = true;
				collisionPoint2 = lamdaCheckMinLine(lineChecks[i + 1], line, collisionPoint2);
			}

			if (Vec2::isSegmentIntersect(lineChecks[i + 2].start, lineChecks[i + 2].end, line.start, line.end))
			{
				isIntersect = true;
				collisionPoint3 = lamdaCheckMinLine(lineChecks[i + 2], line, collisionPoint3);
			}
		}

		/*listDirection[i / 3].collisionPoints.push_back(Line(lineChecks[i].start, collisionPoint1));
		listDirection[i / 3].collisionPoints.push_back(Line(lineChecks[i + 1].start, collisionPoint2));
		listDirection[i / 3].collisionPoints.push_back(Line(lineChecks[i + 2].start, collisionPoint3));*/

		listDirection[i / 3].collisionPoints.emplace_back(lineChecks[i].start, collisionPoint1);
		listDirection[i / 3].collisionPoints.emplace_back(lineChecks[i + 1].start, collisionPoint2);
		listDirection[i / 3].collisionPoints.emplace_back(lineChecks[i + 2].start, collisionPoint3);


		if (isIntersect)
		{
			/*if (i < 3)listDirection[LEFT].avaiable = false;
			else if (i < 6)listDirection[RIGHT].avaiable = false;
			else if (i < 9)listDirection[TOP].avaiable = false;
			else if (i < 12)listDirection[BOT].avaiable = false;*/

			listDirection[i / 3].avaiable = false;
			continue;
		}
	}

	for (auto& dd : listDirection)
	{
		for (auto& line : dd.collisionPoints)
		{
#if DEBUG_GRAHP
			_draw->drawLine(line.start, line.end, Color4F::BLUE);
#endif
			float lenght = line.lenght();
			if (lenght < dimension_detect_collision)
			{

			}
		}

	}


	bool allIsFalse = std::all_of(std::begin(listDirection), std::end(listDirection), [](const Direction_des& des)
	{
		return des.avaiable == false;
	});
	bool allIsCollision = std::all_of(std::begin(listDirection), std::end(listDirection), [](const Direction_des& des)
	{
		for (auto& line : des.collisionPoints)
			if (line.lenght() < dimension_detect_collision)
				return true;
	});

	if (allIsFalse)
	{
		if (allIsCollision)
		{
			
		}
		else
		{
			
		}
	}

	Vec2 velocity = Vec2::ZERO;
	while (true)
	{
		auto dd = listDirection[random(0, 3)];
		if (allIsFalse && allIsCollision)
		{
			velocity = dd.velocity;
			break;
		}

		if (dd.avaiable)
		{
			velocity = dd.velocity;
			break;
		}

	}
	return velocity;
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
