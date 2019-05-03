#pragma once
#include "../Headers.h"

class GS_LoadToGamePlay : public Layer
{
    bool _loadfinish;
    float _countTime;
    //thread _loadGame;
    
    Scene* _gameplay;
    void loadGame();
public:
    GS_LoadToGamePlay();
    static Scene* createScene();
    
    bool init() override;
    void update(float dt) override;
    
    CREATE_FUNC(GS_LoadToGamePlay);
};
