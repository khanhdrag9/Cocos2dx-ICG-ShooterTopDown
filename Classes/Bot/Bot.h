#pragma once
#include "../Characters/Character.h"

class Bot : public Character
{
public:
	Bot();
    
    void init() override;
    void update(float dt) override;
    void pushCommand(shared_ptr<Command>& command) override;
    
    
    CC_SYNTHESIZE(float, _speedMove, SpeedMove);
};
