#include "GS_GamePlay.h"
#include "../Game.h"



cocos2d::Scene *GS_GamePlay::createScene()
{
    Scene* scene = Scene::create();
    
    GS_GamePlay* layer = GS_GamePlay::create();
    scene->addChild(layer);
    return scene;
}


bool GS_GamePlay::init()
{
    if(!Layer::init())
        return false;
    
    Game::getInstance()->init();
    Game::getInstance()->setCurrentState(this);
    Game::getInstance()->createCharacters();
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
}
