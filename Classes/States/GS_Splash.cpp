#include "GS_Splash.h"
#include "../Resource/ResourceManager.h"
#include "../Game.h"
#include "../Objects/ObjectsPool.h"
#include "GS_GameMenu.h"

Scene* GS_Splash::createScene()
{
    Scene* scene = Scene::create();
    GS_Splash* layer = GS_Splash::create();
    scene->addChild(layer);
    return scene;
}

bool GS_Splash::init()
{
    if(!Layer::init())
        return false;
    _isInit = false;
    
    new ResourceManager;
    
    Size sz = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    Sprite* splash = Sprite::create(ResourceManager::getInstance()->at(res::define::SPLASH));
    splash->setPosition(sz.width / 2.f + origin.x, sz.height / 2.f + origin.y);
    splash->setScale(0.5f);
    
    this->addChild(splash);
    
    this->scheduleUpdate();
    
    return true;
}

void GS_Splash::update(float dt)
{
    if(!_isInit)
    {
        new Game;
        new ObjectsPool;
        
        Game::getInstance()->setCurrentState(this);
        Game::getInstance()->init();

        this->schedule(schedule_selector(GS_Splash::changeToGameMenu), 2.f);
        _isInit = true;
    }
}

void GS_Splash::changeToGameMenu(float time)
{
    Scene* menu = GS_GameMenu::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, menu));
}
