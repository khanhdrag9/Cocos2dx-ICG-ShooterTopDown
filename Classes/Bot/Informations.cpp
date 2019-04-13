//
//  Informations.cpp
//  ShooterTopDown2D
//
//  Created by Khanh on 4/13/19.
//

#include "Informations.h"
#include "../Characters/Character.h"

//Information
Information::Information()
{
}

Information::~Information()
{
}

Information::InformationType Information::getType() const
{
    return _type;
}

//Detect Enemy
InformationDetectEnemy::InformationDetectEnemy():
_distance(0.f)
{
    _type = InformationType::DetectEnemy;
}

InformationDetectEnemy::InformationDetectEnemy(const shared_ptr<Character>& enemy, float distance):
_distance(distance)
{
    _type = InformationType::DetectEnemy;
    _enemy = enemy;
}

//Enemy Out Vision
InformationEnemyOutVision::InformationEnemyOutVision()
{
    _type = InformationType::EnemyOutVision;
}

//Move Around
InformationMoveAround::InformationMoveAround()
{
    _type = InformationType::MoveAround;
}

InformationMoveAround::InformationMoveAround(const vector<Vec2>& aroundCollision, const vector<Vec2>& aroundNotCollision):
_aroundCollision(aroundCollision),
_aroundNotCollision(aroundNotCollision)
{
    _type = InformationType::MoveAround;
}

InformationMoveAround::InformationMoveAround(shared_ptr<Character> object, const vector<Vec2>& around)
{
    _type = InformationType::MoveAround;
    Vec2 objPos = object->_sprite->getPosition();
    for (auto point : around)
    {
        float lenght = (point - objPos).length();
        if(lenght >=  500.f)    //500.f is dimention of vision - is coded in Vision.cpp
            _aroundNotCollision.push_back(point);
        else
            _aroundCollision.push_back(point);
    }
}

InformationMoveAround::~InformationMoveAround()
{
    _aroundCollision.clear();
    _aroundNotCollision.clear();
}
