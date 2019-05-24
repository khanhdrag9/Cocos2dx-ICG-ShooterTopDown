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
#include "../Characters/Player.h"
#include "PopupInGame.h"

GS_OptionPage::~GS_OptionPage()
{
}

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
//    _option = ui::Button::create("OptionAssets/optionFill.png");
    _option = ui::Button::create("optionFill.png", "", "", ui::TextureResType::PLIST);
    _option->setColor(_colorUI);
    _option->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    _option->setPosition(Vec2(origin.x + screenSize.width, origin.y + screenSize.height));
    _option->setScale(0.25);
    _option->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
            GoToOption();
    });
    this->addChild(_option, 3);
    
    bool volumnIsEnable = game->isEnableVolumn();
    game->setEnableVolunm(volumnIsEnable);
//    _volumn = ui::Button::create("OptionAssets/volumnOn.png");
    _volumn = ui::Button::create("volumnOn.png", "", "", ui::TextureResType::PLIST);
    _volumn->setColor(_colorUI);
    _volumn->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    _volumn->setPosition(Vec2(origin.x + screenSize.width, origin.y + screenSize.height * 0.9));
    _volumn->setScale(0.25);
    _volumn->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
            this->modifyVolumn();
    });
    this->addChild(_volumn, 3);

//    _disbleVolumn = Sprite::create("OptionAssets/volumnOff.png");
    _disbleVolumn = Sprite::createWithSpriteFrameName("volumnOff.png");
    _disbleVolumn->setColor(_colorUI);
    _disbleVolumn->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _disbleVolumn->setVisible(!game->isEnableVolumn());
    _volumn->addChild(_disbleVolumn);
    _volumn->setVisible(!volumnIsEnable);

//    _about = ui::Button::create("OptionAssets/about.png");
    _about = ui::Button::create("about.png", "", "", ui::TextureResType::PLIST);
    _about->setColor(_colorUI);
    _about->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    _about->setPosition(Vec2(origin.x + screenSize.width, origin.y + screenSize.height * 0.8));
    _about->setScale(0.25f);
    _about->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
            GoToAbout();
    });
    this->addChild(_about, 3);
    _about->setVisible(_volumn->isVisible());
    
    //Back btn
//    _back = ui::Button::create("OptionAssets/back.png");
    _back = ui::Button::create("back.png", "", "", ui::TextureResType::PLIST);
    _back->setColor(_colorUI);
    _back->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _back->setPosition(Vec2(origin.x, origin.y + screenSize.height));
    _back->setScale(0.25f);
    _back->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
            Back();
    });
    this->addChild(_back, 3);
    
    //Bar
//    _bar = Sprite::create("OptionAssets/Bar.png");
    _bar = Sprite::createWithSpriteFrameName("Bar.png");
    _bar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _bar->setPosition(center.x, origin.y + screenSize.height);
    _bar->setScale(1, 0.5);
    _bar->setOpacity(225);
    _bar->setColor(Color3B(7, 71, 45));
    _bar->setVisible(false);
    _bar->setContentSize(Size(screenSize.width, _bar->getContentSize().height));
    this->addChild(_bar, 0);
    
    //Popup exit
//    _popupExit = Sprite::create("OptionAssets/Popup.png");
    _popupExit = Sprite::createWithSpriteFrameName("Popup.png");
    _popupExit->setPosition(center.x, origin.y + screenSize.height * 0.6f);
    _popupExit->setOpacity(235);
    _popupExit->setVisible(false);
    this->addChild(_popupExit);
    
    string font = resMgr->at(res::define::FONT_KENVECTOR_FUTURE_THIN);
    Size popupSize = _popupExit->getContentSize();
    _contentExit = Label::createWithTTF("", font, 30);
    _contentExit->setPosition(popupSize.width * 0.5f, popupSize.height * 0.6);
    _popupExit->addChild(_contentExit, 1);
    
    ui::Button* okBtn = ui::Button::create(resMgr->at(res::define::BTN), "", "", ui::TextureResType::PLIST);
    okBtn->setPosition(Vec2(popupSize.width * 0.95, 0));
    okBtn->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    okBtn->setScale(0.6);
    okBtn->addTouchEventListener([](Ref*, ui::Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            auto tagCurrentState = Game::getInstance()->getCurrentState()->getTag();
            if(tagCurrentState == Game::layer::GAMEPLAY)
            {
                if(Game::getInstance()->getPlayer())
                    Game::getInstance()->getPlayer()->destroy();
            }
            else if(tagCurrentState == Game::layer::GAMELABEL)
            {
                Director::getInstance()->end();
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                exit(0);
    #endif
            }
        }
    });
    okBtn->setTitleText("YES");
    okBtn->setTitleFontName(font);
    okBtn->setTitleFontSize(60);
    okBtn->setColor(Color3B(146, 153, 163));
    _popupExit->addChild(okBtn);
    
    ui::Button* cancelBtn = ui::Button::create(resMgr->at(res::define::BTN), "", "", ui::TextureResType::PLIST);
    cancelBtn->setPosition(Vec2(popupSize.width * 0.05, 0));
    cancelBtn->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    cancelBtn->setScale(0.6);
    cancelBtn->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            this->_popupVisible = false;
            this->_popupExit->setVisible(_popupVisible);
        }
    });
    cancelBtn->setTitleText("NO");
    cancelBtn->setTitleFontName(font);
    cancelBtn->setTitleFontSize(60);
    _popupExit->addChild(cancelBtn);
    
    _popupVisible = false;
    
    return true;
}

void GS_OptionPage::resume()
{
    Layer::resume();
    
    Game* game = Game::getInstance();
    bool volumnIsEnable = game->isEnableVolumn();
    game->setEnableVolunm(volumnIsEnable);
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
    Game* game = Game::getInstance();
    Layer* currentState = game->getCurrentState();
    int tag = currentState->getTag();
    if(tag == Game::layer::PICKMAP)
    {
        currentState->setVisible(false);
        Layer* labelMenu = dynamic_cast<Layer*>(this->getScene()->getChildByTag(Game::layer::GAMELABEL));\
        if(labelMenu)
            game->setCurrentState(labelMenu);
        this->setColorUI(Color3B::BLACK);
    }
    else if(tag == Game::layer::GAMEPLAY)
    {
//        game->backToHomeMenu();
//        if(game->isEnableVolumn())
//            game->setEnableVolunm(true);
//        game->getPlayer()->destroy();
        _contentExit->setString("You will lose if you leave now!\nAre you sure?");
        _popupVisible = true;
        _popupExit->setVisible(_popupVisible);
    }
    else if(tag == Game::layer::GAMELABEL)
    {
        _contentExit->setString("Are you sure you want to exit?");
        _popupVisible = true;
        _popupExit->setVisible(_popupVisible);
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

void GS_OptionPage::setBarVisible(bool visible, Color3B color)
{
    _bar->setVisible(visible);
    _bar->setColor(color);
}

void GS_OptionPage::setBarOpacity(float value)
{
    _bar->setOpacity(value);
}

void GS_OptionPage::setPopupExitColor(Color3B color, Color4B textColor)
{
    _popupExit->setColor(color);
    _contentExit->setTextColor(textColor);
}
