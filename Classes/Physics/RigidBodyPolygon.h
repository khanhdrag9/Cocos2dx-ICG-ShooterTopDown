#pragma once
#include "RigidBody.h"

class RigidBodyPolygon : public RigidBody
{
    void init() override;
    
    Rect _rect;
public:
    static shared_ptr<RigidBodyPolygon> createRigidBodyPolygon(shared_ptr<Character> object);
    
    void update(float dt) override;
    
    
    friend class RigidWorld;
};
