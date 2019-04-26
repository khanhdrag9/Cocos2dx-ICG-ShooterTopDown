#pragma once
#include "Command.h"

class CommandMoveTo : public Command
{
    float _speed;
    Vec2 _velocity;
    Vec2 _target;
    
public:
    CommandMoveTo();
    static shared_ptr<CommandMoveTo> createCommandMoveTo(const float& speed, const Vec2& toPoint);
    
    void update(float dt) override;
    
	Vec2 getTarget() const;
};

