#pragma once
#include "../Headers.h"

class GS_GameMenu : public Layer
{
public:
    static Scene* createScene();
    
    
private:
    bool init() override;
    
    void GoToGamePlay();
    void GoToOption();

    CREATE_FUNC(GS_GameMenu);

};

