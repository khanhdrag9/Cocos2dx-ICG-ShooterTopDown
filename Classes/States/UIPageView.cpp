#include "UIPageView.h"

using namespace ui;

UIPageView::UIPageView()
{
}

UIPageView::~UIPageView()
{
}

bool UIPageView::init()
{
	if (!Layer::init())
		return false;

	auto director = Director::getInstance();
	Size screenSize = director->getVisibleSize();
	Vec2 origin = director->getVisibleOrigin();

	Size pageSize = screenSize / 1.5;
	PageView* pageView = PageView::create();
	pageView->setDirection(PageView::Direction::HORIZONTAL);
	pageView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pageView->setContentSize(pageSize);
	pageView->setPosition(Vec2(screenSize.width / 2.f + origin.x, screenSize.height / 2.f + origin.y));
	pageView->removeAllItems();
	pageView->setIndicatorEnabled(true);
	//pageView->setGlobalZOrder(200);

	int mapCount = 4;
	for (int i = 0; i < mapCount; ++i)
	{
		Layout* layout = Layout::create();
		layout->setContentSize(pageSize / 2.f);

		ImageView* imageView = ImageView::create("splashKW.jpg");
		imageView->setScale9Enabled(true);
		imageView->setContentSize(pageSize);
		imageView->setPosition(Vec2(pageSize.width / 2.f, pageSize.height / 2.f));
		layout->addChild(imageView);

		pageView->insertCustomItem(layout, i);
	}

	pageView->scrollToItem(0);
    pageView->setIndicatorEnabled(false);

	pageView->setIndicatorIndexNodesOpacity(0);
	this->addChild(pageView);

	return true;
}
