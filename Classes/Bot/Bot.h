#pragma once
#include "../Characters/Character.h"

class LinkPosition;

class Bot : public Character
{
	shared_ptr<LinkPosition> _linkPos;
public:
	Bot();
    
    void init() override;
    void update(float dt) override;
    void pushCommand(shared_ptr<Command>& command) override;
	void setLinkPosition(shared_ptr<LinkPosition> newlink);
	shared_ptr<LinkPosition> getLinkPosition() const;
    
    CC_SYNTHESIZE(float, _speedMove, SpeedMove);
};
