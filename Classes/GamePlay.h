#pragma once
#include "Headers.h"
#include "Player.h"
#include "Command.h"

class InGameUI;

class GamePlay : public Layer
{
	unique_ptr<Command> _command;
	shared_ptr<Character> _player;
	

	Size _screenSize;
	Vec2 _origin;
	InGameUI* _uiLayer;

	//PhysicsWorld* _physWorld;
	TMXTiledMap* _tileMap;
	TMXLayer* _backgroudLayer;
	TMXLayer* _collisionLayer;


	void posInit();
	void createMap();
	void createPlayer();
	void createEnemy();
    void createViewCamera();
	void createPhysics();
	void createListener();
	void createSchedule();

	void updateAngle(shared_ptr<Character>& character, const Vec2& loc);	//to rotate player for mouse/touch
	void update(float dt) override;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
	void keyPressed(EventKeyboard::KeyCode code, Event* event);
#else
	//touch or joystick for phone...
#endif

private:
	void GetListVectorFromTileMap(vector<Vec2>& list, const string& nameGroup, const string& preNameObj ,const int& start, const int& end);

	bool contactBegin(PhysicsContact& contact);
	
public:
	static Scene* createScene();

	bool init() override;

	CREATE_FUNC(GamePlay);

	void setViewPointCenter();
	void setObjectFollowByCam(shared_ptr<Character> obj);
	shared_ptr<Character> _isFollowByCam;

	TMXTiledMap* getTiledMap() { return _tileMap; }
	TMXLayer* getBackgroundLayer() { return _backgroudLayer; }
	TMXLayer* getCollisionLayer() { return _collisionLayer; }

	friend class InGameUI;
};
