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
#include "../Envoiments/Vision.h"
#include "../Objects/Mag.h"
//#include "../Physics/RigidWorld.h"
#include "../Resource/Creations.h"

void SolutionWay::push(Vec2 first, Vec2 second)
{
    if(_ways.size() == 0)
    {
        //list<Vec2> w = {first, second};
        list<Vec2> w{first, second};
        _ways.push_back(w);
        return;
    }
    
    list<Vec2> newList;
    for(auto& way : _ways)
    {
        if(way.back() == first)
        {
            way.push_back(second);
            break;
        }
        else
        {
            Vec2 back = way.back();
            way.pop_back();
            if(way.back() == first)
            {
                way.push_back(second);
                for(auto& point : way)newList.push_back(point);
                
                way.pop_back(); //revert origin
                way.push_back(back);
                break;
            }
            else
                way.push_back(back);
        }
    }

    if(newList.size() > 0)
        _ways.emplace_back(newList);
}
void SolutionWay::clear()
{
    _ways.clear();
    _ways.shrink_to_fit();
}
list<Vec2> SolutionWay::findWayMin(const Vec2& target) const
{
    vector<list<Vec2>> avaiable;
    for(auto& way : _ways)
    {
        if(way.back() == target)
            avaiable.push_back(way);
    }
    
    float length1 = 0.f;
    list<Vec2> result;
    for(auto& way : avaiable)
    {
        float length2 = 0.f;
        for(auto begin = way.begin(); begin != way.end();)
        {
            Vec2& point1 = *begin;
            ++begin;
			if (begin != way.end())
			{
				Vec2& point2 = *begin;
				length2 += (point1 - point2).length();
			}
        }
        
        if(length1==0.f)
        {
            length1 = length2;
            result = way;
        }
        else if(length2 < length1)
        {
            length1 = length2;
            result = way;
        }
    }
    return result;
}
int SolutionWay::size() const
{
    return (int)_ways.size();
}
const vector<list<Vec2>>& SolutionWay::getWays() const
{
    return _ways;
}

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


