#include "MoreScene.h"
#include "DeviceControllerScene.h"
#include "DeviceData.h"

using namespace ui;

MoreScene::MoreScene()
{
}

MoreScene::~MoreScene()
{
}

bool MoreScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	////create action
	//cocostudio::timeline::ActionTimeline * gameoveraction = (cocostudio::timeline::ActionTimeline *)CSLoader::createTimeline("helper_more_layer.csb");
	//gameoveraction->gotoFrameAndPlay(0, true);
	////gameoveraction->pause();
	//layer->runAction(gameoveraction);
	auto layer = CSLoader::createNode("helper_more_layer.csb");
	auto layout = layer->getChildByName("bg_layout");
	auto textVersion = (Text*)layout->getChildByName("img_version_bg")->getChildByName("text_version");
	auto layoutNoDownload = layout->getChildByName("layout_no_download");
	auto layoutDownload = layout->getChildByName("layout_download");
	auto buttonTaobao = (Button*)layoutDownload->getChildByName("button_taobao");
	auto buttonDownload = (Button*)layoutDownload->getChildByName("button_download");
    auto buttonBack = (Button*)layout->getChildByName("button_back");
    buttonBack->addClickEventListener([&](Ref*)
    {
        Director::getInstance()->replaceScene(TransitionSlideInL::create(TRANSITION_TIME, DeviceControllerScene::createScene()));
    });

	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    bool isVisible = false;
#else
    bool isVisible = DeviceData::getInstance()->getChannel() == SPECIAL_DOWNLOAD_TAG;
#endif
	layoutNoDownload->setVisible(!isVisible);
	layoutDownload->setVisible(isVisible);

	textVersion->setString("V" + DeviceData::getInstance()->getVersionName());
	buttonTaobao->addTouchEventListener([=](Ref *ref, Widget::TouchEventType type)
	{
		if (Widget::TouchEventType::ENDED == type)
		{
			Application::getInstance()->openURL("https://gloud.taobao.com/");
		}
	});
	buttonDownload->addClickEventListener(CC_CALLBACK_1(MoreScene::download, this));

	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = [&](EventKeyboard::KeyCode code, Event *event){
		if (code == EventKeyboard::KeyCode::KEY_ESCAPE)
		{
			Director::getInstance()->replaceScene(TransitionSlideInL::create(TRANSITION_TIME, DeviceControllerScene::createScene()));
		}
	};
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	addChild(layer);
	return true;

}

Scene *MoreScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MoreScene::create();
	scene->addChild(layer);
	return scene;
}


void MoreScene::download(Ref *ref)
{
	JniDownload();
}



