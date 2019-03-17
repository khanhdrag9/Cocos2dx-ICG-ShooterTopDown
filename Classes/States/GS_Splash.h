#pragma once
#include "cocos2d.h"

USING_NS_CC;

class GS_Splash : public Layer
{
public:
    static Scene* createScene();
    
    
    CREATE_FUNC(GS_Splash);
private:
    bool init() override;
    void update(float dt) override;
    void changeToGameMenu(float time);
    
    bool _isInit;
};