void InformationCenter::update(float dt)
{
//    return;
    auto player = Game::getInstance()->getPlayer();
    if(!player)return;  //Won
    if(player->isDestroyed())return;
    
    Vec2 playerPosition = player->_sprite->getPosition();
	int i = 0;
	for (auto begin = _listBot.begin(); begin != _listBot.end();)
	{
		BotFindWay& bot = *begin;
		if (bot.bot->isDestroyed())
		{
			bot.clear();
			begin = _listBot.erase(begin);
			continue;
		}
        
		//check player
        if(auto body = dynamic_pointer_cast<RigidBodyCircle>(bot.bot->_rigidBody))  
        {
            vector<Vec2> arrayFind {playerPosition};
            auto checkPlayerAround = findPointAvaiableAroud(bot.bot->_sprite->getPosition(), arrayFind, Vision::origin_vision * 0.75 + body->getRadius(), body->getRadius() / 2.f);
            if(checkPlayerAround.size() > 0)    //detect player in vision
            {
				if (bot.isFinish && bot.isThreadAvaiable)
				{
					//bot.task.get();
					//bot.isThreadAvaiable = false;
				}
				bot.isReady = false;
				bot.detectPlayer.isDetected = true;
				bot.detectPlayer.position = playerPosition;
				bot.detectPlayer.isCalled = false;
				bot.detectPlayer.isReceived = false;
                //notify for allbot
                for(auto& botF : _listBot)
                {
					if (botF.detectPlayer.isDetected)continue;
					if (!botF.detectPlayer.isCalled)
					{
						botF.detectPlayer.isCalled = true;
						botF.detectPlayer.position = playerPosition;
						bot.detectPlayer.isReceived = false;
						CCLOG("%d call to other", i);
					}
                }

				while (bot.commands.size() > 0)
					bot.commands.pop();
				bot.bot->releaseMoveCommands();
				

                bot.status = statusBot::SHOOT;
                float rotation = getRotateForwardAPoint(bot.bot, checkPlayerAround.front());
                bot.bot->_sprite->setRotation(rotation);
                if(bot.bot->getMag()->canShoot())
                {
                    Game::getInstance()->handleShootCharacter(bot.bot, bot.bot->getBullet()->getSpeed());
                }
            }	
			else if (bot.detectPlayer.isDetected && !bot.detectPlayer.isReceived)	//duoi theo player
			{
				//bot.detectPlayer.isDetected = false;
				bot.targetGo = bot.detectPlayer.position;
				bot.detectPlayer.isReceived = true;
				bot.isReady = true;
				//bot.isFinish = true;
				bot.status = statusBot::NONE;
				CCLOG("%d follow player", i);
				
			}
			else if (bot.detectPlayer.isCalled && !bot.detectPlayer.isReceived)	//di ra vi tri player bi phat hien khi dc nghe call tu team
			{
				//if ((bot.targetGo - bot.detectPlayer.position).length() > 64.f)
				{
					bot.targetGo = bot.detectPlayer.position;
					bot.isReady = true;
					//bot.isFinish = true;
					bot.status = statusBot::NONE;
					bot.bot->releaseMoveCommands();
					while (bot.commands.size() > 0)
						bot.commands.pop();
					CCLOG("%d Called", i);
					bot.detectPlayer.isReceived = true; 
				}
			}
			else if(bot.isFinish && !bot.isThreadAvaiable && !bot.detectPlayer.isCalled && !bot.detectPlayer.isDetected)	//khi ra vi tri va ko thay player
			{
				bot.detectPlayer.isDetected = false;
				bot.detectPlayer.isCalled = false;
				bot.detectPlayer.position = Vec2::ZERO;
				bot.detectPlayer.isReceived = false;
				bot.targetGo = Vec2::ZERO;
				if(bot.status != statusBot::WALK)bot.status = statusBot::NONE;
//                CCLOG("%d Not Found", i);
			}
        }
		
	

		if (bot.isFinish && bot.isReady)	//start thread
		{
#if DEBUG_GRAHP
            auto characterIsViewed = Game::getInstance()->getCurrentTargetView();
            if(&(*characterIsViewed) == &(*bot.bot))
            {
                _canMovePointDrawer->clear();
            }
#endif
			CCLOG("%d Find Way", i);
			Vec2 botPosition = bot.bot->_sprite->getPosition();
			bot.isFinish = false;
			bot.isReady = false;
			auto lamda = [&](Vec2 position, Vec2 target, BotFindWay* bf) -> pair<Vec2, queue<Vec2>>
			{
                bf->isThreadAvaiable = true;
				float radius = 0.f;
				if (bf)
				{
					if (auto body = dynamic_pointer_cast<RigidBodyCircle>(bf->bot->_rigidBody))
						radius = body->getRadius();
				}

                vector<Vec2> grahpAvaiable;
                for(auto p : _graph){grahpAvaiable.push_back(p);}
				grahpAvaiable.push_back(target);
				
				SolutionWay solutions;
				findWayToPoint(position, target, grahpAvaiable, solutions, radius);
                auto listPoints = solutions.findWayMin(target);
                queue<Vec2> way;
                for(auto p : listPoints)
                    way.push(p);
                
				if (bf)
				{
					if (bf->detectPlayer.isDetected)
						bf->detectPlayer.isDetected = false;
					if (bf->detectPlayer.isCalled)
						bf->detectPlayer.isCalled = false;

					std::lock_guard<mutex> gruard(_m);
					bf->isFinish = true;
					bf->isThreadAvaiable = true;
				}
				return pair<Vec2, queue<Vec2>>(target, way);
			};

            Vec2 target;
            if(!bot.detectPlayer.isCalled && !bot.detectPlayer.isDetected && bot.targetGo == Vec2::ZERO)
            {
                std::random_shuffle(_graph.begin(), _graph.end());
                target = *_graph.begin();
            }
            else
                target = bot.targetGo;
			//target = Game::getInstance()->getPlayer()->_sprite->getPosition();
            bot.task = std::async(launch::async, lamda, botPosition, target, &bot);
		}

		if (bot.isFinish && bot.isThreadAvaiable)
		{
			auto way = bot.task.get();
            bot.isThreadAvaiable = false;
			if (bot.status == statusBot::NONE)
			{
				
				//Move Bot
				if (way.second.size() == 0)
				{
					CCLOG("%d set Way size = 0", i);
					bot.isReady = true;
					if (way.first == bot.targetGo)
						bot.targetGo = Vec2::ZERO;
					bot.detectPlayer.isDetected = false;
					bot.detectPlayer.isCalled = false;
					bot.detectPlayer.isReceived = false;
					//bot.detectPlayer.position = Vec2::ZERO;
				}
				else
				{
					while (way.second.size() > 0)
					{
						shared_ptr<Command> cmd = CommandMoveTo::createCommandMoveTo(bot.bot->getSpeedMove(), way.second.front());
						bot.commands.push(cmd);
						way.second.pop();
					}
					CCLOG("%d set Way size > 0", i);
					bot.status = statusBot::WALK;
				}
				//bot.isReady = true;	//for test
			}
		}
		
		if (bot.status == statusBot::SHOOT)
		{
		}

		if (bot.status == statusBot::WALK)
		{
			std::lock_guard<mutex> guard(_m);
            
#if DEBUG_GRAHP
            auto characterIsViewed = Game::getInstance()->getCurrentTargetView();
            if(&(*characterIsViewed) == &(*bot.bot))
            {
//                _canMovePointDrawer->clear();
                auto queue = bot.commands;
                
                while (queue.size() > 1)
                {
                    if(auto moveTo = dynamic_pointer_cast<CommandMoveTo>(queue.front()))
                    {
                        Vec2 point1 = moveTo->getTarget();
                        queue.pop();
                        if(auto moveTo2 = dynamic_pointer_cast<CommandMoveTo>(queue.front()))
                        {
                            Vec2 point2 = moveTo2->getTarget();
                            _canMovePointDrawer->drawLine(point1, point2, Color4F::GREEN);
                        }
                    }
     
                }
            }
#endif

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
				bot.targetGo = Vec2::ZERO;
                bot.isReady = true;
				if(bot.detectPlayer.isCalled)
					bot.detectPlayer.isCalled = false;
			}
		}
		
		if (bot.status == statusBot::COLLISION)
		{
			bot.status = statusBot::NONE;
			//bot.isFinish = true;
            bot.isReady = true;
			bot.targetGo = Vec2::ZERO;
			while (bot.commands.size() > 0)
				bot.commands.pop();
			bot.bot->releaseMoveCommands();
		}
        
        

		bot.bot->update(dt);
		bot.countDetect.first += dt;
		++begin;
		++i;
	}
}

