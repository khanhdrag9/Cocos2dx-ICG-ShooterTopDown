//
//  Game.cpp
//  Shooter
//
//  Created by Khanh on 3/9/19.
//

#include "Game.h"
#include "Objects/ObjectsPool.h"
#include "Characters/Player.h"
#include "Commands/CommandMoveBy.h"
#include "Commands/CommandShoot.h"
#include "Defines/constants.h"
#include "Physics/RigidBodyPolygon.h"
#include "Physics/RigidWorld.h"
#include "Defines/Templates.h"
#include "Resource/ResourceManager.h"
#include "Bot/Bot.h"
#include "Bot/BotManager.h"

Game::Game():
_currentState(nullptr),
_player(nullptr),
_isHoldKey(false),
_counttimePlayerShoot(0.f),
_intervelPlayerShoot(0.25f),
_tileMap(nullptr),
_backgroundLayer(nullptr),
_collisionLayer(nullptr),
_objIsFollow(nullptr),
_rigidWorld(nullptr),
_sightNode(nullptr)
{
    
}

Game::~Game()
{
	/*CC_SAFE_DELETE(_tileMap);
	CC_SAFE_DELETE(_backgroundLayer);
	CC_SAFE_DELETE(_collisionLayer);

	_keyIsHolds.clear();*/
}

void Game::init()
{
    new ResourceManager;
}

void Game::initGamePlay()
{
    createPhysicsWorld();
	createMap();
	createMainPlayer();
    BotManager::getInstance()->initMovePosition();
	BotManager::getInstance()->initBots();
	createStartCameraView();
    createSight();
}

void Game::update(float dt)
{
    handleKeyboardHold();
    _counttimePlayerShoot += dt;
    
    _player->update(dt);
    updatePhysics(dt);
    
	BotManager::getInstance()->update(dt);
    
	updateCameraView();
    updateSight(dt);
    
    ObjectsPool::getInstance()->update();
}

void Game::setCurrentState(Layer* layer)
{
    _currentState = layer;
}

Layer* Game::getCurrentState()
{
    return _currentState;
}

void Game::setObjectFollowByCam(shared_ptr<Character> object)
{
	_objIsFollow = object;
}

void Game::handleKeyboardPress(EventKeyboard::KeyCode keycode, Event*)  //used in gameplay
{
    _isHoldKey = true;
    _keyIsHolds.push_back(keycode);
}

void Game::handleKeyboardHold()
{
    if(_isHoldKey)
    {
        for(auto& key : _keyIsHolds)
            handleMovePlayerKeyCode(key);
    }
}

void Game::handleKeyboardRelease(EventKeyboard::KeyCode keycode, Event*)    //used in gameplay
{
    for(int i = 0; i < _keyIsHolds.size(); i++)
    {
        if(_keyIsHolds[i] == keycode)
        {
            _keyIsHolds.erase(_keyIsHolds.begin() + i);
            break;
        }
    }
    
    if(_keyIsHolds.size() == 0)_isHoldKey = false;
}

bool Game::handleTouchBegan(Touch* touch, Event* event)
{
    return true;
}

void Game::handleTouchMoved(Touch* touch, Event* event)
{
    //for player
    Vec2 point = touch->getLocation();
    point = _currentState->convertToNodeSpace(point);
    shared_ptr<Character> obj = _player;
    updateAngle(obj, point);
}

void Game::handleTouchRelease(Touch* touch, Event* event)
{
    
}

const unique_ptr<RigidWorld>& Game::getRigidWord() const
{
    return _rigidWorld;
}

void Game::releaseGamePlay()
{
    _player = nullptr;
    _isHoldKey = false;
    _counttimePlayerShoot = 0.f;
    _intervelPlayerShoot = 0.25f;
    CC_SAFE_DELETE(_backgroundLayer);
    CC_SAFE_DELETE(_collisionLayer);
     CC_SAFE_DELETE(_tileMap);
    _objIsFollow = nullptr;
    _rigidWorld = nullptr;
    
    _keyIsHolds.clear();
    ObjectsPool::getInstance()->clear();
}

