//
//  GS_GamePlayUI.cpp
//  ShooterTopDown2D
//
//  Created by Khanh on 4/14/19.
//

#include "GS_GamePlayUI.h"
#include "Game.h"
#include "../Resource/ResourceManager.h"
#include "../Characters/Player.h"
#include "../Bot/Bot.h"
#include "../Bot/BotManager.h"

GS_GamePlayUI::GS_GamePlayUI():
_playerBullet(nullptr),
_menuKDA(nullptr)
{}

GS_GamePlayUI::~GS_GamePlayUI()
{
    _listKDA.clear();
}

bool GS_GamePlayUI::init()
{
    if(!Layer::init())
        return false;
    
    auto sz = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    //mag properties
    auto resMgr = ResourceManager::getInstance();
    
    string font = resMgr->at(res::define::FONT_ARIAL);
    _playerBullet = Label::createWithTTF("0/0", font , 50);
    _playerBullet->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _playerBullet->setPosition(origin.x + sz.width / 2.f, origin.y);
    
    this->addChild(_playerBullet);
    
    //player properties
    _propertiesPlayer = Label::createWithTTF("0%", font, 30);
    _propertiesPlayer->setColor(Color3B::RED);
    _propertiesPlayer->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    _propertiesPlayer->setPosition(origin.x + sz.width - 10, origin.y + 10);
    
    this->addChild(_propertiesPlayer);
    
    this->scheduleUpdate();
    
    //set player profile
    setCharacter(Game::getInstance()->getPlayer());
    
    //KDA
    _kdaTab = Sprite::create(resMgr->at(res::define::IMG_BLACK));
    _kdaTab->setPosition(sz.width / 2.f + origin.x, sz.height / 2.f + origin.y);
    _kdaTab->setScale(0.35, 0.2);
    _kdaTab->runAction(FadeTo::create(0, 0));
    this->addChild(_kdaTab);
    
    initKDA(BotManager::getInstance()->countBots() + 1);
    
    return true;
}

void GS_GamePlayUI::update(float)
{
    //if(auto character = _characterProfile.lock())
    if(auto character = _characterProfile)
    {
        _playerBullet->setVisible(true);
        _propertiesPlayer->setVisible(true);
        
        //for Mag bullet
        int maxbullet = 0;
        int currentbullet = 0;
        
        if(auto player = dynamic_pointer_cast<Player>(character))
        {
            auto& mag = player->getMag();
            maxbullet = mag->getMaxBullet();
            currentbullet = mag->getCurrentBullet();
        }
        else if(auto bot = dynamic_pointer_cast<Bot>(character))
        {
            auto& mag = bot->getMag();
            maxbullet = mag->getMaxBullet();
            currentbullet = mag->getCurrentBullet();
        }
        
        string formatMag = to_string(currentbullet) + "/" + to_string(maxbullet);
        _playerBullet->setString(formatMag);
        
        //for HP
        int currentHP = character->getCurrentHP();
        string formatHP = "HP:" + to_string(currentHP) + "%";
        _propertiesPlayer->setString(formatHP);
        
    }
    else
    {
        _playerBullet->setVisible(false);
        _propertiesPlayer->setVisible(false);
    }
    
    //update KDA
    if(_menuKDA)
        _menuKDA->setOpacity(_kdaTab->getOpacity());
}

void GS_GamePlayUI::setCharacter(const shared_ptr<Character>& character)
{
    _characterProfile = character;
}

void GS_GamePlayUI::initKDA(const int& number)
{
    _listKDA.clear();
    if(_menuKDA)_menuKDA->removeAllChildrenWithCleanup(true);
    
    auto resMgr = ResourceManager::getInstance();
    string font = resMgr->at(res::define::FONT_ARIAL);
    
    //title
    string formatTile = "Name       K       D";
    Label* tile = Label::createWithTTF(formatTile, font, 150);
    MenuItem* itemTile = MenuItemLabel::create(tile);
    _listKDA.pushBack(itemTile);
    
    for(int i = 0; i < number; i++)
    {
        string format = "Name       0       0";
        Label* label = Label::createWithTTF(format, font, 150);
        MenuItem* item = MenuItemLabel::create(label);
        _listKDA.pushBack(item);
    }
    
    _menuKDA = Menu::createWithArray(_listKDA);
    _menuKDA->alignItemsVerticallyWithPadding(40);
    _menuKDA->setOpacity(_kdaTab->getOpacity());
    _menuKDA->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    
    _kdaTab->addChild(_menuKDA);
    auto sizeTab = _kdaTab->getContentSize();
    _menuKDA->setPosition(sizeTab.width / 2.f, sizeTab.height / 2.f);
}

void GS_GamePlayUI::useKDATab(bool show)
{
    Action* action;
    if(show)
        action = FadeTo::create(0.5f, 150);
    else
        action = FadeTo::create(0.5f, 0);
    
    if(_kdaTab)
        _kdaTab->runAction(action);
}
