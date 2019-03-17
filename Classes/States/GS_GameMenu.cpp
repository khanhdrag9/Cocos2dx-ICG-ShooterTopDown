#include "GS_GameMenu.h"
#include "../Game.h"
#include "../Resource/ResourceManager.h"
#include "GS_GamePlay.h"

cocos2d::Scene *GS_GameMenu::createScene()
{
	Scene* scene = Scene::create();
    GS_GameMenu* layer = GS_GameMenu::create();
    scene->addChild(layer);
    return scene;
}


bool GS_GameMenu::init()
{
    if(!Layer::init())
        return false;
    
    Game::getInstance()->setCurrentState(this);
    
    Size screenSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto resMgr = ResourceManager::getInstance();
    
    Vec2 pos[]{
        Vec2(screenSize.width / 2.f, screenSize.height * 0.8),
        Vec2(screenSize.width / 2.f, screenSize.height * 0.6),
        Vec2(screenSize.width / 2.f, screenSize.height * 0.4),
        Vec2(screenSize.width / 2.f, screenSize.height * 0.2)
    };
    //create game label
    
    
    
    //create UI button
    pair<string, string> assetsBtn[]{
        pair<string, string>(resMgr->at(res::define::BTN_GREEN), resMgr->at(res::define::BTN_RED)),
        pair<string, string>(resMgr->at(res::define::BTN_YELLOW), resMgr->at(res::define::BTN_RED)),
    };
    
    vector<string> titleBtn{
        "Start",
        "Option"
    };

    vector<ui::Button*> listBtns;
    int numberbutton = (int)titleBtn.size();
    for(int i = 0; i < numberbutton; i++)
    {
    
        ui::Button* btn = ui::Button::create(assetsBtn[i].first, assetsBtn[i].second);
        btn->setScale(1.5f);
        btn->setTitleText(titleBtn[i]);
        btn->setTitleColor(Color3B::BLACK);
        btn->setTitleFontSize(25);
        btn->setTitleFontName(resMgr->at(res::define::FONT_KENVECTOR_FUTURE_THIN));
        btn->setPosition(pos[i+1]);
        listBtns.push_back(btn);
        
        this->addChild(btn);
    }
    
    //init callback to btn
    listBtns[0]->addTouchEventListener([&](Ref*, ui::Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
            GoToGamePlay();
    });
    
    listBtns[1]->addTouchEventListener([&](Ref*, ui::Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
            GoToOption();
    });
    
    return true;
}

void GS_GameMenu::GoToGamePlay()
{
    Scene* gameplay = GS_GamePlay::createScene();
    Director::getInstance()->pushScene(TransitionFade::create(0.5, gameplay));
}

void GS_GameMenu::GoToOption()
{
    
}
