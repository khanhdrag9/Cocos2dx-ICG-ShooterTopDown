#pragma once
#include "../Headers.h"

class Character;

class RigidBody
{
public:
    enum type : int
    {
        DYNAMIC,
        STATIC,
        KINEMATIC
    };
    
public:
    RigidBody();
    virtual ~RigidBody();
    
    virtual void update(float dt);
    
    shared_ptr<Character> getObject() const;
    void setType(const RigidBody::type& type);
    RigidBody::type getType() const;
    
    void setVelocity(const Vec2& vec);
    Vec2 getVelocity() const;
    
protected:
    virtual void init();
    
    b2Body* _body;
    shared_ptr<Character> _object;
};
