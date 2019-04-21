#pragma once
#include "Headers.h"


class GS_PickMap : public Layer
{
    ui::PageView* _pageView;
    int _countMap;
public:
    CREATE_FUNC(GS_PickMap);
    
    GS_PickMap();
    static Scene* createScene();
    
    bool init() override;
    void GoToMap(const int& index);
};
