#pragma once
#include "../Headers.h"

class GS_OptionPage : public Layer
{
    ui::Button* _option;
    ui::Button* _volumn;
    Sprite* _disbleVolumn;
    ui::Button* _about;
    ui::Button* _back;
    Color3B _colorUI;
private:
    bool init() override;
    
    void GoToOption();
    void modifyVolumn();
    void GoToAbout();
    void Back();
public:
    CREATE_FUNC(GS_OptionPage);
    void setColorUI(Color3B color);
};
