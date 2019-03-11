#pragma once
#include "Command.h"


class CommandShoot : public Command
{
    Vec2 _speed;
public:
    CommandShoot();
    static shared_ptr<CommandShoot> createCommandShoot(const Vec2& speed);
    
    void update(float dt) override;
    
};