TMXTiledMap * Game::getTileMap() const
{
	return _tileMap;
}

shared_ptr<Player> Game::createAPlayer()
{
    auto character = make_shared<Player>();
    character->init();
    _rigidWorld->createRigidBodyCircle(character);
    
    _currentState->addChild(character->_sprite);
    return character;
}

void Game::handleMovePlayerKeyCode(EventKeyboard::KeyCode keycode)
{
    switch (keycode) {
        case cocos2d::EventKeyboard::KeyCode::KEY_W:
            handleMovePlayer(_player, Game::direction::UP);
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_S:
            handleMovePlayer(_player, Game::direction::DOWN);
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_A:
            handleMovePlayer(_player, Game::direction::LEFT);
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_D:
            handleMovePlayer(_player, Game::direction::RIGHT);
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
            if(_counttimePlayerShoot > _intervelPlayerShoot)
            {
                handleShootCharacter((shared_ptr<Character>)_player, 1000);
                _counttimePlayerShoot = 0.f;
            }
            break;
        default:
            break;
    }
}

void Game::handleMovePlayer(shared_ptr<Player> player, const Game::direction& direct)
{
    Vec2 movespeed = Vec2(0,0);
    shared_ptr<Command> cmd;
    
    switch (direct) {
        case direction::UP:
            movespeed.y = player->getSpeedMove();
            break;
        case direction::DOWN:
            movespeed.y = -player->getSpeedMove();
            break;
        case direction::LEFT:
            movespeed.x = -player->getSpeedMove();
            break;
        case direction::RIGHT:
            movespeed.x = player->getSpeedMove();
            break;
        case direction::NONE:
            break;
        default:
            break;
    }
    
    cmd = CommandMoveBy::createCommandMoveBy(movespeed, 0.1);
    player->pushCommand(cmd);
}

void Game::updateAngle(shared_ptr<Character> object, const Vec2& point)
{
    Vec2 objpos = object->_sprite->getPosition();
    //float oldAngle = object->_sprite->getRotation();
    
    auto angle = atan2(point.y - objpos.y, point.x - objpos.x);
    object->_sprite->setRotation(CC_RADIANS_TO_DEGREES(-angle) + 90);
}

void Game::handleShootCharacter(shared_ptr<Character> object, const float& speed)
{
    float rotate = object->_sprite->getRotation();
    float vx = sin(CC_DEGREES_TO_RADIANS(rotate));
    float vy = cos(CC_DEGREES_TO_RADIANS(rotate));
    Vec2 offset = Vec2(vx, vy);
    offset.normalize();
    Vec2 velocity = offset * speed;
    
    shared_ptr<Command> cmdShoot = CommandShoot::createCommandShoot(velocity);
    object->pushCommand(cmdShoot);
}

void Game::createPhysicsWorld()
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    _rigidWorld = pointer::make_unique<RigidWorld>();	//C++ 11 dont contain make_unique
#else
	_rigidWorld = make_unique<RigidWorld>();
#endif
}

void Game::createMap()
{
    //BG
    Sprite* bg = Sprite::create(ResourceManager::getInstance()->at(res::define::IMG_BG_1));
    bg->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    bg->setPosition(0,0);
    _currentState->addChild(bg, 0);
    
	//for map backgroud
	_tileMap = TMXTiledMap::create("Map/Map1.tmx");
	_backgroundLayer = _tileMap->getLayer("Background");
	_collisionLayer = _tileMap->getLayer("Collision");
	_collisionLayer->setVisible(false);

	_currentState->addChild(_tileMap);

	//physics for collision layer
    Size mapSize = _tileMap->getMapSize();

    for (int w = 0; w < mapSize.width; w++)
    {
        for (int h = 0; h < mapSize.height; h++)
        {
            auto tile = _collisionLayer->getTileAt(Vec2(w, h));
            if (tile)
            {
                Rect size = tile->getBoundingBox();
                auto body = _rigidWorld->createRigidBodyPolygon(size);
                body->setTag(RigidBody::tag::WALL);
            }
        }
    }

	_rigidWorld->generateLinesAgain();
}

