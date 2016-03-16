#include "LaunchScene.h"
#include "FindDeviceScene.h"
#include "GuideScene.h"
#include "DeviceControllerScene.h"


LaunchScene::LaunchScene()
{

}
LaunchScene::~LaunchScene()
{

}
bool LaunchScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto layer = CSLoader::createNode("launch_layer.csb");
	//create action
	cocostudio::timeline::ActionTimeline * gameoveraction = (cocostudio::timeline::ActionTimeline *)CSLoader::createTimeline("launch_layer.csb");
	gameoveraction->gotoFrameAndPlay(0, true);
	gameoveraction->setLastFrameCallFunc([](){
		bool isShowGuide = UserDefault::getInstance()->getBoolForKey(USER_DEFAULT_GUIDE_KEY, true);
		if (isShowGuide)
		{
			Director::getInstance()->replaceScene(TransitionCrossFade::create(TRANSITION_TIME, GuideScene::createScene()));
		}
		else
		{
            Director::getInstance()->replaceScene(TransitionCrossFade::create(TRANSITION_TIME, FindDeviceScene::createScene()));
        }
	});
	//gameoveraction->pause();
	layer->runAction(gameoveraction);
	addChild(layer);
	return true;
} 
Scene *LaunchScene::createScene()
{
	auto scene = Scene::create();
	auto layer = LaunchScene::create();
	scene->addChild(layer);
	return scene;
}