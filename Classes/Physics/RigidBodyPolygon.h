#pragma once
#include "RigidBody.h"

class RigidBodyPolygon : public RigidBody
{
    void init() override;
    
public:
    static unique_ptr<RigidBodyPolygon> createRigidBodyPolygon(shared_ptr<Character> object, const vector<Vec2>& vertices);
    
    void update(float dt) override;
};
