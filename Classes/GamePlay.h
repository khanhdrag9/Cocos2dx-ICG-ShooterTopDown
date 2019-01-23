#pragma once
#include "Headers.h"
#include "Player.h"
#include "Command.h"

class GamePlay : public Layer
{
	unique_ptr<Command> _command;
	shared_ptr<Character> _player;
	Sprite* _aim;

	Size _screenSize;
	Vec2 _origin;

	//PhysicsWorld* _physWorld;

	void posInit();
	void createPlayer();
	void createPhysics();
	void createListener();
	void createSchedule();

	void updateAngle(shared_ptr<Character>& character, const Vec2& loc);	//to rotate player for mouse/touch
	void update(float dt);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
	void mouseBegan(EventMouse* event);
	void mouseMoved(EventMouse* event);
	void mouseRelease(EventMouse* event);

	void keyPressed(EventKeyboard::KeyCode code, Event* event);
#else
	//touch or joystick for phone...
#endif

	bool contactBegin(PhysicsContact& contact);
public:
	static Scene* createScene();

	bool init() override;

	CREATE_FUNC(GamePlay);
};