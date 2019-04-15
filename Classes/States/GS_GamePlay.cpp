#include "GS_GamePlay.h"
#include "../Game.h"
#include "../Objects/ObjectsPool.h"
#include "GS_GamePlayUI.h"


cocos2d::Scene *GS_GamePlay::createScene()
{
	Scene* scene = Scene::create();
    GS_GamePlay* layer = GS_GamePlay::create();
    layer->_uilayer = GS_GamePlayUI::create();
    scene->addChild(layer);
    scene->addChild(layer->_uilayer);
    return scene;
}


bool GS_GamePlay::init()
{
    if(!Layer::init())
        return false;
    
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
//    Director::getInstance()->getOpenGLView()->setCursorVisible(false);
//    auto _screenSize = Director::getInstance()->getVisibleSize();
//    auto _origin = Director::getInstance()->getVisibleOrigin();
//    
//    //for AIM
//    Vec2 centerpos = Vec2(_origin.x + _screenSize.width * 0.5f, _origin.y + _screenSize.height / 2.f);
//    _aim = Sprite::create("aim/aim1.png");
//    _aim->setScale(0.25f);
//    _aim->setPosition(centerpos);
//    this->addChild(_aim);
//#endif
    
    Game::getInstance()->setCurrentState(this);
    Game::getInstance()->initGamePlay();
    
    controller();
    
    scheduleUpdate();
    
    return true;
}

void GS_GamePlay::update(float dt)
{
   /* for (int i = 0; i < 5; ++i)
    {
        this->getScene()->getPhysicsWorld()->step(1 / 180.0f);
    }*/
    
    Game::getInstance()->update(dt);

	/*if (test->get_id);
	test = make_unique<thread>(&Game::updateSight, Game::getInstance(), dt);
	test->join();*/
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
