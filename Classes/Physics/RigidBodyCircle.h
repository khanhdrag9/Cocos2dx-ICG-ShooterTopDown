#pragma once
#include "RigidBody.h"

class RigidBodyCircle : public RigidBody
{
    void init() override;
    
    float _radius;
public:
    static shared_ptr<RigidBodyCircle> createRigidBodyCircle(shared_ptr<Character> object);
    
    void update(float dt) override;
    const float& getRadius() const;
    
    friend class RigidWorld;
};
