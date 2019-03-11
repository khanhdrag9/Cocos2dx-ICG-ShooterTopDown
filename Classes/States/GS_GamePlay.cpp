#include "GS_GamePlay.h"
#include "../Game.h"



cocos2d::Scene *GS_GamePlay::createScene()
{
    Scene* scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
    GS_GamePlay* layer = GS_GamePlay::create();
    scene->addChild(layer);
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
    Game::getInstance()->update(dt);
}

void GS_GamePlay::controller()
{
    auto keyboardlistener = EventListenerKeyboard::create();
    keyboardlistener->onKeyPressed = CC_CALLBACK_2(Game::handleKeyboardPress, Game::getInstance());
    keyboardlistener->onKeyReleased = CC_CALLBACK_2(Game::handleKeyboardRelease, Game::getInstance());
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardlistener, this);
    
    
    auto touchlistener = EventListenerTouchOneByOne::create();
    touchlistener->onTouchBegan = CC_CALLBACK_2(Game::handleTouchBegan, Game::getInstance());
    touchlistener->onTouchMoved = CC_CALLBACK_2(Game::handleTouchMoved, Game::getInstance());
    touchlistener->onTouchEnded = CC_CALLBACK_2(Game::handleTouchRelease, Game::getInstance());
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistener, this);
    
//    auto mouselistener = EventListenerMouse::create();
//    mouselistener->onMouseDown = CC_CALLBACK_1(Game::handleTouchBegan, Game::getInstance());
//    mouselistener->onMouseMove = CC_CALLBACK_1(Game::handleTouchMoved, Game::getInstance());
//    mouselistener->onMouseUp = CC_CALLBACK_1(Game::handleTouchRelease, Game::getInstance());
//    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouselistener, this);
}
