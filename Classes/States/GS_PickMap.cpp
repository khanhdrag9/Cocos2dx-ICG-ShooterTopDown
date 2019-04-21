#include "GS_PickMap.h"
#include "../Game.h"
#include "../Resource/ResourceManager.h"
#include "GS_GamePlay.h"
#include "GS_GameMenu.h"

using namespace ui;

GS_PickMap::GS_PickMap():
_countMap(0),
_pageView(nullptr)
{
    
}

Scene* GS_PickMap::createScene()
{
    Scene* scene = Scene::create();
    GS_PickMap* layer = GS_PickMap::create();
    scene->addChild(layer);
    
    return scene;
}

bool GS_PickMap::init()
{
    if(!Layer::init())
        return false;
    
    Game::getInstance()->setCurrentState(this);
    
    Size screenSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 center = Vec2(screenSize.width / 2.f + origin.x, screenSize.height / 2.f + origin.y);
    
    auto resMgr = ResourceManager::getInstance();
    auto game = Game::getInstance();
    
    //bg
    auto bg = Sprite::create(resMgr->at(res::define::IMG_HOME_SCREEN_1_NO_TITLE));
    bg->setPosition(center);
    this->addChild(bg, 0);
    
    //init page view map
    Size pageSize = Size(screenSize.width, screenSize.height);
    _pageView = PageView::create();
    _pageView->setDirection(PageView::Direction::HORIZONTAL);
//    pageView->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _pageView->setContentSize(pageSize);
//    pageView->setPosition(Vec2(screenSize.width / 2.f + origin.x, origin.y + screenSize.height / 2.f));
    _pageView->removeAllItems();
    _pageView->setIndicatorEnabled(true);
    //pageView->setGlobalZOrder(200);
    
    auto listmap = game->getGameMaps();
    string fontTitle = resMgr->at(res::define::FONT_KENVECTOR_FUTURE_THIN);
    _countMap = (int)listmap.size();
    for (int i = 0; i < _countMap; ++i)
    {
        Layout* layout = Layout::create();
        layout->setContentSize(pageSize);
        
        ImageView* imageView = ImageView::create(listmap[i].linkImage);
        imageView->setScale9Enabled(true);
        imageView->setContentSize(pageSize);
        imageView->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        imageView->setPosition(Vec2(pageSize.width / 2.f, 0));
        layout->addChild(imageView);
        
//        Label* title = Label::createWithTTF(listmap[i].name, fontTitle, 120);
//        title->setColor(Color3B(102, 51, 51));
////        title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
//        title->setPosition(Vec2(pageSize.width / 2.f + origin.x, pageSize.height * 0.8f + origin.y));
//        layout->addChild(title, 2);
        
        _pageView->insertCustomItem(layout, i);
    }
    
    _pageView->scrollToItem(0);
    _pageView->setIndicatorEnabled(false);
    
    _pageView->setIndicatorIndexNodesOpacity(0);
    this->addChild(_pageView);
    
    //Play btn
    Button* play = Button::create(resMgr->at(res::define::BTN_PLAY));
    play->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED)
        {
            int currentMap = (int)_pageView->getCurrentPageIndex();
            if(currentMap >= _countMap)return;
            
            GoToMap(currentMap);
        }
    });
    play->setPosition(Vec2(center.x, screenSize.height * 0.1f));
    play->setScale(1, 0.8);
    this->addChild(play);
    
    
    auto tileMap = TMXTiledMap::create("Map/Map2.tmx");
    tileMap->setPosition(Vec2(10, 10));
        auto collisionLayer = tileMap->getLayer("Collision");
        collisionLayer->setVisible(false);
//        this->addChild(tileMap);
    
        tileMap->setScale(0.15);
    
    return true;
}

void GS_PickMap::GoToMap(const int& index)
{
    Game::getInstance()->setMap(index);
    Scene* gameplay = GS_GamePlay::createScene();
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    Director::getInstance()->replaceScene(TransitionFade::create(1.f, gameplay));
}
