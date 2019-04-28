#include "GS_GameMenu.h"
#include "../Game.h"
#include "../Resource/ResourceManager.h"
#include "GS_GamePlay.h"
#include "GS_OptionPage.h"
#include "GS_PickMap.h"
#include "GS_OptionPage.h"

GS_GameMenu::GS_GameMenu():
    _optionPage(nullptr),
    _pickMapPage(nullptr)
{}

cocos2d::Scene *GS_GameMenu::createScene()
{
	Scene* scene = Scene::create();
	GS_GameMenu* layer = GS_GameMenu::create();
    layer->setTag(layer::GAMELABEL);
    layer->_optionPage = GS_OptionPage::create();
    layer->_optionPage->setTag(layer::OPTION);
    scene->addChild(layer, (int)layer::GAMELABEL);
    scene->addChild(layer->_optionPage, (int)layer::OPTION);

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
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
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
        this->GoToPickMap();
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistner, this);


    return true;
}

void GS_GameMenu::GoToGamePlay()
{
    Scene* gameplay = GS_GamePlay::createScene();
    Director::getInstance()->pushScene(TransitionFade::create(0.5, gameplay));
}

void GS_GameMenu::GoToPickMap()
{
        if(_pickMapPage)
        {
            _pickMapPage->setVisible(true);
            Game::getInstance()->setCurrentState(_pickMapPage);
        }
        else
        {
            _pickMapPage = GS_PickMap::create();
            _pickMapPage->setTag(layer::PICKMAP);
            _pickMapPage->_optionPage = _optionPage;
            _optionPage->setColorUI(Color3B::WHITE);
            this->addChild(_pickMapPage, layer::PICKMAP);
        }
}
