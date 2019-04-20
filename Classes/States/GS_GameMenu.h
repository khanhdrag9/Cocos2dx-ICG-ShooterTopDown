#pragma once
#include "../Headers.h"


class GS_GameMenu : public Layer
{
public:
    static Scene* createScene();
    
    
private:
    bool init() override;
    
    void GoToGamePlay();
    void GoToPickMap();
    void GoToOption();
    void modifyVolumn();
    void GoToAbout();
    
private:
    ui::Button* _option;
    ui::Button* _volumn;
    Sprite* _disbleVolumn;
    ui::Button* _about;

    CREATE_FUNC(GS_GameMenu);

};

