#pragma once
#include "../Headers.h"

class GS_GamePlay : public Layer
{
public:
    static Scene* createScene();
    
    
private:
    bool init() override;
    void update(float dt) override;
    void controller();
    
    Sprite* _aim;
    
    CREATE_FUNC(GS_GamePlay);
};

