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

InformationCenter::InformationCenter():
_isStop(false)
{
    thread t(&InformationCenter::update, InformationCenter::getInstance());
    t.detach();
}

InformationCenter::~InformationCenter()
{
    clear();
}

void InformationCenter::update()
{
    while (!_isStop)
    {
        if(_queueInformation.size() > 0)
        {
            auto informationCharacter = _queueInformation.front();
            
            if(!informationCharacter.object.expired())
            {
                auto object = informationCharacter.object.lock();
                auto information = informationCharacter.information;
                
                if(object)
                {
                    //Detect enemy
                    if(information->getType() == Information::InformationType::DetectEnemy)
                    {
                        if(auto detectenemy = dynamic_pointer_cast<InformationDetectEnemy>(information))
                        {
                            auto enemy = detectenemy->_enemy.lock();
                            if(enemy)
                            {
                                Vec2 target = enemy->_sprite->getPosition();
                                Vec2 vectorAngle = target - object->_sprite->getPosition();
                                auto angle = atan2(vectorAngle.y, vectorAngle.x);
                                object->_sprite->setRotation(CC_RADIANS_TO_DEGREES(-angle) + 90);
                                
                                if(auto bot = dynamic_pointer_cast<Bot>(object))
                                    bot->setShoot(true);
                            }
                        }
                    }
                    else if(information->getType() == Information::InformationType::EnemyOutVision)
                    {
                        if(auto bot = dynamic_pointer_cast<Bot>(object))
                            bot->setShoot(false);
                    }
                    
                    
                    //other
                    
                }
            }
            
            _queueInformation.pop();
        }
    }
}

void InformationCenter::pushInformation(const shared_ptr<Character>& character, shared_ptr<Information> information)
{
    _queueInformation.push(InformationCharacter(character, information));
}

void InformationCenter::stop()
{
    _isStop = true;
}

void InformationCenter::clear()
{
    while(_queueInformation.size() > 0)
    {
        _queueInformation.pop();
    }
}
