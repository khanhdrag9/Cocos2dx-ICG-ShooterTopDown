#pragma once
#include "../Headers.h"

class Character;

class GS_GamePlayUI : public Layer
{
    Label* _playerBullet;
    Label* _propertiesPlayer;
    
    shared_ptr<Character> _characterProfile;
public:
    GS_GamePlayUI();
    
    bool init() override;
    void update(float) override;
    
    void setCharacter(const shared_ptr<Character>& character);
    
    CREATE_FUNC(GS_GamePlayUI);
};

