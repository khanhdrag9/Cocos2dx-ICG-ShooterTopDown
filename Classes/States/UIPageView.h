#pragma once
#include "../Headers.h"



class UIPageView : public Layer
{
    
public:
	CREATE_FUNC(UIPageView);

	UIPageView();
	~UIPageView();

	bool init() override;

	
};

