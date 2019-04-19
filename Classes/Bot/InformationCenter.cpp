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



InformationCenter::InformationCenter():
_isStop(true),
_enemyOutVision(nullptr, nullptr)
{
    
}

InformationCenter::~InformationCenter()
{
    clear(true);
}

void InformationCenter::triggerDetectEnemy()
{
    while (!_isStop)
    {
        if(_enemyIsDetected.size() > 0)
        {
            auto& pairInfo = _enemyIsDetected.front();
            auto information = pairInfo.second;
            auto object = pairInfo.first;
            auto enemy = information->_enemy.lock();
            
            if(enemy)
            {
                if(enemy)
                {
                    Vec2 target = enemy->_sprite->getPosition();
//                    Vec2 vectorAngle = target - object->_sprite->getPosition();
//                    auto angle = atan2(vectorAngle.y, vectorAngle.x);
//                    object->_sprite->setRotation(CC_RADIANS_TO_DEGREES(-angle) + 90);
                    object->_sprite->setRotation(getRotateForwardAPoint(object, target));
                    
                    if(auto bot = dynamic_pointer_cast<Bot>(object))
                    {
                        bot->setShoot(true);
                    }
                }
            }
            
            _enemyIsDetected.pop();
        }
    }
}

float InformationCenter::getRotateForwardAPoint(shared_ptr<Character> character ,const Vec2& point)
{
    Vec2 vectorAngle = point - character->_sprite->getPosition();
    float angle = atan2(vectorAngle.y, vectorAngle.x);
    return CC_RADIANS_TO_DEGREES(-angle) + 90;
}

void InformationCenter::triggerEnemyOutVision()
{
    while (!_isStop)
    {
        if(_enemyOutVision.first && _enemyOutVision.second)
        {
            if(auto bot = dynamic_pointer_cast<Bot>(_enemyOutVision.first))
                bot->setShoot(false);
            
            _enemyOutVision.first = nullptr;
            _enemyOutVision.second = nullptr;
        }
    }
}

void InformationCenter::triggerEnemyMoveAround()
{
    //while (!_isStop)
    {
        for(auto& index : _enemyMoveAround)
        {
            auto information = index.second;
            auto backDes = information->_descriptions.back();
            
            switch (backDes->getType())
            {
                case description_type::init:
                    
                    break;
                case description_type::walk:
                    if(auto walk = dynamic_pointer_cast<des_walk>(backDes))
                    {
                        if(auto bot = dynamic_pointer_cast<Bot>(index.first))
                        {
                            float speed = bot->getSpeedMove();
                            float minSpeed = speed / 4.f;
                            float maxSpeed = speed * 4.f;
                            Vec2 velocity = walk->getVelocity();
                            if(!speedAvaiable(velocity, minSpeed, maxSpeed))
                            {
                                Vec2 previousPos = Vec2::ZERO;
                                auto temp = information->_descriptions;
                                while (temp.size() > 0)
                                {
                                    if(auto walk = dynamic_pointer_cast<des_walk>(temp.front()))
                                    {
                                        Vec2 walVelocity = walk->getVelocity();
                                        if(speedAvaiable(walVelocity, minSpeed, maxSpeed))
                                            previousPos = walk->getVelocity();
                                    }
                                    
                                    temp.pop();
                                }
                                
                                if(!speedAvaiable(previousPos, minSpeed, maxSpeed))
                                {
                                    previousPos = getRandomMove(speed);
                                }
                                
                                velocity = previousPos;
                            }
                        
                            moveWithVelocity(index, velocity);
                            //rotate forward direction movement
                            Vec2 target = velocity + bot->_sprite->getPosition();
                            bot->_sprite->setRotation(getRotateForwardAPoint(bot, target));
                        }
                    }
                    
                    break;
                case description_type::run:
                    break;
                case description_type::detect_collision_wall:
					if (auto dcw = dynamic_pointer_cast<des_detect_collision_wall>(backDes))
					{
						if (auto bot = dynamic_pointer_cast<Bot>(index.first))
						{
							float speed = bot->getSpeedMove();
							Vec2 currentVec = dcw->getCurrentVec();
							Vec2 velocity = Vec2::ZERO;
							do
							{
								velocity = getRandomMove(speed);
							} while (velocity == currentVec);
							moveWithVelocity(index, velocity);
						}
					}
                    
                    break;
                case description_type::collision_wall:
					if (auto cw = dynamic_pointer_cast<des_collision_wall>(backDes))
					{
						if (auto bot = dynamic_pointer_cast<Bot>(index.first))
						{
							float speed = bot->getSpeedMove();
							Vec2 currentVec = cw->getCurrentVec();
							Vec2 velocity = Vec2::ZERO;
							do
							{
								velocity = getRandomMove(speed);
							} while (velocity == currentVec);
							moveWithVelocity(index, velocity);
						}
					}

                    break;
                default:
                    break;
            }
            
        }
    }
}

