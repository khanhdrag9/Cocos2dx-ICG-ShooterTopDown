#pragma once

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)

#include "../Headers.h"

class CustomMouse : public Layer
{
    Sprite* _cursor;
    
public:
    bool init() override;
    
    CREATE_FUNC(CustomMouse);
};
#endif
