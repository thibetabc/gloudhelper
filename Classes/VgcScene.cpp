#include "VgcScene.h"
#include "vgc/VirtualGameController.h"
#include "socket/SocketThread.h"
#include "DeviceData.h"
#include "DeviceControllerScene.h"
#include "SetupScene.h"
#include "UploadData.h"

using namespace ui;


VgcScene::VgcScene()
{
	memset(&_gamepadstate, 0, sizeof(_gamepadstate));
	isShowDialog = false;
	m_controller = nullptr;

	mCloseButton = nullptr;
	mCloseSprite = nullptr;
    int clientType = DeviceData::getInstance()->getClientType();
    if(clientType == CLIENT_TYPE_ANDROID || clientType == CLIENT_TYPE_IOS_ENTERPRISE)
    {
        mIsPhoneVibrate = UserDefault::getInstance()->getBoolForKey(USER_DEFAULT_PHONE_VIBRATE_KEY, true);
        mIsGameVibrate = UserDefault::getInstance()->getBoolForKey(USER_DEFAULT_PHONE_VIBRATE_KEY, true);
    }
    else
    {
        mIsPhoneVibrate = false;
        mIsGameVibrate = false;
    }
	mTextPosition = nullptr;
	mLayerSetup = nullptr;
	xmlParser.parseWithFile("string/string.xml");

}

VgcScene::~VgcScene()
{
	if (m_controller)
	{
		getEventDispatcher()->removeEventListener(m_controller);
	}
	getEventDispatcher()->removeCustomEventListeners(CONTROLLER_SHOCK_MSG);
}

Scene *VgcScene::createScene()
{
	Scene *scene = Scene::create();
	VgcScene *layer = VgcScene::create();
	scene->addChild(layer);
	return scene;
}

bool VgcScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
	std::string jsonstr = FileUtils::getInstance()->getStringFromFile("vgc-default.json");
	auto vgc = VirtualGameController::create();
	vgc->loadConfig(jsonstr, visibleSize.width, visibleSize.height);
	vgc->setSwallowTouches(true);
	vgc->setVisible(true);
	addChild(vgc);

	//position
	std::string posStr = StringUtils::format("%dP", DeviceData::getInstance()->getPosition() + 1);
	mTextPosition = Text::create(posStr, "", FONT_SIZE);
	mTextPosition->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	mTextPosition->setColor(Color3B(112, 112, 112));
	addChild(mTextPosition);

	//anim
	mCloseButton = vgc->getButton();
	if (mCloseButton)
	{
		mCloseSprite = Sprite::create("anim/red_0.png");
		mCloseSprite->setPosition(Vec2(mCloseButton->getContentSize() / 2));
		mCloseSprite->setVisible(false);
		mCloseButton->addChild(mCloseSprite);
	}

	m_controller = EventListenerController::create();
	m_controller->onConnected = [&](Controller * controller, Event * event) {
		log("Game controller connected");
	};
	m_controller->onDisconnected = [&](Controller * controller, Event * event) {
		log("Game controller disconnected");
	};
	m_controller->onKeyDown = [&](Controller * controller, int keycode, Event * event) {

		if (mIsPhoneVibrate)
		{
			JniVibrate(VIBRATE_TIME_SHORT);
		}
		ClientCore::dev_xinput::xinput_button input = getCode(controller, keycode);
		//log("controller->onKeyDown(), keycode = %d", keycode);
		if (input != ERROR_KEY_CODE)
		{
			_gamepadstate.sButtons |= input;
			sendMsg(_gamepadstate);
		}
	};
	m_controller->onKeyUp = [&](Controller * controller, int keycode, Event * event) {
		if (keycode == Controller::Key::BUTTON_PAUSE)
		{
            int clientType = DeviceData::getInstance()->getClientType();
            if(clientType == CLIENT_TYPE_IOS_APP_STORE)
            {
                UploadData::getInstance()->deleteSchedule();
                UploadData::getInstance()->upload(UploadData::TYPE::END);
                SocketThread::GetInstance()->stop();
                Director::getInstance()->replaceScene(TransitionCrossFade::create(TRANSITION_TIME, DeviceControllerScene::createScene()));
            }
            else
            {
                JniVibrate(VIBRATE_CANCEL);
                Director::getInstance()->replaceScene(TransitionCrossFade::create(TRANSITION_TIME, SetupScene::createScene()));
            }
		}
		else
		{
			ClientCore::dev_xinput::xinput_button input = getCode(controller, keycode);
			//log("controller->onKeyUp(), keycode = %d", keycode);
			if (input != ERROR_KEY_CODE)
			{
				_gamepadstate.sButtons &= ~input;
				sendMsg(_gamepadstate);
			}
		}
	};
	m_controller->onKeyRepeat = [&](Controller * controller, int keycode, Event * event) {

	};
	m_controller->onAxisEvent = [&](Controller * controller, int keycode, Event * event) {

		const auto& keyStatus = controller->getKeyStatus(keycode);
		//log("Axis KeyCode:%d Axis Value:%f", keycode, keyStatus.value);
		ClientCore::XINPUT_GAMEPAD lastGamepadstate = _gamepadstate;
		switch (keycode)
		{
		case Controller::Key::JOYSTICK_LEFT_X:
			_gamepadstate.sThumbLX = (short)keyStatus.value;
			break;
		case Controller::Key::JOYSTICK_LEFT_Y:
			_gamepadstate.sThumbLY = (short)keyStatus.value;
			break;
		case Controller::Key::JOYSTICK_RIGHT_X:
			_gamepadstate.sThumbRX = (short)keyStatus.value;
			break;
		case Controller::Key::JOYSTICK_RIGHT_Y:
			_gamepadstate.sThumbRY = (short)keyStatus.value;
			break;
		case Controller::Key::AXIS_LEFT_TRIGGER:
			_gamepadstate.sLeftTrigger = (short)keyStatus.value;
			//大于0按下，等于0弹起
			if(mIsPhoneVibrate && _gamepadstate.sLeftTrigger > 0.0f)
			{
				JniVibrate(VIBRATE_TIME_SHORT);
			}
			break;
		case Controller::Key::AXIS_RIGHT_TRIGGER:
			_gamepadstate.sRightTrigger = (short)keyStatus.value;
			//大于0按下，等于0弹起
			if(mIsPhoneVibrate && _gamepadstate.sRightTrigger > 0.0f)
			{
				JniVibrate(VIBRATE_TIME_SHORT);
			}
			break;
		default:
			return;
		}
		if (memcmp(&lastGamepadstate, &_gamepadstate, sizeof(_gamepadstate)) != 0)
		{
			sendMsg(_gamepadstate);
		}
	};						 
	getEventDispatcher()->addEventListenerWithFixedPriority(m_controller, 1);

	//exit
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(VgcScene::keyboardKeyPressedCallback, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	
	receiveODSocket();
	return true;
}



