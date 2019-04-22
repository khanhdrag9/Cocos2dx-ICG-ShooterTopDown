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
//Description
description_type description::getType() const
{
    return _type;
}

des_init::des_init() { _type = description_type::init; }

des_walk::des_walk(const Vec2& velocity) : _vec(velocity) { _type = description_type::walk; }
des_walk::~des_walk() {
}

des_detect_new_road::des_detect_new_road(const Vec2& direct) : _direct(direct) { _type = description_type::detect_new_road; }

des_run::des_run(const Vec2& point) : _point(point) { _type = description_type::run; }

des_detect_collision_wall::des_detect_collision_wall(const Vec2 & currentVec)
	: _currentVec(currentVec)
{
	_type = description_type::detect_collision_wall;
}

des_collision_wall::des_collision_wall(const Vec2& currentVec, const Vec2& collisionPoint) : 
	_collisionPoint(collisionPoint)
{
	_currentVec = currentVec;
	_type = description_type::collision_wall;
}

//information
const int InformationMoveAround::limit_description = 100;

InformationMoveAround::InformationMoveAround()
{
    _type = InformationType::MoveAround;
}

InformationMoveAround::InformationMoveAround(shared_ptr<description> des)
{
    _type = InformationType::MoveAround;
    _descriptions.push_back(des);
}

InformationMoveAround::~InformationMoveAround()
{
	/*while (_descriptions.size() > 0)
	{
		_descriptions.pop();
	}*/
	_descriptions.clear();
}

void InformationMoveAround::add(shared_ptr<InformationMoveAround> newInfo)
{
    //auto queueNew = newInfo->_descriptions;
    while (newInfo->_descriptions.size() > 0)
    {
        _descriptions.push_back(newInfo->_descriptions.front());
		newInfo->_descriptions.pop_front();
		if(_descriptions.size() > limit_description)
			_descriptions.pop_front();
    }
    
    //limit
    /*while (_descriptions.size() > limit_description)
    {
        _descriptions.pop();
    }*/
}
