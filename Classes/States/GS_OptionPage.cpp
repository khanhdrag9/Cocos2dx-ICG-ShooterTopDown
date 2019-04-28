//
//  GS_OptionPage.cpp
//  ShooterTopDown2D
//
//  Created by Khanh on 4/28/19.
//

#include "GS_OptionPage.h"
#include "GS_GameMenu.h"
#include "../Game.h"
#include "../Resource/ResourceManager.h"

bool GS_OptionPage::init()
{
    if(!Layer::init())
        return false;
    
    Size screenSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 center = Vec2(screenSize.width / 2.f + origin.x, screenSize.height / 2.f + origin.y);
    
    auto resMgr = ResourceManager::getInstance();
    auto game = Game::getInstance();
    
    _colorUI = Color3B::BLACK;
    //option
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    _option = ui::Button::create("OptionAssets/optionFill.png");
#else
    _option = ui::Button::create("optionFill.png");
#endif
    _option->setColor(_colorUI);
    _option->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    _option->setPosition(Vec2(origin.x + screenSize.width, origin.y + screenSize.height));
    _option->setScale(0.25);
    _option->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
            GoToOption();
    });
    this->addChild(_option);
    
    bool volumnIsEnable = game->isEnableVolumn();
    game->setEnableVolunm(volumnIsEnable);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    _volumn = ui::Button::create("OptionAssets/volumnOn.png");
#else
    _volumn = ui::Button::create("volumnOn.png");
#endif
    _volumn->setColor(_colorUI);
    _volumn->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    _volumn->setPosition(Vec2(origin.x + screenSize.width, origin.y + screenSize.height * 0.9));
    _volumn->setScale(0.25);
    _volumn->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
            this->modifyVolumn();
    });
    this->addChild(_volumn);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    _disbleVolumn = Sprite::create("OptionAssets/volumnOff.png");
#else
    _disbleVolumn = Sprite::create("volumnOff.png");
#endif
    _disbleVolumn->setColor(_colorUI);
    _disbleVolumn->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _disbleVolumn->setVisible(!game->isEnableVolumn());
    _volumn->addChild(_disbleVolumn);
    _volumn->setVisible(!volumnIsEnable);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    _about = ui::Button::create("OptionAssets/about.png");
#else
    _about = ui::Button::create("about.png");
#endif
    _about->setColor(_colorUI);
    _about->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    _about->setPosition(Vec2(origin.x + screenSize.width, origin.y + screenSize.height * 0.8));
    _about->setScale(0.25f);
    _about->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
            GoToAbout();
    });
    this->addChild(_about);
    _about->setVisible(_volumn->isVisible());
    
    //Back btn
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    _back = ui::Button::create("OptionAssets/back.png");
#else
    _back = ui::Button::create("back.png");
#endif
    _back->setColor(_colorUI);
    _back->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _back->setPosition(Vec2(origin.x, origin.y + screenSize.height));
    _back->setScale(0.25f);
    _back->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
            Back();
    });
    this->addChild(_back);
    
    return true;
}

void GS_OptionPage::GoToOption()
{
    _volumn->setVisible(!_volumn->isVisible());
    _about->setVisible(_volumn->isVisible());
}

void GS_OptionPage::modifyVolumn()
{
    bool enableVolumn = Game::getInstance()->isEnableVolumn();
    Game::getInstance()->setEnableVolunm(!enableVolumn);
    _disbleVolumn->setVisible(enableVolumn);
}

void GS_OptionPage::GoToAbout()
{
    
}

void GS_OptionPage::Back()
{
    Layer* currentState = Game::getInstance()->getCurrentState();
    int tag = currentState->getTag();
    if(tag == Game::layer::PICKMAP)
    {
        currentState->setVisible(false);
        Layer* labelMenu = dynamic_cast<Layer*>(this->getScene()->getChildByTag(Game::layer::GAMELABEL));\
        if(labelMenu)
            Game::getInstance()->setCurrentState(labelMenu);
    }
    else if(tag == Game::layer::GAMEPLAY)
    {
        Game::getInstance()->backToHomeMenu();
    }
}

void GS_OptionPage::setColorUI(Color3B color)
{
    _colorUI = color;
    _option->setColor(_colorUI);
    _volumn->setColor(_colorUI);
    _disbleVolumn->setColor(_colorUI);
    _about->setColor(_colorUI);
    _back->setColor(_colorUI);
}
