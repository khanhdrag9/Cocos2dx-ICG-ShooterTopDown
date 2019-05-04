#include "GS_GameMenu.h"
#include "../Game.h"
#include "../Resource/ResourceManager.h"
#include "GS_GamePlay.h"
#include "GS_OptionPage.h"
#include "GS_PickMap.h"
#include "GS_OptionPage.h"
#include "CustomMouse.h"

GS_GameMenu::GS_GameMenu():
    _optionPage(nullptr),
    _pickMapPage(nullptr)
{}

cocos2d::Scene *GS_GameMenu::createScene()
{
	Scene* scene = Scene::create();
	GS_GameMenu* layer = GS_GameMenu::create();
    layer->setTag(Game::layer::GAMELABEL);
    layer->_optionPage = GS_OptionPage::create();
    layer->_optionPage->setTag(Game::layer::OPTION);
    layer->_optionPage->setPopupExitColor(Color3B(8, 70, 90), Color4B::WHITE);
    scene->addChild(layer, (int)Game::layer::GAMELABEL);
    scene->addChild(layer->_optionPage, (int)Game::layer::OPTION);
    scene->addChild(CustomMouse::create(), 100);

//    UIPageView* pageView = UIPageView::create();
    //scene->addChild(pageView, 1);

    return scene;
}


bool GS_GameMenu::init()
{
    if(!Layer::init())
        return false;
    
    Game::getInstance()->setCurrentState(this);
    
    Size screenSize = Director::getInstance()->getVisibleSize();
    CCLOG("khanh getVisibleSize %f - %f", screenSize.width, screenSize.height);
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    CCLOG("khanh getVisibleOrigin %f - %f", origin.x, origin.y);
    Vec2 center = Vec2(screenSize.width / 2.f + origin.x, screenSize.height / 2.f + origin.y);
            
    auto resMgr = ResourceManager::getInstance();
    auto game = Game::getInstance();
    
    //background
    auto bg = Sprite::create(resMgr->at(res::define::IMG_HOME_SCREEN_1));
    bg->setPosition(center);
    this->addChild(bg, 0);
    
    //touch screen text
    auto label = Label::createWithTTF("Touch on screen to start game", resMgr->at(res::define::FONT_KENVECTOR_FUTURE_THIN), 30);
    label->setPosition(center.x, center.y * 0.6);
    this->addChild(label);
    
    auto actionLabel = Sequence::createWithTwoActions(FadeIn::create(1.f), FadeOut::create(0.5f));
    label->runAction(RepeatForever::create(actionLabel));
    
    //touch
    auto touchlistner = EventListenerTouchOneByOne::create();
    touchlistner->onTouchBegan = [this](Touch* touch, Event*){ return true;};
    touchlistner->onTouchEnded = [this](Touch* touch, Event*)
    {
        if(!_optionPage->getPopupVisible())
            this->GoToPickMap();
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistner, this);


    return true;
}

void GS_GameMenu::GoToPickMap()
{
        if(_pickMapPage)
        {
            _pickMapPage->setVisible(true);
            if(_pickMapPage->getIndexPageViewGlobal() == 0)
                _optionPage->setColorUI(Color3B::WHITE);
            else
                _optionPage->setColorUI(Color3B::BLACK);
            Game::getInstance()->setCurrentState(_pickMapPage);
        }
        else
        {
            _pickMapPage = GS_PickMap::create();
            _pickMapPage->setTag(Game::layer::PICKMAP);
            _pickMapPage->_optionPage = _optionPage;
            _optionPage->setColorUI(Color3B::WHITE);
            this->addChild(_pickMapPage, Game::layer::PICKMAP);
        }
}
