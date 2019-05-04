#pragma once
#include "../Headers.h"

class GS_GamePlayUI;
class GS_OptionPage;
class Joystick;

class GS_GamePlay : public Layer
{
public:
	GS_GamePlay();
	~GS_GamePlay();

    static Scene* createScene();
    
    GS_GamePlayUI* getUILayer() const;
private:
    bool init() override;
    void update(float dt) override;
    void controller();
    
    GS_GamePlayUI* _uilayer;
    
    
    CREATE_FUNC(GS_GamePlay);
};

