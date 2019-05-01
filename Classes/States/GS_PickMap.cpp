#include "GS_PickMap.h"
#include "../Game.h"
#include "../Resource/ResourceManager.h"
#include "GS_GameMenu.h"
#include "GS_LoadToGamePlay.h"
#include "GS_OptionPage.h"
#include "../Resource/Creations.h"
#include "../Bot/BotManager.h"

using namespace ui;

GS_PickMap::GS_PickMap():
_countMap(0),
_countCharacter(0),
_countPages(0),
_pageView(nullptr),
_pageViewCharacter(nullptr),
_pageViewGlobal(nullptr),
_nextPageLeft(nullptr),
_nextPageRight(nullptr),
_optionPage(nullptr)
{
    
}

//Scene* GS_PickMap::createScene()
//{
//    Scene* scene = Scene::create();
//    GS_PickMap* layer = GS_PickMap::create();
//    scene->addChild(layer);
//
//    return scene;
//}

bool GS_PickMap::init()
{
	if (!Layer::init())
		return false;

	Game::getInstance()->setCurrentState(this);

    Size screenSize = Director::getInstance()->getVisibleSize();
//    Size screenSize = Size(1024,768);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Vec2 center = Vec2(screenSize.width / 2.f + origin.x, screenSize.height / 2.f + origin.y);

	auto resMgr = ResourceManager::getInstance();
	auto game = Game::getInstance();

	//bg
	auto bg = Sprite::create(resMgr->at(res::define::IMG_HOME_SCREEN_1_NO_TITLE));
	bg->setPosition(center);
	this->addChild(bg, 0);

	/*=============================For PageViews=============================*/
	//init page view map
	Size pageSize = Size(screenSize.width, screenSize.height);
	_pageView = PageView::create();
	_pageView->setDirection(PageView::Direction::VERTICAL);
	_pageView->setContentSize(pageSize);
	_pageView->removeAllItems();
	_pageView->setIndicatorEnabled(false);
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
	_pageView->setIndicatorIndexNodesOpacity(0);
	//this->addChild(_pageView);

	//page for character choose
	Size pageSizeCharacter = pageSize;
	_pageViewCharacter = PageView::create();
	_pageViewCharacter->setDirection(PageView::Direction::VERTICAL);
	_pageViewCharacter->setContentSize(pageSizeCharacter);
	_pageViewCharacter->removeAllItems();
	_pageViewCharacter->setIndicatorEnabled(false);

    auto listChaCreate = resMgr->getListCharacterCreation();
	_countCharacter = (int)listChaCreate.size();

	for (int i = 0; i < _countCharacter; ++i)
	{
		Layout* layout = Layout::create();
		layout->setContentSize(pageSizeCharacter);

		ImageView* imageView = ImageView::create(listChaCreate[i].getImage());
		imageView->setScale9Enabled(true);
		imageView->setContentSize(pageSizeCharacter);
		imageView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		imageView->setPosition(Vec2(pageSize.width / 2.f, pageSize.height / 2.f));
		layout->addChild(imageView);

		_pageViewCharacter->insertCustomItem(layout, i);
	}
	_pageViewCharacter->scrollToItem(0);
	_pageViewCharacter->setIndicatorIndexNodesOpacity(0);
	//this->addChild(_pageViewCharacter);

	//init global pageView ( map + character chooses)
	Size pageSizeGlobal = pageSize;
	_pageViewGlobal = PageView::create();
	_pageViewGlobal->setDirection(PageView::Direction::HORIZONTAL);
	_pageViewGlobal->setContentSize(pageSizeGlobal);
	_pageViewGlobal->removeAllItems();
	_pageViewGlobal->setIndicatorEnabled(false);
	
	PageView* pageChilds[] = { _pageView, _pageViewCharacter };
	for (int i = 0; i < 2; ++i)
	{
		Layout* layout = Layout::create();
		layout->setContentSize(pageSizeGlobal);
		layout->addChild(pageChilds[i], i);

		_pageViewGlobal->insertCustomItem(layout, i);
	}
	_pageViewGlobal->scrollToItem(0);
	_pageViewGlobal->setIndicatorIndexNodesOpacity(0);
	
	this->addChild(_pageViewGlobal);
    

	/*=============================For buttons===================================*/
	//next side page btn
	_countPages = 2;
	Button* btnNextPage[2];
	for (int i = 0; i < 2; i++)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		btnNextPage[i] = Button::create("OptionAssets/nextPage.png");
#else
        btnNextPage[i] = Button::create("nextPage.png");
#endif
		btnNextPage[i]->setScale(0.25f);
		btnNextPage[i]->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType type) {
			if (type == Widget::TouchEventType::ENDED)
			{
				int currentPage = (int)_pageViewGlobal->getCurrentPageIndex();
				if (++currentPage >= TOTAL)currentPage = 0;
				GoToSidePage(currentPage);
			}
		});
		this->addChild(btnNextPage[i]);
	}

	{
		_nextPageLeft = btnNextPage[0];
        _nextPageLeft->setColor(Color3B::BLACK);
		Size left = _nextPageLeft->getBoundingBox().size;
		_nextPageLeft->setPosition(Vec2(origin.x + left.width * 0.4f, origin.y + screenSize.height * 0.15));
		_nextPageLeft->setFlippedX(true);
		_nextPageLeft->setVisible(false);

		_nextPageRight = btnNextPage[1];
        _nextPageRight->setColor(Color3B::WHITE);
		Size right = _nextPageRight->getBoundingBox().size;
		_nextPageRight->setPosition(Vec2(origin.x + screenSize.width - left.width * 0.4f, _nextPageLeft->getPositionY()));
	}
	

    //Play btn
    Button* play = Button::create(resMgr->at(res::define::BTN_PLAY));
    play->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED)
        {
            int currentMap = (int)_pageView->getCurrentPageIndex();
            int playerSelect = (int)_pageViewCharacter->getCurrentPageIndex();
            if(currentMap >= _countMap)return;
            
            GoToMap(currentMap, playerSelect);
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

void GS_PickMap::GoToMap(const int& index, const int& character)
{
    Game::getInstance()->setMap(index);
    Game::getInstance()->setPlayerCreation(character);
    auto listCreations = ResourceManager::getInstance()->getListCharacterCreation();
    vector<CharacterCreation> botCreations{
        listCreations[random(0, (int)listCreations.size() - 1)],
        listCreations[random(0, (int)listCreations.size() - 1)],
        listCreations[random(0, (int)listCreations.size() - 1)],
        listCreations[random(0, (int)listCreations.size() - 1)]
    };
    BotManager::getInstance()->setBotCreations(botCreations);
    
    Scene* gameplay = GS_LoadToGamePlay::createScene();
    this->setVisible(false);
    Director::getInstance()->pushScene(TransitionMoveInL::create(0.5f, gameplay));
}

void GS_PickMap::GoToSidePage(const int& index)
{
	_nextPageLeft->setVisible(true);
	_nextPageRight->setVisible(true);

	if (index == 0)
    {
		_nextPageLeft->setVisible(false);
        _optionPage->setColorUI(Color3B::WHITE);
        
    }
	else if (index == _countPages - 1)
    {
		_nextPageRight->setVisible(false);
        _optionPage->setColorUI(Color3B::BLACK);
    }

	_pageViewGlobal->scrollToItem(index);
}
