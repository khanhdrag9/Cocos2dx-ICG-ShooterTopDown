#pragma once
#include "../Headers.h"

class Character;
class GS_OptionPage;
class Joystick;

class GS_GamePlayUI : public Layer
{
    Label* _playerBullet;
    Label* _propertiesPlayer;

//    Sprite* _kdaTab;
//    Menu* _menuKDA;
//    Vector<MenuItem*> _listKDA;
    
    shared_ptr<Character> _characterProfile;
    vector<ui::Button*> _skillsCharacter;
    
    CC_SYNTHESIZE_READONLY(GS_OptionPage*, _optionPage, OptionPage);
public:
    GS_GamePlayUI();
    ~GS_GamePlayUI();
    
    bool init() override;
    void update(float) override;
    
    void setCharacter(const shared_ptr<Character>& character);
    void initKDA(const int& number);
    void useTab(bool show);
    void showResult();
    
    void clear();
    CREATE_FUNC(GS_GamePlayUI);
    
#if USE_JOYSTICK
    CC_SYNTHESIZE_READONLY(Joystick*, _joystick, Joystick);
#endif
};

