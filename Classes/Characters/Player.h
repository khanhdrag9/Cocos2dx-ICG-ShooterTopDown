#pragma once
#include "Character.h"

class Player : public Character
{
public:
    Player();
    
    void init() override;
    void update(float dt) override;
    void pushCommand(shared_ptr<Command>& command, bool replace = false) override;
    
    
    CC_SYNTHESIZE(float, _speedMove, SpeedMove);
};
