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
#include "Envoiments/Vision.h"
#include "Envoiments/VisionPlayer.h"
#include "Envoiments/VisionEnemy.h"

Game::Game():
_currentState(nullptr),
_player(nullptr),
_playerShoot(false),
_counttimePlayerShoot(0.f),
_intervelPlayerShoot(0.25f),
_isHoldKey(false),
_tileMap(nullptr),
_backgroundLayer(nullptr),
_collisionLayer(nullptr),
_objIsFollow(nullptr),
_rigidWorld(nullptr),
_sightNode(nullptr),
_fogSprite(nullptr),
_fogClip(nullptr)
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
    createSight();
	createStartCameraView();
}

void Game::update(float dt)
{
    handleKeyboardHold();

	//player
    _counttimePlayerShoot += dt;
	if (_playerShoot)
	{
		if (_counttimePlayerShoot > _intervelPlayerShoot)
		{
			handleShootCharacter((shared_ptr<Character>)_player, 1000);
			_counttimePlayerShoot = 0.f;
		}
		_playerShoot = false;
	}
    _player->update(dt);
	//end player

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
			_playerShoot = true;
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
    
    //Fog
    _fogSprite = Sprite::create(ResourceManager::getInstance()->at(res::define::IMG_FOG));
    _fogSprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _fogSprite->setPosition(0,0);
    _fogSprite->setOpacity(225.f);
    //_currentState->addChild(_fogSprite, 10);
    

    
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
    
    //for sight
    auto lineGO = _tileMap->getObjectGroup("Lines");
    for(int i = 1; i <= lineGO->getObjects().size(); i++)
    {
        //start
        string name = to_string(i);
        auto pointInTile1 = lineGO->getObject(name);
        if(pointInTile1.size() == 0)continue;
        
        float x = pointInTile1.at("x").asFloat();
        float y = pointInTile1.at("y").asFloat();
        
        Vec2 start(x, y);
        
        //get end
        auto next = pointInTile1.at("next").asInt();
        if(next > 0)
        {
            auto pointInTile2 = lineGO->getObject(to_string(next));
            if(pointInTile2 != ValueMap())
            {
                Vec2 end = Vec2(pointInTile2.at("x").asFloat(), pointInTile2.at("y").asFloat());
                _rigidWorld->pushLine(Line(start, end));
            }
        }
    }

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
//    setObjectFollowByCam(BotManager::getInstance()->getBot(0));
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
    _fogClip = ClippingNode::create();
    _fogClip->setStencil(_sightNode);
    _fogClip->setInverted(true);
    _fogClip->addChild(_fogSprite);
    _currentState->addChild(_fogClip, 100);
    
    shared_ptr<Vision> playerVision = createView(_player, type_vision::VISION_PLAYER);
    playerVision->setDraw(true);
    
    for(int i = 0; i < BotManager::getInstance()->countBots(); i++)
    {
        shared_ptr<Vision> botVision = createView(BotManager::getInstance()->getBot(i), type_vision::VISION_ENEMY);
        botVision->setDraw(false);
    }
    
#if DEBUG_SIGHT
    _debugWall = DrawNode::create();
    _currentState->addChild(_debugWall);
#endif
}

void Game::updatePhysics(float dt)
{
    _rigidWorld->update(dt);
}

void Game::updateSight(float dt)
{
    if(!_sightNode)return;
    
#if DEBUG_SIGHT
    _debugWall->clear();
    for (auto& line : _rigidWorld->getListLines())
    {
        _debugWall->drawLine(line.start, line.end, Color4F::RED);
    }
#endif
    
    _sightNode->clear();
    
    for(auto& vision : _listVision)
    {
        vision->update(_sightNode);
    }
    
    
}

void Game::pushView(shared_ptr<Vision> vision)
{
    _listVision.push_back(vision);
}

shared_ptr<Vision> Game::createView(shared_ptr<Character> object, type_vision type)
{
	shared_ptr<Vision> vision;
	
	switch (type)
	{
	case type_vision::VISION_PLAYER:
		vision = make_shared<VisionPlayer>(object);
		break;
	case type_vision::VISION_ENEMY:
		vision = make_shared<VisionEnemy>(object);
		break;
	default:
		vision = make_shared<Vision>(object);
		break;
	}
	
    _listVision.push_back(vision);
    return _listVision.back();
}

