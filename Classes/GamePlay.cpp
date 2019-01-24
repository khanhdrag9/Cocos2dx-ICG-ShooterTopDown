#include "GamePlay.h"
#include "InGameUI.h"

Scene* GamePlay::createScene()
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	auto layer = GamePlay::create();
	scene->addChild(layer, 0);

	layer->_uiLayer = InGameUI::create();
	layer->_uiLayer->_gamelayer = layer;
	layer->_uiLayer->setTag(1);
	scene->addChild(layer->_uiLayer, 1);

	return scene;
}

bool GamePlay::init()
{
	if (!Layer::init())
		return false;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
    Director::getInstance()->getOpenGLView()->setCursorVisible(false);
#endif
	_screenSize = Director::getInstance()->getVisibleSize();
	_origin = Director::getInstance()->getVisibleOrigin();

	posInit();
	createMap();
	createPlayer();
	createPhysics();
	createListener();
	createSchedule();

	return true;
}

void GamePlay::update(float dt)
{
	if (_command)
	{
		_command->handleActionsCharacter(_player, dt);
	}
	setViewPointCenter(_player->sprite);
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
	TMXObjectGroup* objg = _tileMap->objectGroupNamed("Player");
	auto playerPos = objg->objectNamed("PlayerPos");
	float x = playerPos.at("x").asFloat();
	float y = playerPos.at("y").asFloat();

	_player->sprite->setPosition(x, y);
	setViewPointCenter(_player->sprite);
	//caculate scale for screen
	float ratio = 1.f;
	_player->sprite->setScale(ratio);
	_player->addParrent(this);

}

void GamePlay::createPhysics()
{
	//physics for player
	auto body = PhysicsBody::createBox(_player->sprite->getContentSize());
	body->setRotationEnable(true);
	body->setContactTestBitmask(PHYSICS_PLAYER);
	body->setCategoryBitmask(PHYSICS_PLAYER);
	body->setCollisionBitmask(PHYSICS_PLAYER);
	_player->sprite->setPhysicsBody(body);

}

void GamePlay::createMap()
{
	_tileMap = TMXTiledMap::create(TITLEMAP_PATH);
	_backgroudLayer = _tileMap->layerNamed("Board");

	this->addChild(_tileMap);

}

void GamePlay::createListener()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
	//use mouse on compute os
	/*auto mouselistener = EventListenerMouse::create();
	mouselistener->onMouseDown = CC_CALLBACK_1(GamePlay::mouseBegan, this);
	mouselistener->onMouseMove = CC_CALLBACK_1(GamePlay::mouseMoved, this);
	mouselistener->onMouseUp = CC_CALLBACK_1(GamePlay::mouseRelease, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouselistener, this);*/

	auto keyboard = EventListenerKeyboard::create();
	keyboard->onKeyPressed = CC_CALLBACK_2(GamePlay::keyPressed, this);
	keyboard->onKeyReleased = [this](EventKeyboard::KeyCode code, Event*) {
		_command->remote(_player, code, false);
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboard, this);
#else
	//touch or joystick for phone...

#endif

	auto listener = EventListenerPhysicsContact::create();
	listener->onContactBegin = CC_CALLBACK_1(GamePlay::contactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void GamePlay::createSchedule()
{
	scheduleUpdate();
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
void GamePlay::keyPressed(EventKeyboard::KeyCode code, Event* event)
{
	_command->remote(_player, code, true);
}

#else
	//touch or joystick for phone...

#endif

template<typename T>
bool campare2way(const T& a, const T& b, const T& value1, const T& value2)
{
	if ((a == value1 && b == value2) || (a == value2 && b == value1))
		return true;
	else
		return false;
}

bool GamePlay::contactBegin(PhysicsContact& contact)
{
	auto shape1 = contact.getShapeA();
	auto shape2 = contact.getShapeB();

	auto shape1Collision = shape1->getCollisionBitmask();
	auto shape2Collision = shape2->getCollisionBitmask();

	if (campare2way(shape1Collision, shape2Collision, PHYSICS_PLAYER, PHYSICS_BULLET_PLAYER))
	{
		//from player!
		return false;
	}
    
    if (campare2way(shape1Collision, shape2Collision, PHYSICS_PLAYER, PHYSICS_EDGE))
    {
       
    }

	if (campare2way(shape1Collision, shape2Collision, PHYSICS_PLAYER, PHYSICS_EDGE))
	{
		
	}

	return true;
}

void GamePlay::setViewPointCenter(Sprite* obj)
{
	//auto follow = Follow::create(obj);
	//this->runAction(follow);
	//_uiLayer->runAction(follow);

	Size winSize = Director::sharedDirector()->getWinSize();
	Vec2 position = obj->getPosition();

	int x = MAX(position.x, winSize.width / 2);
	int y = MAX(position.y, winSize.height / 2);
	x = MIN(x, (_tileMap->getMapSize().width * this->_tileMap->getTileSize().width) - winSize.width / 2);
	y = MIN(y, (_tileMap->getMapSize().height * _tileMap->getTileSize().height) - winSize.height / 2);
	CCPoint actualPosition = ccp(x, y);

	CCPoint centerOfView = ccp(winSize.width / 2, winSize.height / 2);
	CCPoint viewPoint = ccpSub(centerOfView, actualPosition);
	this->setPosition(viewPoint);

	//set REAL position that you see on the screen!
	_player->setPosInScreen(Vec2(MIN(position.x, winSize.width / 2), MIN(position.y, winSize.height / 2)));
}


