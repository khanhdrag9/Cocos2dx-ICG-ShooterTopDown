#pragma once
#include "Headers.h"


class GS_PickMap : public Layer
{
    
    
public:
    CREATE_FUNC(GS_PickMap);
    
    GS_PickMap();
    static Scene* createScene();
    
    bool init() override;
};