void VgcScene::keyboardKeyPressedCallback(EventKeyboard::KeyCode code, Event *event)
{
	if (code == EventKeyboard::KeyCode::KEY_ESCAPE || code == EventKeyboard::KeyCode::KEY_MENU)
	{
		if (mCloseButton)
		{
			if (mCloseSprite->getNumberOfRunningActions() == 0)
			{
				auto anim = Animation::create();
				const int FRAME_MAX = 4;
				for (int i = 0; i < FRAME_MAX; i++)
				{
					anim->addSpriteFrameWithFile(StringUtils::format("anim/red_%d.png", i));
				}
				// should last 12.0f seconds. And there are FRAME_MAX frames. 
				anim->setDelayPerUnit(12.0f / 60.0f / FRAME_MAX);
				anim->setRestoreOriginalFrame(true);//back first frame
				auto animate = Animate::create(anim);

				//1. pre action
				//auto action = Sequence::createWithTwoActions(Repeat::create(Sequence::createWithTwoActions(animate, animate->reverse()), 3), CallFunc::create([&](){mCloseSprite->setVisible(false); }));
				//mCloseSprite->runAction(action);

				//2.custom action
				auto action1 = Sequence::createWithTwoActions(Repeat::create(Sequence::createWithTwoActions(animate, animate->reverse()), 2), animate);
				anim->setDelayPerUnit(25.0f / 60.f / FRAME_MAX);
				mCloseSprite->runAction(Sequence::create(action1, Animate::create(anim)->reverse(), CallFunc::create([&](){mCloseSprite->setVisible(false); }), NULL));
				mCloseSprite->setVisible(true);				
			}
		}
	}
}

