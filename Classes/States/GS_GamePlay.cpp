#include "GS_GamePlay.h"
#include "../Game.h"
#include "../Objects/ObjectsPool.h"
#include "GS_GamePlayUI.h"
#include "GS_OptionPage.h"
#include "CustomMouse.h"
#include "Joystick.h"
#include "../Characters/Player.h"

GS_GamePlay::GS_GamePlay():
	_uilayer(nullptr)
{
}

GS_GamePlay::~GS_GamePlay()
{
	Game::getInstance()->releaseGamePlay();
    if(Game::getInstance()->isEnableVolumn())
        Game::getInstance()->setEnableVolunm(true);
}

cocos2d::Scene *GS_GamePlay::createScene()
{
	Scene* scene = Scene::create();
    GS_GamePlay* layer = GS_GamePlay::create();
    layer->setTag(Game::layer::GAMEPLAY);
    layer->_uilayer = GS_GamePlayUI::create();
    scene->addChild(layer);
    scene->addChild(layer->_uilayer);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
    scene->addChild(CustomMouse::create(), 100);
    #if USE_JOYSTICK
    layer->_uilayer->useTab(true);
    layer->_uilayer->getOptionPage()->setBarOpacity(150.f);
    layer->_uilayer->getOptionPage()->setBarVisible(true, Color3B::BLACK);
    #endif
#else
    layer->_uilayer->useTab(true);
    layer->_uilayer->getOptionPage()->setBarOpacity(150.f);
    layer->_uilayer->getOptionPage()->setBarVisible(true, Color3B::BLACK);

#endif

    layer->_uilayer->setCharacter(Game::getInstance()->getPlayer());

    
    return scene;
}


bool GS_GamePlay::init()
{
    if(!Layer::init())
        return false;
    
    Game::getInstance()->setCurrentState(this);
    Game::getInstance()->initGamePlay();
    
    controller();
    
    scheduleUpdate();
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    
    return true;
}

void GS_GamePlay::update(float dt)
{
    Game::getInstance()->update(dt);
}

GS_GamePlayUI* GS_GamePlay::getUILayer() const
{
    return _uilayer;
}

void GS_GamePlay::controller()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
    auto keyboardlistener = EventListenerKeyboard::create();
    keyboardlistener->onKeyPressed = CC_CALLBACK_2(Game::handleKeyboardPress, Game::getInstance());
    keyboardlistener->onKeyReleased = CC_CALLBACK_2(Game::handleKeyboardRelease, Game::getInstance());
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardlistener, this);
#endif
    
#if USE_TOUCH
    auto touchlistener = EventListenerTouchOneByOne::create();
    touchlistener->onTouchBegan = CC_CALLBACK_2(Game::handleTouchBegan, Game::getInstance());
    touchlistener->onTouchMoved = CC_CALLBACK_2(Game::handleTouchMoved, Game::getInstance());
    touchlistener->onTouchEnded = CC_CALLBACK_2(Game::handleTouchRelease, Game::getInstance());
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistener, this);
#else
    auto mouselistener = EventListenerMouse::create();
    mouselistener->onMouseDown = CC_CALLBACK_1(Game::handleMouseBegan, Game::getInstance());
    mouselistener->onMouseMove = CC_CALLBACK_1(Game::handleMouseMoved, Game::getInstance());
    mouselistener->onMouseUp = CC_CALLBACK_1(Game::handleMouseRelease, Game::getInstance());
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouselistener, this);
#endif
}
