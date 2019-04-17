//
//  Game.cpp
//  Shooter
//
//  Created by Khanh on 3/9/19.
//

#include "Game.h"
#include "States/GS_GameMenu.h"
#include "States/GS_GamePlay.h"
#include "States/GS_GamePlayUI.h"
#include "States/PopupInGame.h"
#include "Objects/ObjectsPool.h"
#include "Characters/Player.h"
#include "Commands/CommandMoveBy.h"
#include "Commands/CommandShoot.h"
#include "Defines/constants.h"
#include "Physics/RigidBodyPolygon.h"
#include "Physics/RigidBodyCircle.h"
#include "Physics/RigidWorld.h"
#include "Defines/Templates.h"
#include "Resource/ResourceManager.h"
#include "Bot/Bot.h"
#include "Bot/BotManager.h"
#include "Envoiments/Vision.h"
#include "Envoiments/VisionPlayer.h"
#include "Envoiments/VisionEnemy.h"
#include "Bot/InformationCenter.h"

Game::Game():
_currentState(nullptr),
_player(nullptr),
_playerShoot(false),
_isHoldKey(false),
_tileMap(nullptr),
_backgroundLayer(nullptr),
_collisionLayer(nullptr),
_objIsFollow(nullptr),
_rigidWorld(nullptr),
_sightNode(nullptr),
_fogSprite(nullptr),
_fogClip(nullptr),
_isMouseDown(false),
_isPopupInGameVisible(false),
_isPopupKDAVisible(false)
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
    new InformationCenter;
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
    InformationCenter::getInstance()->start();
}

void Game::update(float dt)
{
    handleKeyboardHold();

	//player
    if(_player)
    {
        if(_player->isDestroyed())
        {
            _player = nullptr;
        }
        else
        {
            if (_playerShoot && _player->getMag()->canShoot())
            {
                handleShootCharacter((shared_ptr<Character>)_player, 1000);
            }
            _player->update(dt);
        }
    }
	//end player

    updatePhysics(dt);
    
	BotManager::getInstance()->update(dt);
    
	updateCameraView();

    updateSight(dt);
#if DEBUG_ENEMY
	for (int i = 0; i < BotManager::getInstance()->countBots(); i++)
	{
		auto obj = BotManager::getInstance()->getBot(i);
		_sightNode->drawLine(obj->_sprite->getPosition(), _player->_sprite->getPosition(), Color4F::RED);
		if (auto circleBody = dynamic_pointer_cast<RigidBodyCircle>(obj->_rigidBody))
			_sightNode->drawCircle(obj->_sprite->getPosition(), circleBody->getRadius(), 0, 360, false, Color4F::RED);
	}
#endif
    
    ObjectsPool::getInstance()->update();
    
    InformationCenter::getInstance()->update();
    InformationCenter::getInstance()->clear();
    
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
    if(keycode == EventKeyboard::KeyCode::KEY_TAB)
    {
        useKDATab(!_isPopupKDAVisible);
    }
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
   
    
    switch (keycode) {
        case cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE:
            usePopupInGame(!_isPopupInGameVisible);
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_TAB:
            useKDATab(!_isPopupKDAVisible);
            break;
#if CHEAT
        case cocos2d::EventKeyboard::KeyCode::KEY_F1:
            _player->_sprite->setPosition(getRandomPosition());        //random rivavel position
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_F2:
            //_listVision.clear();    //disble vision
            if(_listVision.size() > 0)
            {
                for (auto& vision : _listVision)
                    vision->stop();
            }
            else
            {
                shared_ptr<Vision> playerVision = createView(_player, type_vision::VISION_PLAYER);
                playerVision->setDraw(true);
                
                for(int i = 0; i < BotManager::getInstance()->countBots(); i++)
                {
                    shared_ptr<Vision> botVision = createView(BotManager::getInstance()->getBot(i), type_vision::VISION_ENEMY);
                    botVision->setDraw(false);
                }
            }
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_F3:
            //_listVision.clear();    //disble vision
            if (BotManager::getInstance()->countBots() > 0)
                BotManager::getInstance()->clear();
            else
            {
                BotManager::getInstance()->initMovePosition();
                BotManager::getInstance()->initBots();
                for(int i = 0; i < BotManager::getInstance()->countBots(); i++)
                {
                    shared_ptr<Vision> botVision = createView(BotManager::getInstance()->getBot(i), type_vision::VISION_ENEMY);
                    botVision->setDraw(false);
                }
            }
            
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_F4:
            for(auto& vision : _listVision)
            {
                if(auto b = dynamic_pointer_cast<Bot>(vision->getObject()))
                    vision->setDraw(!vision->isDraw());
            }
            break;
#endif
        default:
            break;
    }
    
}

