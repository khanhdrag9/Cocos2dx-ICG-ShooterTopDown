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
                    auto typeInformation = information->getType();
                    bool isBotShoot = false;
                    if(typeInformation == Information::InformationType::DetectEnemy)
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
                                {
                                    isBotShoot = true;
                                    bot->setShoot(true);
                                }
                            }
                        }
                    }
                    else if(typeInformation == Information::InformationType::EnemyOutVision)
                    {
                        if(auto bot = dynamic_pointer_cast<Bot>(object))
                            bot->setShoot(false);
                    }
                    
                    
                    //other
                    if(typeInformation == Information::InformationType::MoveAround)
                    {
                        if(auto movearound = dynamic_pointer_cast<InformationMoveAround>(information))
                        {
                            auto& pointsCollision = movearound->_aroundCollision;
                            auto& pointsNotCollision = movearound->_aroundNotCollision;
                            Vec2 objPosition = object->_sprite->getPosition();
                            //caculator...
                            
                            
                            auto max = std::max_element(pointsNotCollision.begin(), pointsNotCollision.end(), [&](Vec2 point1, Vec2 point2){
                                return (objPosition - point1).length() > (objPosition - point2).length();
                            });
                            if(max != pointsNotCollision.end())
                            {
                                Vec2 direction = *max - object->_sprite->getPosition();
                                direction.normalize();
                                direction *= 100;
                                if(auto bot = dynamic_pointer_cast<Bot>(object))
                                {
                                    bot->setWalk(true);
                                    shared_ptr<Command> cmd = CommandMoveBy::createCommandMoveBy(direction, 0.1);
                                    bot->Character::pushCommand(cmd);
                                }
                            }
                        }
                        
                    }
                    
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
