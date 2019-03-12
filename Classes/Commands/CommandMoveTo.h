#pragma once
#include "Command.h"

class CommandMoveTo : public Command
{
    float _speed;
    Vec2 _velocity;
    Vec2 _target;
    
public:
    CommandMoveTo();
    static shared_ptr<CommandMoveTo> createCommandMoveBy(const float& speed, const Vec2& toPoint);
    
    void update(float dt) override;
    
};

