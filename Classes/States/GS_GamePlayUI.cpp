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

GS_GamePlayUI::GS_GamePlayUI():
_playerBullet(nullptr)
{}

bool GS_GamePlayUI::init()
{
    if(!Layer::init())
        return false;
    
    auto sz = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    //mag properties
    string font = ResourceManager::getInstance()->at(res::define::FONT_ARIAL);
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
    
}

void GS_GamePlayUI::setCharacter(const shared_ptr<Character>& character)
{
    _characterProfile = character;
}
