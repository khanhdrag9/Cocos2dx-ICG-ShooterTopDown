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

//Description
enum class description_type : int
{
	none,
    init,
    walk,
    run,
    detect_collision_wall,
    collision_wall
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
    inline Vec2 getVelocity() const;
};

inline Vec2 des_walk::getVelocity() const { return _vec; }

class des_run : public description
{
    Vec2 _point;
public:
    des_run(const Vec2& point);
	inline Vec2 getVelocity() const;
};

inline Vec2 des_run::getVelocity() const { return _point; }

class des_detect_collision_wall : public description
{
protected:
	Vec2 _currentVec;
public:
	des_detect_collision_wall() {}
	des_detect_collision_wall(const Vec2& rotation);
	inline Vec2 getCurrentVec() const;
};

inline Vec2 des_detect_collision_wall::getCurrentVec() const { return _currentVec; }

class des_collision_wall : public des_detect_collision_wall
{
	Vec2 _collisionPoint;
public:
	des_collision_wall(const Vec2& currentVec, const Vec2& collisionPoint);	//collision point havent used 
	inline Vec2 getCollisionPoint() const;
};

inline Vec2 des_collision_wall::getCollisionPoint() const { return _collisionPoint; }

//Information
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
