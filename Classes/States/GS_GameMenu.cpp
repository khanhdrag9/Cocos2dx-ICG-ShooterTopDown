#include "GS_GameMenu.h"
#include "../Game.h"
#include "../Resource/ResourceManager.h"
#include "GS_GamePlay.h"
#include "UIPageView.h"
#include "GS_PickMap.h"

cocos2d::Scene *GS_GameMenu::createScene()
{
	Scene* scene = Scene::create();
	GS_GameMenu* layer = GS_GameMenu::create();
    scene->addChild(layer, 2);

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
    
    //option
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	_option = ui::Button::create("OptionAssets/optionFill.png");
#else
    _option = ui::Button::create("optionFill.png");
#endif
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
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	_volumn = ui::Button::create("OptionAssets/volumnOn.png");
#else
    _volumn = ui::Button::create("volumnOn.png");
#endif
    _volumn->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    _volumn->setPosition(Vec2(origin.x + screenSize.width, origin.y + screenSize.height * 0.9));
    _volumn->setScale(0.25);
    _volumn->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
            this->modifyVolumn();
    });
    this->addChild(_volumn);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	_disbleVolumn = Sprite::create("OptionAssets/volumnOff.png");
#else
    _disbleVolumn = Sprite::create("volumnOff.png");
#endif
    _disbleVolumn->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _disbleVolumn->setVisible(!game->isEnableVolumn());
    _volumn->addChild(_disbleVolumn);
    _volumn->setVisible(!volumnIsEnable);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	_about = ui::Button::create("OptionAssets/about.png");
#else
    _about = ui::Button::create("about.png");
#endif
    _about->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    _about->setPosition(Vec2(origin.x + screenSize.width, origin.y + screenSize.height * 0.8));
    _about->setScale(0.25f);
    _about->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
            GoToAbout();
    });
    this->addChild(_about);
    _about->setVisible(_volumn->isVisible());
    
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
    auto pickmap = GS_PickMap::createScene();
    Director::getInstance()->pushScene(TransitionFadeBL::create(1.0f, pickmap));
}

void GS_GameMenu::GoToOption()
{
    _volumn->setVisible(!_volumn->isVisible());
    _about->setVisible(_volumn->isVisible());
}

void GS_GameMenu::modifyVolumn()
{
    bool enableVolumn = Game::getInstance()->isEnableVolumn();
    Game::getInstance()->setEnableVolunm(!enableVolumn);
    _disbleVolumn->setVisible(enableVolumn);
}

void GS_GameMenu::GoToAbout()
{
    
}
