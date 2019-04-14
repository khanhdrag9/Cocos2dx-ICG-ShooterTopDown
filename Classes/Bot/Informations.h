#pragma once
#include "../Headers.h"

class Character;


//base
class Information : public enable_shared_from_this<Information>
{
public:
    enum InformationType
    {
        None,
        DetectEnemy,
        EnemyOutVision,
        MoveAround
    };
    
protected:
    InformationType _type;
    
public:
    Information();
    virtual ~Information();
    InformationType getType() const;
};

//lop con
class InformationDetectEnemy : public Information
{
protected:
    weak_ptr<Character> _enemy;
    float _distance;
    
public:
    InformationDetectEnemy();
    InformationDetectEnemy(const shared_ptr<Character>& enemy, float distance);
    
    friend class InformationCenter;
};

class InformationEnemyOutVision : public Information
{
public:
    InformationEnemyOutVision();
    
    friend class InformationCenter;
};

class InformationMoveAround : public Information
{
    vector<Vec2> _aroundCollision;
    vector<Vec2> _aroundNotCollision;
public:
    InformationMoveAround();
    InformationMoveAround(const vector<Vec2>& aroundCollision, const vector<Vec2>& aroundNotCollision);
    InformationMoveAround(shared_ptr<Character> object, const vector<Vec2>& around);
    ~InformationMoveAround();
    
    friend class InformationCenter;
};