void VgcScene::sendMsg(ClientCore::XINPUT_GAMEPAD xinput_gamepad)
{
	std::string sendMsg = StringUtils::format("{\"Type\":\"Joystick\",\"Buttons\":%d,\"LeftTrigger\":%d,\"RightTrigger\":%d,\"ThumbLX\":%d,\"ThumbLY\":%d,\"ThumbRX\":%d,\"ThumbRY\":%d}",
		xinput_gamepad.sButtons, xinput_gamepad.sLeftTrigger, xinput_gamepad.sRightTrigger, xinput_gamepad.sThumbLX, xinput_gamepad.sThumbLY, xinput_gamepad.sThumbRX, xinput_gamepad.sThumbRY);

	log("socket::send(...): called: %s", sendMsg.c_str());
#if(CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	ODSocket csocket = SocketThread::GetInstance()->getSocket();
	char tHeader[2] = { 0 };
	int tLen = sendMsg.length();
	tHeader[0] = (char)(tLen >> 8);
	tHeader[1] = (char)tLen;
	int sendResult = csocket.Send(tHeader, 2, 0);
	if (sendResult == -1 && !isShowDialog)
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]()
		{
			if (isShowDialog)
			{
				return;
			}
			showDialog(xmlParser["disconnect_gloud_title"], xmlParser["disconnect_gloud_msg"], xmlParser["ok_button"], 90.0f);
		});
	}
	else
	{
		csocket.Send(sendMsg.c_str(), sendMsg.length(), 0);
	}
#endif
}
ClientCore::dev_xinput::xinput_button VgcScene::getCode(Controller * controller, int keycode)
{
	ClientCore::dev_xinput::xinput_button input = ERROR_KEY_CODE;
	switch (keycode)
	{
	case Controller::Key::BUTTON_SELECT:
		input = ClientCore::dev_xinput::XINPUT_GAMEPAD_BACK;
		break;
	case Controller::Key::BUTTON_START:
		input = ClientCore::dev_xinput::XINPUT_GAMEPAD_START;
		break;
	case Controller::Key::BUTTON_DPAD_UP:
		input = ClientCore::dev_xinput::XINPUT_GAMEPAD_DPAD_UP;
		break;
	case Controller::Key::BUTTON_DPAD_DOWN:
		input = ClientCore::dev_xinput::XINPUT_GAMEPAD_DPAD_DOWN;
		break;
	case Controller::Key::BUTTON_DPAD_LEFT:
		input = ClientCore::dev_xinput::XINPUT_GAMEPAD_DPAD_LEFT;
		break;
	case Controller::Key::BUTTON_DPAD_RIGHT:
		input = ClientCore::dev_xinput::XINPUT_GAMEPAD_DPAD_RIGHT;
		break;
	case Controller::Key::BUTTON_A:
		input = ClientCore::dev_xinput::XINPUT_GAMEPAD_A;
		break;
	case Controller::Key::BUTTON_B:
		input = ClientCore::dev_xinput::XINPUT_GAMEPAD_B;
		break;
	case Controller::Key::BUTTON_X:
		input = ClientCore::dev_xinput::XINPUT_GAMEPAD_X;
		break;
	case Controller::Key::BUTTON_Y:
		input = ClientCore::dev_xinput::XINPUT_GAMEPAD_Y;
		break;
	case Controller::Key::BUTTON_LEFT_SHOULDER:
		input = ClientCore::dev_xinput::XINPUT_GAMEPAD_LEFT_SHOULDER;
		break;
	case Controller::Key::BUTTON_RIGHT_SHOULDER:
		input = ClientCore::dev_xinput::XINPUT_GAMEPAD_RIGHT_SHOULDER;
		break;
	case Controller::Key::BUTTON_LEFT_THUMBSTICK:
		input = ClientCore::dev_xinput::XINPUT_GAMEPAD_LEFT_THUMB;
		break;
	case Controller::Key::BUTTON_RIGHT_THUMBSTICK:
		input = ClientCore::dev_xinput::XINPUT_GAMEPAD_RIGHT_THUMB;
		break;
	}
	return input;
}

void VgcScene::onEnter()
{
	Layer::onEnter(); 
	UploadData::getInstance()->upload(UploadData::TYPE::END);//如果用户启动时没连wifi,这里再检查一次
	UploadData::getInstance()->upload(UploadData::TYPE::START);
	UploadData::getInstance()->startSchedule();
}

void VgcScene::receiveODSocket()
{
	//shake
	getEventDispatcher()->addCustomEventListener(CONTROLLER_SHOCK_MSG, [&](EventCustom * event)
	{
		if (mIsGameVibrate)
		{
			rapidjson::Document *data = (rapidjson::Document *)event->getUserData();
			const rapidjson::Value &valueLeft = data->operator[]("Left");
			const rapidjson::Value &valueRight = data->operator[]("Right");
			int leftShock = valueLeft.GetInt();
			int rightShock = valueRight.GetInt();
			if (leftShock <= 0 && rightShock <= 0)
			{
				JniVibrate(VIBRATE_CANCEL);
			}
			else
			{
				JniVibrate(100.0 / 65535.0 * MAX(leftShock, rightShock));
			}
		}
	});


	
}
