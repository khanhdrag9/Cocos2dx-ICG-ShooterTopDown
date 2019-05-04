#include "GS_GamePlay.h"
#include "../Game.h"
#include "../Objects/ObjectsPool.h"
#include "GS_GamePlayUI.h"
#include "GS_OptionPage.h"
#include "CustomMouse.h"

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
    scene->addChild(CustomMouse::create(), 100);
    
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
    auto keyboardlistener = EventListenerKeyboard::create();
    keyboardlistener->onKeyPressed = CC_CALLBACK_2(Game::handleKeyboardPress, Game::getInstance());
    keyboardlistener->onKeyReleased = CC_CALLBACK_2(Game::handleKeyboardRelease, Game::getInstance());
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardlistener, this);
    
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
