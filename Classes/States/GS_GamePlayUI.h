#pragma once
#include "../Headers.h"

class Character;

class GS_GamePlayUI : public Layer
{
    Label* _playerBullet;
    Label* _propertiesPlayer;
    
    Sprite* _kdaTab;
    Menu* _menuKDA;
    Vector<MenuItem*> _listKDA;
    
    shared_ptr<Character> _characterProfile;
public:
    GS_GamePlayUI();
    ~GS_GamePlayUI();
    
    bool init() override;
    void update(float) override;
    
    void setCharacter(const shared_ptr<Character>& character);
    void initKDA(const int& number);
    void useKDATab(bool show);
    
    CREATE_FUNC(GS_GamePlayUI);
};

