#pragma once
#include "Headers.h"

class GS_OptionPage;

class GS_PickMap : public Layer
{
	ui::PageView* _pageViewGlobal;
	int _countPages;
	ui::PageView* _pageViewCharacter;
	int _countCharacter;
    ui::PageView* _pageView;
    int _countMap;

    Label* _playLabel;
    bool _isFirstIn;
public:
	enum page : int
	{
		MAP = 0,
		CHARACTER = 1,
		TOTAL = 2
	};

    CREATE_FUNC(GS_PickMap);
    
    GS_PickMap();
//    static Scene* createScene();
    
    bool init() override;
    void GoToMap(const int& index, const int& character);
    void GoToSidePage(const int& index);
    int getIndexPageViewGlobal() const;
    GS_OptionPage* _optionPage;
};
