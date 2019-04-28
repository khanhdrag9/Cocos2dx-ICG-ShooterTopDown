#pragma once
#include "../Headers.h"

class Character;


class PopupInGame : public Layer
{
    
public:
    PopupInGame();
    bool init() override;
    
    CREATE_FUNC(PopupInGame);
};

