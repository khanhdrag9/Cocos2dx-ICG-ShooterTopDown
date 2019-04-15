//
//  PopupInGame.cpp
//  ShooterTopDown2D
//
//  Created by Khanh on 4/15/19.
//

#include "PopupInGame.h"

Scene* PopupInGame::createScene()
{
    auto scene = Scene::create();
    PopupInGame* layer = PopupInGame::create();
    scene->addChild(layer);
    return scene;
}

bool PopupInGame::init()
{
    if(!Layer::init())
        return false;
    
    
    
    return true;
}
