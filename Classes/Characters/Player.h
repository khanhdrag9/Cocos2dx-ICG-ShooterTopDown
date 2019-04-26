#pragma once
#include "Character.h"
#include "../Objects/Mag.h"

class Player : public Character
{
    unique_ptr<Mag> _bulletMag;
    
public:
    Player();
    ~Player();
    
    void init() override;
    void update(float dt) override;
    bool pushCommand(shared_ptr<Command>& command, bool replace = false) override;
    const unique_ptr<Mag>& getMag() const;
    
    CC_SYNTHESIZE(float, _speedMove, SpeedMove);
};
