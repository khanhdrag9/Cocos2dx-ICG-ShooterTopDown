#pragma once
#include "../Characters/Character.h"
#include "../Objects/Mag.h"

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
    
    list<Status> _currentStatus;
    
	bool _ableWalk;
	bool _ableShoot;
    unique_ptr<Mag> _bulletMag;
public:
	Bot();
    
    void init() override;
    void update(float dt) override;
    void pushCommand(shared_ptr<Command>& command, bool replace = false) override;
    
    void setStatus(Status status);
    list<Status>& getStatus();
    
    CC_SYNTHESIZE(float, _speedMove, SpeedMove);

	bool containStatus(Status status);

	bool isCanTriggerWalk();
	bool isCanTriggerShoot();

	void setWalk(bool enable);
	void setShoot(bool enable);
	
    
    const unique_ptr<Mag>& getMag() const;
};
