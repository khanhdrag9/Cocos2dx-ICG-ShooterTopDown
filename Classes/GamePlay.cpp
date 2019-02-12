#include "GamePlay.h"
#include "InGameUI.h"
#include "BotManager.h"

Scene* GamePlay::createScene()
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	//escene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	auto layer = GamePlay::create();
	scene->addChild(layer, 0);

	layer->_uiLayer = InGameUI::create();
	layer->_uiLayer->_gamelayer = layer;
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
	createEnemy();
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
		BotManager::getInstance()->update(dt, _command);
	}
	
}

void GamePlay::updateAngle(shared_ptr<Character>& character, const Vec2& loc)
{
    _command->rotate(_player, loc);
}

void GamePlay::posInit()
{
	_command = make_unique<Command>();
	_command->setGamePlay(this);
}

void GamePlay::createPlayer()
{
	_player = make_shared<Player>();
	_player = make_shared<PlayerSquare>(_player);
	_player->sprite->setTag(objecttag::PLAYER);

	//calculate position start
	TMXObjectGroup* objg = _tileMap->getObjectGroup("Player");
	auto playerPos = objg->getObject("PlayerPos");
	float x = playerPos.at("x").asFloat();
	float y = playerPos.at("y").asFloat();

	_player->sprite->setPosition(x, y);
	setViewPointCenter(_player->sprite);
	//caculate scale for screen
	float ratio = 0.5f;
	_player->sprite->setScale(ratio);
	_player->addParrent(this);

}

void GamePlay::createEnemy()
{
	auto bot = BotManager::getInstance()->createBot(_player->sprite->getPosition(), this);
	//bot->actions.SetCommand(Command::command::MOVE_UP, true);
}

void GamePlay::createPhysics()
{
	//physics for collision layer
	Size mapSize = _tileMap->getMapSize();

	for (int w = 0; w < mapSize.width; w++)
	{
		for (int h = 0; h < mapSize.height; h++)
		{
			auto tile = _collisionLayer->getTileAt(Vec2(w, h));
			if (tile)
			{
				Size size = tile->getBoundingBox().size;
				auto body = PhysicsBody::createBox(size);
				body->setContactTestBitmask(PHYSICS_EDGE);
				body->setCategoryBitmask(PHYSICS_EDGE);
				body->setCollisionBitmask(PHYSICS_EDGE);
				body->setDynamic(false);

				tile->setPhysicsBody(body);
			}
		}
	}
}

void GamePlay::createMap()
{
	_tileMap = TMXTiledMap::create(TITLEMAP_PATH);
	_backgroudLayer = _tileMap->getLayer("Background");
	_collisionLayer = _tileMap->getLayer("Collision");
	_collisionLayer->setVisible(false);

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

	auto node1 = shape1->getBody()->getNode();
	auto node2 = shape2->getBody()->getNode();

	auto shape1Collision = shape1->getCollisionBitmask();
	auto shape2Collision = shape2->getCollisionBitmask();

	if (campare2way(shape1Collision, shape2Collision, PHYSICS_PLAYER, PHYSICS_BULLET_PLAYER))
	{
		//from player!
		return false;
	}
    
    if (campare2way(shape1Collision, shape2Collision, PHYSICS_PLAYER, PHYSICS_EDGE))
    {
		//return false;
    }

	if (campare2way(shape1Collision, shape2Collision, PHYSICS_ENEMY, PHYSICS_EDGE))
	{
		//return false;
	}

	if (campare2way(shape1Collision, shape2Collision, PHYSICS_BULLET_PLAYER, PHYSICS_EDGE) ||
		campare2way(shape1Collision, shape2Collision, PHYSICS_BULLET_PLAYER, PHYSICS_BULLET_PLAYER) ||
		campare2way(shape1Collision, shape2Collision, PHYSICS_BULLET_PLAYER, PHYSICS_BULLET_BOT))
	{
		if (node1->getTag() == objecttag::BULLET)
		{
			node1->runAction(RemoveSelf::create());
		}
		if (node2->getTag() == objecttag::BULLET)
		{
			node2->runAction(RemoveSelf::create());
		}
	}

	return true;
}

void GamePlay::setViewPointCenter(Sprite* obj)
{
	Size winSize = Director::getInstance()->getWinSize();
	Vec2 position = obj->getPosition();

	int x = MAX(position.x, winSize.width / 2);
	int y = MAX(position.y, winSize.height / 2);
	x = MIN(x, (_tileMap->getMapSize().width * this->_tileMap->getTileSize().width) - winSize.width / 2);
	y = MIN(y, (_tileMap->getMapSize().height * _tileMap->getTileSize().height) - winSize.height / 2);
	Vec2 actualPosition = Vec2(x, y);

	Vec2 centerOfView = Vec2(winSize.width / 2, winSize.height / 2);
	Vec2 viewPoint = Vec2(centerOfView - actualPosition);
	this->setPosition(viewPoint);
}


