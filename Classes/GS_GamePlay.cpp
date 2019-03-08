#include "GS_GamePlay.h"




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
    
    
    return true;
}

void GS_GamePlay::update(float dt)
{
    
}
