//
//  PopupInGame.cpp
//  ShooterTopDown2D
//
//  Created by Khanh on 4/15/19.
//

#include "PopupInGame.h"
#include "../Resource/ResourceManager.h"
#include "../Game.h"
#include "GS_OptionPage.h"

PopupInGame::PopupInGame()
{
}

bool PopupInGame::init()
{
    if(!Layer::init())
        return false;
    
    Size sz = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 center = Vec2(sz.width / 2.f + origin.x, sz.height / 2.f + origin.y);
    
    auto sprite = Sprite::create("OptionAssets/Popup.png");
    sprite->setPosition(center);
    sprite->setOpacityModifyRGB(200);
//    sprite->setColor(Color3B(61, 37,38));
    this->addChild(sprite);
    
    return true;
}

void PopupInGame::setText(const string &text)
{
    
}
