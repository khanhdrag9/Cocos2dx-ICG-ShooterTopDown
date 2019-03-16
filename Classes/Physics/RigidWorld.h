#pragma once
#include "../Headers.h"

class RigidBody;
class RigidBodyPolygon;
class Character;
class BulletBasic;

class RigidWorld
{
    vector<shared_ptr<RigidBody>> _listRigidBodies;
    
    bool checkCollisionOther(shared_ptr<RigidBody> body);
public:
    RigidWorld();
    virtual ~RigidWorld();
    
    void update(float dt);
    
    shared_ptr<RigidBodyPolygon> createRigidBodyPolygon(const Rect& rect);
    shared_ptr<RigidBody> createRigidBodyPolygon(shared_ptr<Character> character);
    //shared_ptr<RigidBody> createRigidBodyPolygon(shared_ptr<BulletBasic> bullet);
    
};