#if USE_TOUCH
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
    
    if(obj)
        updateAngle(obj, point);
}

void Game::handleTouchRelease(Touch* touch, Event* event)
{
    
}
#else
bool Game::handleMouseBegan(EventMouse* event)
{
    _playerShoot = true;
    _isMouseDown = true;
    return true;
}

void Game::handleMouseMoved(EventMouse* event)
{
    //for player
    if(_isMouseDown) _playerShoot = true;
    
    Vec2 point = event->getLocation() + Director::getInstance()->getVisibleOrigin();
    point = _currentState->convertToNodeSpace(point);
    shared_ptr<Character> obj = _player;
    
    if(obj)
        updateAngle(obj, point);
}

void Game::handleMouseRelease(EventMouse* event)
{
    _isMouseDown = false;
    _playerShoot = false;
}
#endif

const unique_ptr<RigidWorld>& Game::getRigidWord() const
{
    return _rigidWorld;
}

void Game::releaseGamePlay()
{
//    if(_player)
//    {
//        _player->destroy();
//        _player = nullptr;
//    }
    _playerShoot = false;
    _isHoldKey = false;
    _keyIsHolds.clear();
    
    if(_tileMap)
    {
        _tileMap->removeFromParentAndCleanup(true);
        _tileMap = nullptr;
    }
    
    _objIsFollow = nullptr;
    _rigidWorld = nullptr;
    
    if(_sightNode)
    {
        _sightNode->removeFromParentAndCleanup(true);
        _sightNode = nullptr;
    }
#if DEBUG_SIGHT
    if(_debugWall)
    {
        _debugWall->removeFromParentAndCleanup(true);
        _debugWall = nullptr;
    }
#endif
    
    if(_fogSprite)
    {
        _fogSprite->removeFromParentAndCleanup(true);
        _fogSprite = nullptr;
    }
    if(_fogClip)
    {
        _fogClip->removeFromParentAndCleanup(true);
        _fogClip = nullptr;
    }
    
    for(auto& vision : _listVision)
        vision->stop();
    //_listVision.clear();
    
    _revivalPosition.clear();
    _isPopupInGameVisible = false;
    ObjectsPool::getInstance()->clear();
    
#if USE_TOUCH
#else
    _isMouseDown = false;
#endif
    auto botMgr = BotManager::getInstance();
    botMgr->clear();
    
    if(_player)
    {
        _player->releaseCommands();
        _player->destroy();
        //_player = nullptr;
    }
    
    InformationCenter::getInstance()->stop();
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
#if USE_TOUCH
        case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
			_playerShoot = true;
            break;
#endif
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
#if USE_TOUCH
    object->_sprite->setRotation(CC_RADIANS_TO_DEGREES(-angle) + 90);
#else
    object->_sprite->setRotation(CC_RADIANS_TO_DEGREES(angle) + 90);
#endif
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
    _rigidWorld = make_unique<RigidWorld>();	//C++ 11 dont contain make_unique
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
	_tileMap = TMXTiledMap::create("Map/Map2.tmx");
	_backgroundLayer = _tileMap->getLayer("Background");
	_collisionLayer = _tileMap->getLayer("Collision");
	_collisionLayer->setVisible(false);

	_currentState->addChild(_tileMap);

	//physics for collision layer
	Size mapSize = _tileMap->getMapSize();
	Vec2 scaleSize = Vec2(mapSize.width / 50.f, mapSize.height / 50.f);
	bg->setScale(scaleSize.x, scaleSize.y);
	_fogSprite->setScale(scaleSize.x, scaleSize.y);

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
	if(lineGO)
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

	//for revival position
	auto revivalObj = _tileMap->getObjectGroup("RevivalPosition");
	if (revivalObj)
		for (int i = 1; i <= revivalObj->getObjects().size(); i++)
		{
			string name = to_string(i);
			auto pointInTile1 = revivalObj->getObject(name);
			if (pointInTile1.size() == 0)continue;

			float x = pointInTile1.at("x").asFloat();
			float y = pointInTile1.at("y").asFloat();

			_revivalPosition.emplace_back(x, y);
		}
}

