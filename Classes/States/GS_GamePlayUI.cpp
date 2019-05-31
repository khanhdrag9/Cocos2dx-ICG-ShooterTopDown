//
//  GS_GamePlayUI.cpp
//  ShooterTopDown2D
//
//  Created by Khanh on 4/14/19.
//

#include "GS_GamePlayUI.h"
#include "GS_GameResult.h"
#include "Game.h"
#include "../Resource/ResourceManager.h"
#include "../Characters/Player.h"
#include "../Bot/Bot.h"
#include "../Bot/BotManager.h"
#include "GS_OptionPage.h"
#include "Joystick.h"

GS_GamePlayUI::GS_GamePlayUI():
_playerBullet(nullptr),
//_menuKDA(nullptr),
_optionPage(nullptr)
{}

GS_GamePlayUI::~GS_GamePlayUI()
{
   
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
    _propertiesPlayer = Label::createWithTTF("0", font, 30);
    _propertiesPlayer->setColor(Color3B::RED);
    _propertiesPlayer->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    _propertiesPlayer->setPosition(origin.x + sz.width - 10, origin.y + 10);
    
    this->addChild(_propertiesPlayer);
    
    this->scheduleUpdate();
    
    //set player profile
    setCharacter(Game::getInstance()->getPlayer());
    
    //KDA
//    _kdaTab = Sprite::create(resMgr->at(res::define::IMG_BLACK));
//    _kdaTab->setPosition(sz.width / 2.f + origin.x, sz.height / 2.f + origin.y);
//    _kdaTab->setScale(0.35, 0.2);
//    _kdaTab->runAction(FadeTo::create(0, 0));
//    this->addChild(_kdaTab);
    
    initKDA(BotManager::getInstance()->countBots() + 1);
    
    //option
    _optionPage = GS_OptionPage::create();
    _optionPage->setColorUI(Color3B::WHITE);
    _optionPage->setOpacity(0);
    _optionPage->setBarVisible(true);
    _optionPage->setPopupExitColor(Color3B(7, 71, 45), Color4B::WHITE);
    this->addChild(_optionPage, Game::layer::OPTION);
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();

    //joystick
#if USE_JOYSTICK
    _joystick = Joystick::create();
    this->addChild(_joystick, 50);
#endif
    
    //Time
    _coutTime = TOTAL_TIME * 60;
    _time = Label::createWithTTF(std::to_string(_coutTime), font, 40);
    _time->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _time->setPosition(Vec2(origin.x + sz.width / 2, origin.y + sz.height));
    _time->setTextColor(Color4B::YELLOW);
    this->addChild(_time);
    
#if CHEAT
    string F1 = "F1: jump to revival position\n";
    string F2 = "F2: destroy visions\n";
    string F3 = "F3: kill all enemy\n";
    string F4 = "F4: enable/disible light of vision\n";
    string F5 = "F5: change camera to anthor character\n";
    string F6 = "F6: kill player\n";
    string F7 = "F7: player becomes un/immortal\n";
    string text = F1 + F2 + F3 + F4 + F5 + F6 + F7;
    
    Label* cheat = Label::createWithTTF(text.c_str(), font, 20);
    cheat->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    cheat->setPosition(origin.x, origin.y + sz.height);
    cheat->setColor(Color3B::RED);
    this->addChild(cheat, 10);
#endif
    
    return true;
}

string convertSecondToMinutes(int second){
    int m = second / 60;
    int s = second % 60;
    return std::to_string(m) + ":" + std::to_string(s);
}

void GS_GamePlayUI::update(float dt)
{
    //update time
    
    if(_coutTime > 0)_coutTime -= dt;
    _time->setString(convertSecondToMinutes(_coutTime));
    //if(auto character = _characterProfile.lock())
    if(auto character = _characterProfile)
    {
		if (!character->isDestroyed())
		{
			_playerBullet->setVisible(true);
			_propertiesPlayer->setVisible(true);

			//for Mag bullet
			int maxbullet = 0;
			int currentbullet = 0;
            bool isReloading = false;

			if (auto player = dynamic_pointer_cast<Player>(character))
			{
				auto& mag = player->getMag();
				maxbullet = mag->getMaxBullet();
				currentbullet = mag->getCurrentBullet();
                isReloading = mag->isReloading();
			}
			else if (auto bot = dynamic_pointer_cast<Bot>(character))
			{
				auto& mag = bot->getMag();
				maxbullet = mag->getMaxBullet();
				currentbullet = mag->getCurrentBullet();
                isReloading = mag->isReloading();
			}

            if(!isReloading)
            {
                string formatMag = to_string(currentbullet) + "/" + to_string(maxbullet);
                _playerBullet->setString(formatMag);
                _playerBullet->setColor(Color3B::YELLOW);
            }
            else
            {
                _playerBullet->setString("Reloading....");
                _playerBullet->setColor(Color3B::YELLOW);
            }
			//for HP
			int currentHP = character->getCurrentHP();
			string formatHP = "HP:" + to_string(currentHP);
			_propertiesPlayer->setString(formatHP);
		}
        else
            _characterProfile = nullptr;
    }
    else
    {
        _playerBullet->setVisible(false);
        _propertiesPlayer->setVisible(false);
    }
    
    //update KDA
//    if(_menuKDA)
//        _menuKDA->setOpacity(_kdaTab->getOpacity());
    
    if(_optionPage)
    {
        _optionPage->setVisible(true);
        if(_optionPage->getOpacity() == 0)_optionPage->setVisible(false);
    }
}

