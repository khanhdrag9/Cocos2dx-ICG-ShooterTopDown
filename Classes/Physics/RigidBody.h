#pragma once
#include "../Headers.h"

class Character;

class RigidBody
{
public:
    friend class RigidWorld;
    
    
    enum type : int
    {
        DYNAMIC,
        STATIC,
        KINEMATIC
    };
    
    enum tag : int
    {
        PLAYER,
        ENEMY,
        WALL,
        BULLET,
        BULLET_PLAYER = PLAYER,
        BULLET_ENEMY = ENEMY
    };
    
    Vec2 _velocity;
public:
    RigidBody();
    virtual ~RigidBody();
    
    virtual void update(float dt);
    
    shared_ptr<Character> getObject() const;
    void setType(const RigidBody::type& type);
    RigidBody::type getType() const;
    void setTag(int tag);
    int getTag() const;
    
protected:
    virtual void init();
    
    RigidBody::type _type;
    shared_ptr<Character> _object;
    int _tag;
};
