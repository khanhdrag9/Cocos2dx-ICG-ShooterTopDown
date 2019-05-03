#pragma once
#include "../Characters/Character.h"
#include "../Objects/Mag.h"

class LinkPosition;

class Bot : public Character
{
private:
    unique_ptr<Mag> _bulletMag;
public:
	Bot();
    
    void init(CharacterCreation* creation) override;
    void update(float dt) override;
    bool pushCommand(shared_ptr<Command>& command, bool replace = false) override;
    
    
    CC_SYNTHESIZE(float, _speedMove, SpeedMove);
    const unique_ptr<Mag>& getMag() const;
};
