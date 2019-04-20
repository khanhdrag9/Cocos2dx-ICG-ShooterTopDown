#include "GS_PickMap.h"
#include "../Game.h"
#include "../Resource/ResourceManager.h"
#include "GS_GamePlay.h"
#include "GS_GameMenu.h"

using namespace ui;

GS_PickMap::GS_PickMap()
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
    PageView* pageView = PageView::create();
    pageView->setDirection(PageView::Direction::HORIZONTAL);
//    pageView->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    pageView->setContentSize(pageSize);
//    pageView->setPosition(Vec2(screenSize.width / 2.f + origin.x, origin.y + screenSize.height / 2.f));
    pageView->removeAllItems();
    pageView->setIndicatorEnabled(true);
    //pageView->setGlobalZOrder(200);
    
    auto listmap = game->getGameMaps();
    string fontTitle = resMgr->at(res::define::FONT_KENVECTOR_FUTURE_THIN);
    
    for (int i = 0; i < listmap.size(); ++i)
    {
        Layout* layout = Layout::create();
        layout->setContentSize(pageSize);
        
        ImageView* imageView = ImageView::create(listmap[i].linkImage);
        imageView->setScale9Enabled(true);
        imageView->setContentSize(pageSize);
        imageView->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        imageView->setPosition(Vec2(pageSize.width / 2.f, 0 + pageSize.height * 0.025f));
        layout->addChild(imageView);
        
        Label* title = Label::createWithTTF(listmap[i].name, fontTitle, 120);
        title->setColor(Color3B(102, 51, 51));
//        title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        title->setPosition(Vec2(pageSize.width / 2.f + origin.x, pageSize.height * 0.8f + origin.y));
        layout->addChild(title, 2);
        
        pageView->insertCustomItem(layout, i);
    }
    
    pageView->scrollToItem(0);
    pageView->setIndicatorEnabled(false);
    
    pageView->setIndicatorIndexNodesOpacity(0);
    this->addChild(pageView);
    
    return true;
}
