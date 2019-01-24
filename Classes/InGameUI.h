#pragma once
#include "cocos2d.h"

class GamePlay;

class InGameUI : public cocos2d::Layer
{
	cocos2d::Sprite* _aim;
	GamePlay* _gamelayer;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
	void mouseBegan(cocos2d::EventMouse* event);
	void mouseMoved(cocos2d::EventMouse* event);
	void mouseRelease(cocos2d::EventMouse* event);
#else
	//touch or joystick for phone...
#endif

public:
	bool init() override;

	CREATE_FUNC(InGameUI);

	void setPositionAim(const cocos2d::Vec2& pos);

	friend class GamePlay;
};