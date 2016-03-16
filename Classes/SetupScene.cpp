#include "SetupScene.h"
#include "VgcScene.h"
#include "DeviceControllerScene.h"
#include "DeviceData.h"
#include "socket/SocketThread.h"
#include "LoadingLayer.h"
#include "UploadData.h"

using namespace ui;
SetupScene::SetupScene()
{
	xmlParser.parseWithFile("string/string.xml");
	mLayerLoading = nullptr;
}

SetupScene::~SetupScene()
{
	getEventDispatcher()->removeCustomEventListeners(CUSTOM_SWAP_POSITION_MSG);
	getEventDispatcher()->removeCustomEventListeners(REPORT_JOYSTICK_STATUS_MSG);
	

}

Scene *SetupScene::createScene()
{
	auto scene = Scene::create();
	auto layer = SetupScene::create();
	scene->addChild(layer);
	return scene;
}

bool SetupScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto layer = CSLoader::createNode("layer_vgc_setup.csb");
	addChild(layer);

	auto layoutBg = layer->getChildByName("layout_bg");
	auto listviewFullLayout = (ListView*)layoutBg->getChildByName("listview_full_layout");	
	auto layoutPositionItemBg = listviewFullLayout->getChildByName("layout_position_bg")->getChildByName("layout_position_item");
	mLayoutPositionItem = layoutPositionItemBg->getChildByName("layout_button_bg");
	mLayoutPositionItem->setVisible(false);


	//返回上一层
	auto textBack = (Text *)layoutBg->getChildByName("layout_green_bg")->getChildByName("text_back");
	textBack->addClickEventListener([](Ref *ref){
		Director::getInstance()->replaceScene(TransitionCrossFade::create(TRANSITION_TIME, VgcScene::createScene()));
	});
	listviewFullLayout->setClippingType(LayoutClippingType::SCISSOR);//解决ClippingNode会白屏的bug
	//loading提示
	mLayerLoading = LoadingLayer::create();
	mLayerLoading->setPosition(Vec2(layoutPositionItemBg->getContentSize().width / 2 - mLayerLoading->getContentSize().width / 2, layoutPositionItemBg->getContentSize().height / 2 - mLayerLoading->getContentSize().height / 2));
	layoutPositionItemBg->addChild(mLayerLoading);



	//换位置按钮
	auto buttonPosition0 = (Button *)mLayoutPositionItem->getChildByName("button_p0");
	auto buttonPosition1 = (Button *)mLayoutPositionItem->getChildByName("button_p1");
	auto buttonPosition2 = (Button *)mLayoutPositionItem->getChildByName("button_p2");
	auto buttonPosition3 = (Button *)mLayoutPositionItem->getChildByName("button_p3");
	buttonPosition0->setTag(0);
	buttonPosition0->addClickEventListener(CC_CALLBACK_1(SetupScene::buttonPositionClickEvent, this));
	buttonPosition1->setTag(1);
	buttonPosition1->addClickEventListener(CC_CALLBACK_1(SetupScene::buttonPositionClickEvent, this));
	buttonPosition2->setTag(2);
	buttonPosition2->addClickEventListener(CC_CALLBACK_1(SetupScene::buttonPositionClickEvent, this));
	buttonPosition3->setTag(3);
	buttonPosition3->addClickEventListener(CC_CALLBACK_1(SetupScene::buttonPositionClickEvent, this));

	auto listviewSetup        = listviewFullLayout->getChildByName("layout_setup_bg")->getChildByName("listview_setup");
	auto checkboxRotate       = (CheckBox*)listviewSetup->getChildByName("layout_rotate_bg")->getChildByName("button_rotate")->getChildByName("checkbox_rotate");
	auto checkboxPhoneVibrate = (CheckBox*)listviewSetup->getChildByName("layout_phone_vibrate_bg")->getChildByName("button_phone_vibrate")->getChildByName("checkbox_phone_vibrate");
	auto checkboxGameVibrate  = (CheckBox*)listviewSetup->getChildByName("layout_game_vibrate_bg")->getChildByName("button_game_vibrate")->getChildByName("checkbox_game_vibrate");
	auto buttonExitVgc        = (Button *)listviewFullLayout->getChildByName("layout_exit_bg")->getChildByName("layout_exit_vgc_bg")->getChildByName("button_exit_vgc");

	//是否显示(需1.3.5版本格来云游戏)
	auto textVibrateVersionTips = listviewSetup->getChildByName("layout_game_vibrate_bg")->getChildByName("button_game_vibrate")->getChildByName("text_vibrate_version_tips");
	mTextPositionVersionTips = (Text *)layoutPositionItemBg->getChildByName("text_position_version_tips");
	if(DeviceData::getInstance()->getGloudVersionNameInteger() <= GLOUD_GAME_VERSION)
	{
		removeLayerLoading();
		mTextPositionVersionTips->setVisible(true);
	}
	else
	{
		mTextPositionVersionTips->setVisible(false);
		textVibrateVersionTips->setVisible(false);
	}

	//rotate
	checkboxRotate->addEventListener([=](Ref *ref, CheckBox::EventType type)
	{
		if (type == CheckBox::EventType::SELECTED)
		{
			JniSetRequestedOrientation(SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
			UserDefault::getInstance()->setIntegerForKey(USER_DEFAULT_SCREEN_ORIENTATION_KEY, SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
		}
		else if (type == CheckBox::EventType::UNSELECTED)
		{
			JniSetRequestedOrientation(SCREEN_ORIENTATION_USER_LANDSCAPE);
			UserDefault::getInstance()->setIntegerForKey(USER_DEFAULT_SCREEN_ORIENTATION_KEY, SCREEN_ORIENTATION_USER_LANDSCAPE);
		}
	});
	int dir = JniGetRequestedOrientation();
	if (dir == SCREEN_ORIENTATION_SENSOR_LANDSCAPE)
	{
		checkboxRotate->setSelected(true);
	}
	else
	{
		checkboxRotate->setSelected(false);
	}

	//phoneVibrate
	checkboxPhoneVibrate->addEventListener([=](Ref *ref, CheckBox::EventType type)
	{
		if (type == CheckBox::EventType::SELECTED)
		{
			UserDefault::getInstance()->setBoolForKey(USER_DEFAULT_PHONE_VIBRATE_KEY, true);
			JniVibrate(VIBRATE_TIME);
		}
		else if (type == CheckBox::EventType::UNSELECTED)
		{
			UserDefault::getInstance()->setBoolForKey(USER_DEFAULT_PHONE_VIBRATE_KEY, false);
		}
	});
	checkboxPhoneVibrate->setSelected(UserDefault::getInstance()->getBoolForKey(USER_DEFAULT_PHONE_VIBRATE_KEY, true));

	//gameVibrate
	checkboxGameVibrate->addEventListener([=](Ref *ref, CheckBox::EventType type)
	{
		if (type == CheckBox::EventType::SELECTED)
		{
			UserDefault::getInstance()->setBoolForKey(USER_DEFAULT_GAME_VIBRATE_KEY, true);
			JniVibrate(VIBRATE_TIME);
		}
		else if (type == CheckBox::EventType::UNSELECTED)
		{
			UserDefault::getInstance()->setBoolForKey(USER_DEFAULT_GAME_VIBRATE_KEY, false);
		}
	});
	checkboxGameVibrate->setSelected(UserDefault::getInstance()->getBoolForKey(USER_DEFAULT_GAME_VIBRATE_KEY, true));



	buttonExitVgc->addClickEventListener([](Ref *ref)
	{
		UploadData::getInstance()->deleteSchedule();
		UploadData::getInstance()->upload(UploadData::TYPE::END);
		SocketThread::GetInstance()->stop();
		Director::getInstance()->replaceScene(TransitionCrossFade::create(TRANSITION_TIME, DeviceControllerScene::createScene()));
	});

	//exit
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = [](EventKeyboard::KeyCode code, Event *event){
		if (code == EventKeyboard::KeyCode::KEY_ESCAPE)
		{
			Director::getInstance()->replaceScene(TransitionCrossFade::create(TRANSITION_TIME, VgcScene::createScene()));
		}
	};
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);


	getEventDispatcher()->addCustomEventListener(CUSTOM_SWAP_POSITION_MSG, [=](EventCustom * event)
	{
		rapidjson::Document *data = (rapidjson::Document *)event->getUserData();
		const rapidjson::Value &valueCode = data->operator[]("Code");
		const rapidjson::Value &valuePosition = data->operator[]("Position");
		int code = valueCode.GetInt();
		if (code != 0)
		{
			ToastShow(xmlParser["swap_position_error"]);
		}
		else
		{
			int position = valuePosition.GetInt();
			DeviceData::getInstance()->setPosition(position);
			ToastShow(xmlParser["swap_position_success"]);
		}

	});

	getEventDispatcher()->addCustomEventListener(REPORT_JOYSTICK_STATUS_MSG, [=](EventCustom * event)
	{
		rapidjson::Document *data = (rapidjson::Document *)event->getUserData();
		const rapidjson::Value &valueJoysticks = data->operator[]("Joysticks");
		if (valueJoysticks.IsArray() && valueJoysticks.Size() > 0)
		{
			int myPosition = data->operator[]("MyPosition").GetInt();
			DeviceData::getInstance()->setPosition(myPosition);
			log("myPosition = %d", myPosition);
			for (rapidjson::SizeType i = 0; i < valueJoysticks.Size(); i++)
			{
				const rapidjson::Value &valueJson = valueJoysticks[i];
				int type = valueJson["Type"].GetInt();
				int position = valueJson["Position"].GetInt();
				Button *buttonPos = (Button*)mLayoutPositionItem->getChildByTag(position);
				if (position == myPosition)
				{
					buttonPos->setTitleColor(Color3B::WHITE);
					buttonPos->setTitleText(xmlParser["position_self"]);
					buttonPos->loadTextureNormal("button_normal_orange_vgc_setup.png");
					buttonPos->loadTexturePressed("button_press_vgc_setup.png");
				}
				else
				{
					if (type == 0)
					{
						buttonPos->setTitleColor(Color3B::GRAY);
						buttonPos->setTitleText(xmlParser["position_empty"]);
						buttonPos->loadTextureNormal("button_normal_white_vgc_setup.png");
						buttonPos->loadTexturePressed("button_press_white_vgc_setup.png");
					}
					else
					{
						buttonPos->setTitleColor(Color3B::WHITE);
						buttonPos->setTitleText(StringUtils::format("%dP", position+1));
						buttonPos->loadTextureNormal("button_normal_gray_vgc_setup.png");
						buttonPos->loadTexturePressed("button_press_gray_vgc_setup.png");
					}
				}
			}
			removeLayerLoading();
			mTextPositionVersionTips->setVisible(false);
			mLayoutPositionItem->setVisible(true);
		}

	});
	
	//scheduleOnce([&](float dt){requestJoystickStatus(); }, 2.0f, "1111");
    requestJoystickStatus();
    //ios去掉翻转设置
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    listviewSetup->getChildByName("layout_rotate_bg")->removeFromParent();
#endif

	return true;
}
void SetupScene::requestJoystickStatus()
{
#if(CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	ODSocket csocket = SocketThread::GetInstance()->getSocket();
	std::string sendMsg = "{\"Type\":\"RequestJoystickStatus\"}";
	int sendResult = csocket.SendToJava(sendMsg.c_str(), sendMsg.length(), 0);
	if (sendResult == -1)
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([&]() {
			ToastShow(xmlParser["no_connect_msg"]);
		});
	}
#endif
}
void SetupScene::buttonPositionClickEvent(Ref *ref)
{
	Button *button = (Button*)ref;
	int curPosion = DeviceData::getInstance()->getPosition();
	int newPosition = button->getTag();
	//log("buttonClickEventPosition(...) : button->getTag() = %d, curPosion = %d", newPosition, curPosion);
	if (curPosion == newPosition)
	{
		//ToastShow(xmlParser["swap_position_success"]);
		return;
	}
	std::string sendMsg = StringUtils::format("{\"Type\":\"SwapPosition\",\"NewPosition\":%d}", newPosition);
	log("socket::send(...): called: %s", sendMsg.c_str());
#if(CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	ODSocket csocket = SocketThread::GetInstance()->getSocket();
	int sendResult = csocket.SendToJava(sendMsg.c_str(), sendMsg.length(), 0);
	if (sendResult == -1)
	{
		ToastShow(xmlParser["no_connect_msg"]);
	}
#endif
}
void SetupScene::removeLayerLoading()
{
	if (mLayerLoading != nullptr)
	{
		mLayerLoading->removeFromParent();
		mLayerLoading = nullptr;
	}
}
