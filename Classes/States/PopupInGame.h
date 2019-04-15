#include "../Headers.h"

class Character;

class PopupInGame : public Layer
{
    
public:
    PopupInGame();
    
    static Scene* createScene();
    
    bool init() override;
    
    
    CREATE_FUNC(GS_GamePlayUI);
};

