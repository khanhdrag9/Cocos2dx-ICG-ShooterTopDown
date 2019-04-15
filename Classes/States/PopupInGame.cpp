//
//  PopupInGame.cpp
//  ShooterTopDown2D
//
//  Created by Khanh on 4/15/19.
//

#include "PopupInGame.h"
#include "../Resource/ResourceManager.h"

PopupInGame::PopupInGame()
{
}

bool PopupInGame::init()
{
    if(!Layer::init())
        return false;
    
    auto resMgr = ResourceManager::getInstance();
    
    auto bg = Sprite::create(resMgr->at(res::define::IMG_BLACK));
    bg->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    bg->setOpacity(150);
    this->addChild(bg);
    
    //popup here
    
    return true;
}
