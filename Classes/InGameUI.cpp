#include "InGameUI.h"
#include "Headers.h"
#include "GamePlay.h"

USING_NS_CC;

bool InGameUI::init()
{
	if (!Layer::init())
		return false;

	auto _screenSize = Director::getInstance()->getVisibleSize();
	auto _origin = Director::getInstance()->getVisibleOrigin();

	//for AIM
	Vec2 centerpos = Vec2(_origin.x + _screenSize.width * 0.5f, _origin.y + _screenSize.height / 2.f);
	_aim = Sprite::create(AIM1);
	_aim->setScale(0.25f);
	_aim->setPosition(centerpos);
	this->addChild(_aim);

	//listener touch/mouse
	auto mouselistener = EventListenerMouse::create();
	mouselistener->onMouseDown = CC_CALLBACK_1(InGameUI::mouseBegan, this);
	mouselistener->onMouseMove = CC_CALLBACK_1(InGameUI::mouseMoved, this);
	mouselistener->onMouseUp = CC_CALLBACK_1(InGameUI::mouseRelease, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouselistener, this);

	return true;
}

void InGameUI::setPositionAim(const cocos2d::Vec2& pos)
{
	_aim->setPosition(pos);
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
void InGameUI::mouseBegan(EventMouse* event)
{
	Vec2 mousePosition = Vec2(event->getCursorX(), event->getCursorY()) + _gamelayer->_origin;
	_gamelayer->updateAngle(_gamelayer->_player, mousePosition);

	_gamelayer->_command->remote(_gamelayer->_player, mousePosition, EventMouse::MouseEventType::MOUSE_DOWN, true);
}

void InGameUI::mouseMoved(EventMouse* event)
{
	Vec2 mousePosition = Vec2(event->getCursorX(), event->getCursorY()) + _gamelayer->_origin;

	setPositionAim(mousePosition);

	//rotate player forward pointer
	if (_gamelayer->_player)
	{
		_gamelayer->updateAngle(_gamelayer->_player, mousePosition);
	}

	_gamelayer->_command->remote(_gamelayer->_player, mousePosition, EventMouse::MouseEventType::MOUSE_MOVE, true);
}

void InGameUI::mouseRelease(EventMouse* event)
{
	Vec2 mousePosition = Vec2(event->getCursorX(), event->getCursorY()) + _gamelayer->_origin;
	_gamelayer->_command->remote(_gamelayer->_player, mousePosition, EventMouse::MouseEventType::MOUSE_UP, false);
}

#else
//touch or joystick for phone...

#endif
