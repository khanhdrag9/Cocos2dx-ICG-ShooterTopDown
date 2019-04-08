#pragma once
#include "../Characters/Character.h"

class LinkPosition;

class Bot : public Character
{
public:
    enum Status : int
    {
        STOP,
        WALK,
        SHOOT
    };
    
private:
    
	shared_ptr<LinkPosition> _linkPos;
    list<Status> _currentStatus;
    
	bool _ableWalk;
	bool _ableShoot;
	pair<float, float> _countShoot;
public:
	Bot();
    
    void init() override;
    void update(float dt) override;
    void pushCommand(shared_ptr<Command>& command) override;
	void setLinkPosition(shared_ptr<LinkPosition> newlink);
	shared_ptr<LinkPosition> getLinkPosition() const;
    
    void setStatus(Status status);
    list<Status>& getStatus();
    
    CC_SYNTHESIZE(float, _speedMove, SpeedMove);

	bool containStatus(Status status);

	bool isCanTriggerWalk();
	bool isCanTriggerShoot();

	void setWalk(bool enable);
	void setShoot(bool enable);
	
};
