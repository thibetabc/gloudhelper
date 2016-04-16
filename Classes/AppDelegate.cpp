#include "AppDelegate.h"
#include "DeviceControllerScene.h"
#if(CC_PLATFORM_IOS == CC_TARGET_PLATFORM)
#include "FindDeviceScene.h"
#include "GuideScene.h"
#else
#include "LaunchScene.h"
#endif

USING_NS_CC;


int searchTarget(int array[], int low, int high, int target)
{
    
    if (low > high) {
        return  -1;
    }
    int mid = low + (high - low) / 2;
    if (array[mid] > target) {
        return searchTarget(array, low, mid-1, target);
    }
    else if(array[mid] < target)
    {
        return searchTarget(array, mid+1, high, target);
    }
    
    return array[mid];
    
}

template <typename T, typename V>
inline int searchTargetWhile(T &array, int low, int high, V &target)
{
    
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (array[mid] > target) {
            high = mid - 1;
        }
        else if(array[mid] < target)
        {
            low = mid + 1;
        }
        else
        {
            return mid;
        }
    }
    return -1;
    
}

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
	GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    
    int testArray[] = {2, 3, 5, 7, 11, 13, 17};
    int target = 7;
    int findIndex = searchTargetWhile(testArray, 0, sizeof(testArray)/sizeof(testArray[0]), target);
    log("result = %d", findIndex);
    
    
    
    
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	glview->setFrameSize(COCOS2DX_DESIGNED_WIDTH, COCOS2DX_DESIGNED_HEIGHT);
#endif
	glview->setDesignResolutionSize(COCOS2DX_DESIGNED_WIDTH, COCOS2DX_DESIGNED_HEIGHT, ResolutionPolicy::EXACT_FIT);
	// turn on display FPS
    director->setDisplayStats(false);

	FileUtils::getInstance()->addSearchPath("vgc");
	/*auto layer = CSLoader::createNode("Layer1.csb");
	auto layoutBg = layer->getChildByName("layout_bg");
	std::string str[] = {
		"button_back",
		"button_start",
		"button_gamepad_up",
		"button_gamepad_down",
		"button_gamepad_left",
		"button_gamepad_right",
		"button_gamepad_center",
		"button_gamepad_left_up",
		"button_gamepad_left_down",
		"button_gamepad_right_up",
		"button_gamepad_right_down",
		"button_a",
		"button_b",
		"button_x",
		"button_y",
		"button_lb",
		"button_rb",
		"button_lt",
		"button_rt",
		"button_joystick_left_bg",
		"button_joystick_right_bg",
		"button_close",
		"button_gamepad_direction",
	};
	int len = sizeof(str) / sizeof(str[0]);
	for (int i = 0; i < len; i++)
	{
		auto button = layoutBg->getChildByName(str[i]);
		auto position = button->getPosition();
		auto size = button->getContentSize();
		CCLOG("width:%.4f \nheight:%.4f \n \"x\": \"%.4f\",\n \"y\": \"%.4f\",\n \"w\": \"%.4f\",\n \"h\": \"%.4f\"    [%s]\n",
			size.width, 
			size.height,
			position.x / COCOS2DX_DESIGNED_WIDTH,
			position.y / COCOS2DX_DESIGNED_HEIGHT,
			size.width / COCOS2DX_DESIGNED_WIDTH,
			size.height / COCOS2DX_DESIGNED_HEIGHT,
			str[i].c_str());
	}*/
    // set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0 / 60.0);
	FileUtils::getInstance()->addSearchPath("../Resources");
	FileUtils::getInstance()->addSearchPath("Resources");
	FileUtils::getInstance()->addSearchPath("img");
	FileUtils::getInstance()->addSearchPath("vgc");
	FileUtils::getInstance()->addSearchPath("string");
	//use plist, eg: Sprite::createWithSpriteFrameName("icon.png");
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("img.plist", "img.png"); 
    // create a scene. it's an autorelease object
#if(CC_PLATFORM_IOS == CC_TARGET_PLATFORM)
    Scene *scene = nullptr;
    bool isShowGuide = UserDefault::getInstance()->getBoolForKey(USER_DEFAULT_GUIDE_KEY, true);
    if (isShowGuide)
    {
        scene = GuideScene::createScene();
    }
    else
    {
        scene = FindDeviceScene::createScene();
    }
#else
	auto scene = LaunchScene::createScene();
#endif
	director->runWithScene(scene);
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
	
    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
    log(" AppDelegate::applicationWillEnterForeground() ");
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	auto scene = Director::getInstance()->getRunningScene();
    if(scene)
    {
        auto layer = (DeviceControllerScene *)scene->getChildByName(DEVICE_CONTROLLER_SCENE);
        if (layer)
        {
            layer->setWifiState(true);
        }
    }
}




