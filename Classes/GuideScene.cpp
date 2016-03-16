#include "GuideScene.h"
#include "FindDeviceScene.h"
#include "UIPageViewStandard.h"

using namespace ui;
GuideScene::GuideScene()
{
	exitTime = 0;
}
GuideScene::~GuideScene()
{

}
bool GuideScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto layer = CSLoader::createNode("guide_layer.csb");
	auto layout = layer->getChildByName("layout_bg");
	auto layoutSize = layout->getContentSize();
	auto pageView = ui::PageViewStandard::create();
	pageView->setContentSize(layoutSize);
	pageView->setClippingEnabled(true);
	pageView->setTouchListenerDirection(ui::PageViewStandard::TouchListenerDirection::VERTICAL);
	pageView->setCustomScrollThreshold(30.f);
	layout->addChild(pageView);

	const int maxPage = 3;
	for (int i = 0; i < maxPage; i++)
	{
		Layout* layout = Layout::create();
		layout->setContentSize(Size(layoutSize.width, layoutSize.height));
		layout->setBackGroundImage(StringUtils::format("img_launch_%d.png", i));
		if (maxPage - 1 == i)
		{
			//动画
			Sprite *sp = Sprite::create("img_launch_2_anim.png");
			sp->setPosition(Vec2(layoutSize.width/2,layoutSize.height/2 + 50));
			float rotation = 4.0f;
			auto action = RotateBy::create(0.1f, rotation);
			auto action2 = Sequence::createWithTwoActions(action, action->reverse());
			auto delayTime = DelayTime::create(1.0f);
			auto seq = Sequence::create(action2, action2, action2, delayTime, NULL);
			sp->runAction(RepeatForever::create(seq));
			layout->addChild(sp);

			//立即体验
			Button *button = Button::create("button_guide_normal.png", "button_press_guide.png", "");
			button->setPosition(Vec2(layoutSize.width / 2, layoutSize.height / 6));
			button->setTouchEnabled(true);
			button->addClickEventListener([&](Ref *ref)
			{
				UserDefault::getInstance()->setBoolForKey(USER_DEFAULT_GUIDE_KEY, false);
				Director::getInstance()->replaceScene(FindDeviceScene::createScene());

			});
			layout->addChild(button);
		}


		pageView->addPage(layout);
	}

	addChild(layer);


	//two times exit
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = [&](EventKeyboard::KeyCode code, Event *event){
		if (code == EventKeyboard::KeyCode::KEY_ESCAPE)
		{
			if ((millisecondNow() - exitTime) > 2000)
			{
				exitTime = millisecondNow();
				XMLParser xmlParser;
				if (xmlParser.parseWithFile("string/string.xml"))
				{
					log("ToastShow(...)");
					ToastShow(xmlParser["exit_tip"], 0.5f, -90.0f);
				}
				else
				{
					log("xmlParser error");
				}
			}
			else
			{
				Director::getInstance()->end();
			}
		}
	};
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}
Scene *GuideScene::createScene()
{
	Scene *scene = Scene::create();
	auto layer = GuideScene::create();
	scene->addChild(layer);
	return scene;
}
