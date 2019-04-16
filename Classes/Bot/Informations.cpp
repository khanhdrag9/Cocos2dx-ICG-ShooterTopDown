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

InformationMoveAround::~InformationMoveAround()
{
    
}
