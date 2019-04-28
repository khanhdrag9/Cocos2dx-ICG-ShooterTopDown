#pragma once
#include "../Headers.h"

class GS_OptionPage;
class GS_PickMap;

class GS_GameMenu : public Layer
{
public:
    GS_GameMenu();
    static Scene* createScene();
    
private:
    bool init() override;
    
    void GoToPickMap();
private:

    CREATE_FUNC(GS_GameMenu);

    GS_OptionPage* _optionPage;
    GS_PickMap* _pickMapPage;
};