void Game::createMainPlayer()
{
	_player = createAPlayer();
    _player->_rigidBody->setTag(RigidBody::tag::PLAYER);
    
	Vec2 ranPosition = getRandomPosition();
	_player->_sprite->setPosition(ranPosition);

}

void Game::createStartCameraView()
{
//    auto character = (shared_ptr<Character>)_player;
    auto character = BotManager::getInstance()->getBot(0);
    if(GS_GamePlay* gameplayLayer = dynamic_cast<GS_GamePlay*>(_currentState))
    {
        if(gameplayLayer->getUILayer())
            gameplayLayer->getUILayer()->setCharacter(character);
    }
    
    
    setObjectFollowByCam(character);
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
        botVision->setDraw(true);
    }
    
#if DEBUG_SIGHT
    _debugWall = DrawNode::create();
    _currentState->addChild(_debugWall);
#endif
}

void Game::updatePhysics(float dt)
{
    if(_rigidWorld)
        _rigidWorld->update(dt);
}

Vec2 Game::getRandomPosition() const
{
	int ranIndex = RandomHelper::random_int(0, (int)_revivalPosition.size() - 1);
	return _revivalPosition.at(ranIndex);
}

void Game::updateSight(float dt)
{
    //delete vision is marked delete
    for (auto i = _listVision.begin(); i != _listVision.end();)
    {
        auto& vision = (*i);
        if (vision->getObject()->isDestroyed())
            vision->stop();
        if (vision->avaibleToDelete())
            i = _listVision.erase(i);
        else
            ++i;
    }
    
    if(!_sightNode)return;
    
#if DEBUG_SIGHT
    if(_debugWall)
    {
        _debugWall->clear();
        for (auto& line : _rigidWorld->getListLines())
        {
            _debugWall->drawLine(line.start, line.end, Color4F::RED);
        }
    }
#endif
    
    _sightNode->clear();
    
	//update vision
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

shared_ptr<Player> Game::getPlayer() const
{
	return _player;
}

void Game::usePopupInGame(bool push)
{
    if(push)
    {
        auto popup = PopupInGame::create();
        popup->setName("ingamepopup");
        if(auto gameplay = dynamic_cast<GS_GamePlay*>(_currentState))
        {
            gameplay->getUILayer()->addChild(popup);
            _isPopupInGameVisible = true;
        }
        else
            CC_SAFE_DELETE(popup);
    }
    else
    {
        if(auto gameplay = dynamic_cast<GS_GamePlay*>(_currentState))
        {
            auto popup = gameplay->getUILayer()->getChildByName("ingamepopup");
            if(popup)
            {
                popup->runAction(RemoveSelf::create());
                _isPopupInGameVisible = false;
            }
        }
    }
}

void Game::useKDATab(bool show)
{
    if(auto gameplay = dynamic_cast<GS_GamePlay*>(_currentState))
    {
        gameplay->getUILayer()->useKDATab(show);
        _isPopupKDAVisible = show;
    }
}

void Game::resetGame()
{
    
}

void Game::backToHomeMenu()
{
    releaseGamePlay();
    auto home = GS_GameMenu::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, home));
}