void GS_GamePlayUI::setCharacter(const shared_ptr<Character>& character)
{
    _characterProfile = character;
    for(auto& btn : _skillsCharacter)
    {
        if(btn)
            btn->removeFromParentAndCleanup(true);
    }
    _skillsCharacter.clear();
    
#if USE_JOYSTICK
    auto sz = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
//    auto button = ui::Button::create(_characterProfile->getBullet()->getBulletImage());
//    button->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
//    button->setScale(0.5f);
//    button->setOpacity(225);
//    button->setPosition(Vec2(origin.x + sz.width * 0.7f, origin.y + sz.height * 0.1f));
//    button->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType type){
//        if(type == ui::Widget::TouchEventType::BEGAN || type == ui::Widget::TouchEventType::MOVED)
//        {
////            Game::getInstance()->handleShootCharacter(_characterProfile, _characterProfile->getBullet()->getSpeed());
//            Game::getInstance()->setShootOfPlayer(true);
//        }
//        else
//            Game::getInstance()->setShootOfPlayer(false);
//    });
//    this->addChild(button);
//    _skillsCharacter.push_back(button);
    
    auto reload = ui::Button::create(ResourceManager::getInstance()->at(res::define::BTN_RELOAD), "", "", ui::TextureResType::PLIST);
    reload->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    reload->setScale(0.4f);
//    reload->setOpacity(225);
    reload->setPosition(Vec2(origin.x + sz.width * 0.95, origin.y + Joystick::radius * 2 + Joystick::offset_y * 2));
    reload->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::BEGAN)
        {
            Game::getInstance()->getPlayer()->getMag()->reload();
        }
    });
    this->addChild(reload);
    _skillsCharacter.push_back(reload);
#endif
}

void GS_GamePlayUI::initKDA(const int& number)
{
//    _listKDA.clear();
//    if(_menuKDA)_menuKDA->removeAllChildrenWithCleanup(true);
//
//    auto resMgr = ResourceManager::getInstance();
//    string font = resMgr->at(res::define::FONT_ARIAL);
//
//    //title
//    string formatTile = "Name       K       D";
//    Label* tile = Label::createWithTTF(formatTile, font, 150);
//    MenuItem* itemTile = MenuItemLabel::create(tile);
//    _listKDA.pushBack(itemTile);
//
//    for(int i = 0; i < number; i++)
//    {
//        string format = "Name       0       0";
//        Label* label = Label::createWithTTF(format, font, 150);
//        MenuItem* item = MenuItemLabel::create(label);
//        _listKDA.pushBack(item);
//    }
//
//    _menuKDA = Menu::createWithArray(_listKDA);
//    _menuKDA->alignItemsVerticallyWithPadding(40);
//    _menuKDA->setOpacity(_kdaTab->getOpacity());
//    _menuKDA->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
//
//    _kdaTab->addChild(_menuKDA);
//    auto sizeTab = _kdaTab->getContentSize();
//    _menuKDA->setPosition(sizeTab.width / 2.f, sizeTab.height / 2.f);
}

void GS_GamePlayUI::useTab(bool show)
{
    Action* action;
    if(show)
        action = FadeTo::create(0.25f, 150);
    else
        action = FadeTo::create(0.25f, 0);
    
//    if(_kdaTab)
//        _kdaTab->runAction(action->clone());
    if(_optionPage)
    {
        _optionPage->setVisible(true);
        _optionPage->runAction(action->clone());
    }
}

void GS_GamePlayUI::showResult()
{
    GS_GameResult* gameresult = GS_GameResult::create();
    useTab(false);
    this->getScene()->addChild(gameresult, 10);
}

void GS_GamePlayUI::clear()
{
//    _listKDA.clear();
	_characterProfile = nullptr;
    _skillsCharacter.clear();
}