inline bool InformationCenter::speedAvaiable(const Vec2& speedCheck, const float& min, const float& max) const
{
    float x = abs(speedCheck.x);
    float y = abs(speedCheck.y);
    return (speedCheck != Vec2::ZERO && ((x > min && x < max) || (y > min && y < max)));
}

Vec2 InformationCenter::getRandomMove(float speed) const
{
    Vec2 randVec[] = {
        Vec2(0, speed),
        Vec2(0, -speed),
        Vec2(speed, 0),
        Vec2(-speed, 0)
    };
    Vec2 move = randVec[random(0, 3)];
    return move;
}

void InformationCenter::moveWithVelocity(pairCharacterMove& pair, const Vec2& velocity)
{
    shared_ptr<description> des = make_shared<des_walk>(velocity);
    pair.second->add(make_shared<InformationMoveAround>(des));
    
    shared_ptr<Command> cmd = CommandMoveBy::createCommandMoveBy(velocity, 0.1);
    pair.first->pushCommand(cmd);
}

void InformationCenter::pushInformation(const shared_ptr<Character>& character, shared_ptr<InformationDetectEnemy> information)
{
    _enemyIsDetected.push(pair<shared_ptr<Character>,shared_ptr<InformationDetectEnemy>>(character, information));
	triggerEnemyMoveAround();
}

void InformationCenter::pushInformation(const shared_ptr<Character>& character, shared_ptr<InformationEnemyOutVision> information)
{
    _enemyOutVision = pair<shared_ptr<Character>, shared_ptr<InformationEnemyOutVision>>(character, information);
	triggerEnemyMoveAround();
}

void InformationCenter::pushInformation(const shared_ptr<Character>& character, shared_ptr<InformationMoveAround> information)
{
    bool _isContain = false;
    pairCharacterMove _contain;
    
    for(auto& o : _enemyMoveAround)
    {
        if(&(*o.first) == &(*character))
        {
            _isContain = true;
            _contain = o;
            break;
        }
    }
    
    if(_isContain)
    {
        _contain.second->add(information);
    }
    else
        _enemyMoveAround.push_back(pair<shared_ptr<Character>, shared_ptr<InformationMoveAround>>(character, information));

	triggerEnemyMoveAround();
}

void InformationCenter::start()
{
    _isStop = false;
    
    detectEnemy = thread(&InformationCenter::triggerDetectEnemy, this);
    //detectEnemy.detach();
    
    enemyOutVision = thread(&InformationCenter::triggerEnemyOutVision, this);
//    enemyOutVision.detach();
    
    //enemyMoveAround = thread(&InformationCenter::triggerEnemyMoveAround, this);
//    enemyMoveAround.detach();
}

void InformationCenter::update()
{
    if(_isStop)return;
    
    //triggerEnemyMoveAround();
}

void InformationCenter::stop()
{
    _isStop = true;
    
    detectEnemy.join();
    enemyOutVision.join();
        
    //enemyMoveAround.join();
}

void InformationCenter::clear(bool full)
{
    while(_enemyIsDetected.size() > 0)
        _enemyIsDetected.pop();
    
    _enemyOutVision.first = nullptr;
    _enemyOutVision.second = nullptr;
    
    if(full)_enemyMoveAround.clear();
}
