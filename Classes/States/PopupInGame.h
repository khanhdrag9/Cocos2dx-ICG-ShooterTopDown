#pragma once
#include "../Headers.h"

class Character;


class PopupInGame : public Layer
{
    
public:
    PopupInGame();
    bool init() override;
    void setText(const string& text);
    
    //CREATE_FUNC(PopupInGame);
};

