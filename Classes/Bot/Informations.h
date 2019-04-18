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

/*--------------------------*/
class InformationEnemyOutVision : public Information
{
public:
    InformationEnemyOutVision();
    
    friend class InformationCenter;
};

/*--------------------------*/

enum class description_type : int
{
    init,
    walk,
    run,
    detect_collision,
    collision
};

class description
{
protected:
    description_type _type;
public:
    description(){}
    virtual ~description() = default;
    description_type getType() const;
};

class des_init : public description
{
public:
    des_init();
};

class des_walk : public description
{
    Vec2 _vec;
public:
    des_walk(const Vec2& velocity);
    Vec2 getVelocity() const;
};

class des_run : public description
{
    Vec2 _point;
public:
    des_run(const Vec2& point);
    Vec2 getVelocity() const;
};


class InformationMoveAround : public Information
{
protected:
    queue<shared_ptr<description>> _descriptions;
    
public:
    InformationMoveAround();
    InformationMoveAround(shared_ptr<description> des);
    ~InformationMoveAround();
    
    void add(shared_ptr<InformationMoveAround> newInfo);
    
    friend class InformationCenter;
    
    const static int limit_description;
};