void Game::createMainPlayer()
{
	_player = createAPlayer();
    _player->_rigidBody->setTag(RigidBody::tag::PLAYER);
    
	//get position start from tileMap
	TMXObjectGroup* objg = _tileMap->getObjectGroup("Player");
	auto playerPos = objg->getObject("PlayerPos");
	float x = playerPos.at("x").asFloat();
	float y = playerPos.at("y").asFloat();
	_player->_sprite->setPosition(x, y);

}

void Game::createStartCameraView()
{
	setObjectFollowByCam((shared_ptr<Character>)_player);
    //setObjectFollowByCam(BotManager::getInstance()->getBot(0));
	updateCameraView();
}

void Game::updateCameraView()
{
	if (_objIsFollow)
	{
		Size winSize = Director::getInstance()->getWinSize();
		Vec2 position = _objIsFollow->_sprite->getPosition();

		int x = MAX(position.x, winSize.width / 2);
		int y = MAX(position.y, winSize.height / 2);
		x = MIN(x, (_tileMap->getMapSize().width * this->_tileMap->getTileSize().width) - winSize.width / 2);
		y = MIN(y, (_tileMap->getMapSize().height * _tileMap->getTileSize().height) - winSize.height / 2);
		Vec2 actualPosition = Vec2(x, y);

		Vec2 centerOfView = Vec2(winSize.width / 2, winSize.height / 2);
		Vec2 viewPoint = Vec2(centerOfView - actualPosition);
		_currentState->setPosition(viewPoint);
	}
}

void Game::createSight()
{
    _sightNode = DrawNode::create();
    _currentState->addChild(_sightNode);
}

void Game::updatePhysics(float dt)
{
    _rigidWorld->update(dt);
}

void Game::updateSight(float dt)
{
    if(!_sightNode)return;
    
    _sightNode->clear();
    
    Vec2 playerPos = _player->_sprite->getPosition();
    float dimention = 400.f;

	//for sight 360 degree
//    float view = 360;
//    for(int i = 0; i < view; i+=10)
//    {
//        float x = sin(CC_DEGREES_TO_RADIANS(i)) * dimention + playerPos.x;
//        float y = cos(CC_DEGREES_TO_RADIANS(i)) * dimention + playerPos.y;
//        _sightNode->drawLine(playerPos, Vec2(x, y), Color4F::YELLOW);
//    }
    
	//for each rect of bodies
    /*for(auto& body : _rigidWorld->getListBodies())
    {
        if(body->getTag() == RigidBody::tag::WALL)
        {
            shared_ptr<RigidBodyPolygon> wall = dynamic_pointer_cast<RigidBodyPolygon>(body);
            if(wall)
            {
                Rect rect = wall->getRect();
                vector<Vec2> vertices
                {
                    Vec2(rect.getMinX(), rect.getMaxY()),
                    Vec2(rect.getMinX(), rect.getMinY()),
                    Vec2(rect.getMaxX(), rect.getMinY()),
                    Vec2(rect.getMaxX(), rect.getMaxY())
                };
                
                for(auto& point : vertices)
                {
                    float length = (point - playerPos).length();
                    if(length <= dimention)
                    {
                        Vec2 des = point - playerPos;
                        des.normalize();
                        des *= dimention;
                        des += playerPos;

                        _sightNode->drawLine(playerPos, des, Color4F::YELLOW);
                        
                    }
                }
                
            }
        }
    }*/


	//for lines (new)
	for (auto& line : _rigidWorld->getListLines())
	{
		Vec2 vertices[]{ line.start, line.end };

		for (auto& point : vertices)
		{
			float length = (point - playerPos).length();
			if (length <= dimention)
			{
				Vec2 des = point - playerPos;
				des.normalize();
				des *= dimention;
				des += playerPos;

				_sightNode->drawLine(playerPos, des, Color4F::YELLOW);
			}
		}
	}
}


