#pragma once
#include "Command.h"

class CommandMoveBy : public Command
{
    float _counttime;
    float _duration;
    
     CC_SYNTHESIZE(Vec2, _velocity, Velocity);
    
public:
    CommandMoveBy();
    static shared_ptr<CommandMoveBy> createCommandMoveBy(const Vec2& vec, const float& duration);
    
    void update(float dt) override;
    
};
