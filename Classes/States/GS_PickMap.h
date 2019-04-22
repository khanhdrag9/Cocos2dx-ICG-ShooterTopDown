#pragma once
#include "Headers.h"


class GS_PickMap : public Layer
{
	ui::PageView* _pageViewGlobal;
	int _countPages;
	ui::PageView* _pageViewCharacter;
	int _countCharacter;
    ui::PageView* _pageView;
    int _countMap;

	ui::Button* _nextPageLeft;
	ui::Button* _nextPageRight;
public:
	enum page : int
	{
		MAP = 0,
		CHARACTER = 1,
		TOTAL = 2
	};

    CREATE_FUNC(GS_PickMap);
    
    GS_PickMap();
    static Scene* createScene();
    
    bool init() override;
    void GoToMap(const int& index);
	void GoToSidePage(const int& index);
};