list<Vec2> InformationCenter::findPointAvaiableAroud(Vec2 position, const vector<Vec2>& arrayFind, float vision, float radius)
{
	vector<Line> lines;
	{
		//std::lock_guard<mutex> gruard(_m);
		lines = Game::getInstance()->getRigidWord()->getListLines();
	}

	radius *= 1.5f;
	list<Vec2> result;
	for (auto begin = arrayFind.begin(); begin != arrayFind.end(); ++begin)
	{
		auto& pointGrahp = *begin;
        if(pointGrahp == position)
            continue;

		if ((pointGrahp - position).length() > vision)
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

bool InformationCenter::findWayToPoint(Vec2 start, Vec2 target, vector<Vec2>& grahp, SolutionWay& result, float radius)
{
    if(grahp.size() == 0)return false;
	list<Vec2> around = findPointAvaiableAroud(start, grahp, 1000.f, radius);
	if (around.size() == 0)return false;

    for(auto& point : around)
    {
        result.push(start, point);
    }
    
    if(std::find(around.begin(), around.end(), target) != around.end())
        return true;
    
    for(auto begin = grahp.begin(); begin != grahp.end();)
    {
        Vec2 point = *begin;
        if(std::find(around.begin(), around.end(), point) != around.end() || point == start)
            begin = grahp.erase(begin);
        else
            ++begin;
    }
    
    bool findResult = false;
    
    while (true)
    {
        list<Vec2> unless;
		list<Vec2> tempAround;
		for (auto& p : around)tempAround.emplace_back(p);
        around.clear();
        for(auto& point : tempAround)
        {
            list<Vec2> aroundIn = findPointAvaiableAroud(point, grahp, 1000.f, radius);
            if(std::find(aroundIn.begin(), aroundIn.end(), target) != aroundIn.end())
            {
                result.push(point, target);
//                for(auto& destroyP : aroundIn)
//                    unless.push_back(destroyP);
//                unless.push_back(point);
                findResult = true;
            }
//            else

			if (findResult)
				return true;
            {
                for(auto& p : aroundIn)
                {
                    if(std::find(around.begin(), around.end(), p) == around.end())
                        around.push_back(p);
                    result.push(point, p);
                }
            }
                
            unless.push_back(point);
        }
        
        for(auto begin = grahp.begin(); begin != grahp.end();)
        {
            Vec2 point = *begin;
            if(std::find(unless.begin(), unless.end(), point) != unless.end())
                begin = grahp.erase(begin);
            else
                ++begin;
        }
        
        if(around.size() == 0)break;
    }

    return findResult;
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

InformationCenter::BotFindWay*  InformationCenter::findBotWayByBot(const shared_ptr<Character>& character)
{
	for (auto& bot : _listBot)
	{
		if (&(*bot.bot) == &(*character))
			return &bot;
	}
	return nullptr;
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

