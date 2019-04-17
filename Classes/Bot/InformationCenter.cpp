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
_isStop(false),
_enemyOutVision(nullptr, nullptr)
{
    
}

InformationCenter::~InformationCenter()
{
    clear();
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
                    Vec2 vectorAngle = target - object->_sprite->getPosition();
                    auto angle = atan2(vectorAngle.y, vectorAngle.x);
                    object->_sprite->setRotation(CC_RADIANS_TO_DEGREES(-angle) + 90);
                    
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
    while (!_isStop)
    {
        for(auto movearound : _enemyMoveAround)
        {
            
        }
    }
}

void InformationCenter::pushInformation(const shared_ptr<Character>& character, shared_ptr<InformationDetectEnemy> information)
{
    _enemyIsDetected.push(pair<shared_ptr<Character>,shared_ptr<InformationDetectEnemy>>(character, information));
}

void InformationCenter::pushInformation(const shared_ptr<Character>& character, shared_ptr<InformationEnemyOutVision> information)
{
    _enemyOutVision = pair<shared_ptr<Character>, shared_ptr<InformationEnemyOutVision>>(character, information);
}

void InformationCenter::pushInformation(const shared_ptr<Character>& character, shared_ptr<InformationMoveAround> information)
{
    _enemyMoveAround.push_back(pair<shared_ptr<Character>, shared_ptr<InformationMoveAround>>(character, information));
}

void InformationCenter::start()
{
    _isStop = false;
    
    detectEnemy = thread(&InformationCenter::triggerDetectEnemy, this);
    detectEnemy.detach();
    
    enemyOutVision = thread(&InformationCenter::triggerEnemyOutVision, this);
    enemyOutVision.detach();
    
    enemyMoveAround = thread(&InformationCenter::triggerEnemyMoveAround, this);
    enemyMoveAround.detach();
}

void InformationCenter::stop()
{
    _isStop = true;
    
}

void InformationCenter::clear()
{
    while(_enemyIsDetected.size() > 0)
        _enemyIsDetected.pop();
    
    _enemyOutVision.first = nullptr;
    _enemyOutVision.second = nullptr;
    
    _enemyMoveAround.clear();
}
