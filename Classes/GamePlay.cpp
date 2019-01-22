#include "GamePlay.h"

Scene* GamePlay::createScene()
{
	auto scene = Scene::create();
	auto layer = GamePlay::create();
	scene->addChild(layer);
	return scene;
}

bool GamePlay::init()
{
	if (!Layer::init())
		return false;

	_screenSize = Director::getInstance()->getVisibleSize();
	_origin = Director::getInstance()->getVisibleOrigin();

	posInit();
	createPlayer();
	createListener();
	createSchedule();

	return true;
}

void GamePlay::update(float dt)
{
	if (_command)
	{
		_command->handleActionsCharacter(_player);
	}
}

void GamePlay::updateAngle(shared_ptr<Character>& character, const Vec2& loc)
{
	Vec2 arrowWorldSpace = character->getArrowWorldSpace();
	auto angle = atan2(loc.y - arrowWorldSpace.y, loc.x - arrowWorldSpace.x);
	_player->sprite->setRotation(CC_RADIANS_TO_DEGREES(-angle) + 90);
}

void GamePlay::posInit()
{
	_command = make_unique<Command>();
}

void GamePlay::createPlayer()
{
	_player = make_unique<Player>();

	//calculate position start
	Vec2 startPos = Vec2(_origin.x + _screenSize.width * 0.2, _origin.y + _screenSize.height / 2.f);
	_player->sprite->setPosition(startPos);

	//caculate scale for screen
	float ratio = 0.5f;
	_player->sprite->setScale(ratio);

	_player->addParrent(this);

	//for AIM
	Vec2 centerpos = Vec2(_origin.x + _screenSize.width * 0.5f, _origin.y + _screenSize.height / 2.f);
	_aim = Sprite::create(AIM1);
	_aim->setScale(0.25f);
	_aim->setPosition(centerpos);
	this->addChild(_aim);
}

void GamePlay::createListener()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
	//use mouse on compute os
	auto mouselistener = EventListenerMouse::create();
	mouselistener->onMouseDown = CC_CALLBACK_1(GamePlay::mouseBegan, this);
	mouselistener->onMouseMove = CC_CALLBACK_1(GamePlay::mouseMoved, this);
	mouselistener->onMouseUp = CC_CALLBACK_1(GamePlay::mouseRelease, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouselistener, this);

	auto keyboard = EventListenerKeyboard::create();
	keyboard->onKeyPressed = CC_CALLBACK_2(GamePlay::keyPressed, this);
	keyboard->onKeyReleased = [this](EventKeyboard::KeyCode code, Event*) {
		_command->remote(_player, code, false);
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboard, this);
#else
	//touch or joystick for phone...

#endif
}

void GamePlay::createSchedule()
{
	scheduleUpdate();
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
void GamePlay::mouseBegan(EventMouse* event)
{
	Vec2 mousePosition = Vec2(event->getCursorX(), event->getCursorY()) + _origin;
	updateAngle(_player, mousePosition);
}

void GamePlay::mouseMoved(EventMouse* event)
{
	Vec2 mousePosition = Vec2(event->getCursorX(), event->getCursorY()) + _origin;
	if (_aim)
	{
		Vec2 pos = mousePosition;
		_aim->setPosition(pos);
	}

	//rotate player forward pointer
	if (_player)
	{
		updateAngle(_player, mousePosition);
	}
}

void GamePlay::mouseRelease(EventMouse* event)
{

}

void GamePlay::keyPressed(EventKeyboard::KeyCode code, Event* event)
{
	_command->remote(_player, code, true);
}
#else
	//touch or joystick for phone...

#endif