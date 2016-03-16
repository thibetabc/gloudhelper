#include "MyVgcScene.h"
#include "vgc/VirtualGameController.h"
#include "socket/SocketThread.h"
#include "DeviceData.h"
#include "DeviceControllerScene.h"

using namespace ui;


MyVgcScene::MyVgcScene()
{
	memset(&_gamepadstate, 0, sizeof(_gamepadstate));
	isShowDialog = false;
	m_controller = nullptr;

	mCloseButton = nullptr;
	mCloseSprite = nullptr;
	mVgc = nullptr;
	isLockScreen = false;


}

MyVgcScene::~MyVgcScene()
{
	if (m_controller)
	{
		getEventDispatcher()->removeEventListener(m_controller);
	}
	getEventDispatcher()->removeCustomEventListeners(CONTROLLER_SHOCK_MSG);
}

Scene *MyVgcScene::createScene()
{
	Scene *scene = Scene::create();
	MyVgcScene *layer = MyVgcScene::create();
	scene->addChild(layer);
	return scene;
}

void MyVgcScene::initView()
{

	cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
	FileUtils::getInstance()->addSearchPath("vgc");
	auto layer = CSLoader::createNode("Layer1.csb");	
	auto layoutBg               = layer->getChildByName("layout_bg");
	auto buttonLock             = (Button*)layoutBg->getChildByName("button_lock");
	auto buttonClose            = (Button*)layoutBg->getChildByName("button_close");
	auto buttonBack             = (Button*)layoutBg->getChildByName("button_back");
	auto buttonStart            = (Button*)layoutBg->getChildByName("button_start");
	auto buttonLb               = (Button*)layoutBg->getChildByName("button_lb");
	auto buttonRb               = (Button*)layoutBg->getChildByName("button_rb");
	auto buttonLt               = (Button*)layoutBg->getChildByName("button_lt");
	auto buttonRt               = (Button*)layoutBg->getChildByName("button_rt");
	auto buttonX                = (Button*)layoutBg->getChildByName("button_x");
	auto buttonY                = (Button*)layoutBg->getChildByName("button_y");
	auto buttonA                = (Button*)layoutBg->getChildByName("button_a");
	auto buttonB                = (Button*)layoutBg->getChildByName("button_b");
	auto buttonJoystickLeft     = (Button*)layoutBg->getChildByName("button_joystick_left");
	auto buttonJoystickRight    = (Button*)layoutBg->getChildByName("button_joystick_right");
	auto layoutGamepadBg        = layoutBg->getChildByName("layout_gamepad_bg");
	auto buttonGamepadUp        = (Button*)layoutGamepadBg->getChildByName("button_gamepad_up");
	auto buttonGamepadDown      = (Button*)layoutGamepadBg->getChildByName("button_gamepad_down");
	auto buttonGamepadLeft      = (Button*)layoutGamepadBg->getChildByName("button_gamepad_left");
	auto buttonGamepadRight     = (Button*)layoutGamepadBg->getChildByName("button_gamepad_right");
	auto buttonGamepadLeftUp    = (Button*)layoutGamepadBg->getChildByName("button_gamepad_left_up");
	auto buttonGamepadLeftDown  = (Button*)layoutGamepadBg->getChildByName("button_gamepad_left_down");
	auto buttonGamepadRightUp   = (Button*)layoutGamepadBg->getChildByName("button_gamepad_right_up");
	auto buttonGamepadRightDown = (Button*)layoutGamepadBg->getChildByName("button_gamepad_right_down");

	buttonLock->setTag(BUTTON_LOCK);
	buttonClose->setTag(BUTTON_CLOSE);
	buttonBack->setTag(BUTTON_BACK);
	buttonStart->setTag(BUTTON_START);
	buttonLb->setTag(BUTTON_LB);
	buttonRb->setTag(BUTTON_RB);
	buttonLt->setTag(BUTTON_LT);
	buttonRt->setTag(BUTTON_RT);
	buttonX->setTag(BUTTON_X);
	buttonY->setTag(BUTTON_Y);
	buttonA->setTag(BUTTON_A);
	buttonB->setTag(BUTTON_B);
	buttonJoystickLeft->setTag(BUTTON_JOYSTICK_LEFT);
	buttonJoystickRight->setTag(BUTTON_JOYSTICK_RIGHT);
	buttonGamepadUp->setTag(BUTTON_GAMEPAD_UP);
	buttonGamepadDown->setTag(BUTTON_GAMEPAD_DOWN);
	buttonGamepadLeft->setTag(BUTTON_GAMEPAD_LEFT);
	buttonGamepadRight->setTag(BUTTON_GAMEPAD_RIGHT);
	buttonGamepadLeftUp->setTag(BUTTON_GAMEPAD_LEFT_UP);
	buttonGamepadLeftDown->setTag(BUTTON_GAMEPAD_LEFT_DOWN);
	buttonGamepadRightUp->setTag(BUTTON_GAMEPAD_RIGHT_UP);
	buttonGamepadRightDown->setTag(BUTTON_GAMEPAD_RIGHT_DOWN);	

	buttonLock->addTouchEventListener(CC_CALLBACK_2(MyVgcScene::touchEventListener,this));
	buttonClose->addTouchEventListener(CC_CALLBACK_2(MyVgcScene::touchEventListener,this));
	buttonBack->addTouchEventListener(CC_CALLBACK_2(MyVgcScene::touchEventListener,this));
	buttonStart->addTouchEventListener(CC_CALLBACK_2(MyVgcScene::touchEventListener,this));
	buttonLb->addTouchEventListener(CC_CALLBACK_2(MyVgcScene::touchEventListener,this));
	buttonRb->addTouchEventListener(CC_CALLBACK_2(MyVgcScene::touchEventListener,this));
	buttonLt->addTouchEventListener(CC_CALLBACK_2(MyVgcScene::touchEventListener,this));
	buttonRt->addTouchEventListener(CC_CALLBACK_2(MyVgcScene::touchEventListener,this));
	buttonX->addTouchEventListener(CC_CALLBACK_2(MyVgcScene::touchEventListener,this));
	buttonY->addTouchEventListener(CC_CALLBACK_2(MyVgcScene::touchEventListener,this));
	buttonA->addTouchEventListener(CC_CALLBACK_2(MyVgcScene::touchEventListener,this));
	buttonB->addTouchEventListener(CC_CALLBACK_2(MyVgcScene::touchEventListener,this));
	buttonJoystickLeft->addTouchEventListener(CC_CALLBACK_2(MyVgcScene::touchEventListener,this));
	buttonJoystickRight->addTouchEventListener(CC_CALLBACK_2(MyVgcScene::touchEventListener,this));
	buttonGamepadUp->addTouchEventListener(CC_CALLBACK_2(MyVgcScene::touchEventListener,this));
	buttonGamepadDown->addTouchEventListener(CC_CALLBACK_2(MyVgcScene::touchEventListener,this));
	buttonGamepadLeft->addTouchEventListener(CC_CALLBACK_2(MyVgcScene::touchEventListener,this));
	buttonGamepadRight->addTouchEventListener(CC_CALLBACK_2(MyVgcScene::touchEventListener,this));
	buttonGamepadLeftUp->addTouchEventListener(CC_CALLBACK_2(MyVgcScene::touchEventListener,this));
	buttonGamepadLeftDown->addTouchEventListener(CC_CALLBACK_2(MyVgcScene::touchEventListener,this));
	buttonGamepadRightUp->addTouchEventListener(CC_CALLBACK_2(MyVgcScene::touchEventListener,this));
	buttonGamepadRightDown->addTouchEventListener(CC_CALLBACK_2(MyVgcScene::touchEventListener,this));

}

void MyVgcScene::touchEventListener(Ref* ref, Widget::TouchEventType type)
{
	Button *button = (Button*)ref;
	int tag = button->getTag();

	int keyCode = 0;
	switch (tag)
	{
	case BUTTON_LOCK:
		break;
	case BUTTON_CLOSE:
		keyCode = Controller::Key::BUTTON_PAUSE;
		break;
	case BUTTON_BACK:
		keyCode = Controller::Key::BUTTON_SELECT;
		break;
	case BUTTON_START:
		keyCode = Controller::Key::BUTTON_START;
		break;
	case BUTTON_LB:
		keyCode = Controller::Key::BUTTON_LEFT_SHOULDER;
		break;
	case BUTTON_RB:
		keyCode = Controller::Key::BUTTON_RIGHT_SHOULDER;
		break;
	case BUTTON_LT:
		keyCode = Controller::Key::BUTTON_LEFT_THUMBSTICK;
		break;
	case BUTTON_RT:
		keyCode = Controller::Key::BUTTON_RIGHT_THUMBSTICK;
		break;
	case BUTTON_X:
		keyCode = Controller::Key::BUTTON_X;
		break;
	case BUTTON_Y:
		keyCode = Controller::Key::BUTTON_Y;
		break;
	case BUTTON_A:
		keyCode = Controller::Key::BUTTON_A;
		break;
	case BUTTON_B:
		keyCode = Controller::Key::BUTTON_B;
		break;
	case BUTTON_GAMEPAD_UP:
		keyCode = Controller::Key::BUTTON_DPAD_UP;
		break;
	case BUTTON_GAMEPAD_DOWN:
		keyCode = Controller::Key::BUTTON_DPAD_DOWN;
		break;
	case BUTTON_GAMEPAD_LEFT:
		keyCode = Controller::Key::BUTTON_DPAD_LEFT;
		break;
	case BUTTON_GAMEPAD_RIGHT:
		keyCode = Controller::Key::BUTTON_DPAD_RIGHT;
		break;
	case BUTTON_GAMEPAD_LEFT_UP:
		break;
	case BUTTON_GAMEPAD_LEFT_DOWN:
		break;
	case BUTTON_GAMEPAD_RIGHT_UP:
		break;
	case BUTTON_GAMEPAD_RIGHT_DOWN:
		break;
	case BUTTON_JOYSTICK_LEFT:
		break;
	case BUTTON_JOYSTICK_RIGHT:
		break;
	default:
		break;
	}
	if (keyCode <= 0)
	{
		return;
	}
	if (Widget::TouchEventType::BEGAN == type)
	{
		onKeyUp(keyCode);
	}
	else if (Widget::TouchEventType::BEGAN == type)
	{
		onKeyDown(keyCode);
	}
}

void MyVgcScene::onKeyUp(int keyCode)
{

}
void MyVgcScene::onKeyDown(int keyCode)
{

}

bool MyVgcScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	initView();
	//Size visibleSize = Director::getInstance()->getVisibleSize();
	//FileUtils::getInstance()->addSearchPath("vgc");
	//mVgc = VirtualGameController::create();
	//std::string jsonstr = FileUtils::getInstance()->getStringFromFile("vgc-default.json");
	//mVgc->loadConfig(jsonstr, visibleSize.width, visibleSize.height);
	//addChild(mVgc);
	//mVgc->setSwallowTouches(true);
	//mVgc->setVisible(true);

	////position
	//std::string posStr = StringUtils::format("%dP", DeviceData::getInstance()->getPosition() + 1);
	//auto playerPositionText = Text::create(posStr, "", FONT_SIZE);
	//playerPositionText->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	//playerPositionText->setColor(Color3B(112, 112, 112));
	//addChild(playerPositionText);

	////anim
	//mCloseButton = mVgc->getButton();
	//if (mCloseButton)
	//{
	//	mCloseSprite = Sprite::create("anim/red_0.png");
	//	mCloseSprite->setPosition(Vec2(mCloseButton->getContentSize() / 2));
	//	mCloseSprite->setVisible(false);
	//	mCloseButton->addChild(mCloseSprite);
	//}

	//m_controller = EventListenerController::create();
	//m_controller->onConnected = [&](Controller * controller, Event * event) {
	//};
	//m_controller->onDisconnected = [&](Controller * controller, Event * event) {
	//};
	//m_controller->onKeyDown = [&](Controller * controller, int keycode, Event * event) {
	//	ClientCore::dev_xinput::xinput_button input = getCode(controller, keycode);
	//	//log("controller->onKeyDown(), keycode = %d", keycode);
	//	if (input != ERROR_KEY_CODE)
	//	{
	//		_gamepadstate.sButtons |= input;
	//		sendMsg(_gamepadstate);
	//	}
	//};
	//m_controller->onKeyUp = [&](Controller * controller, int keycode, Event * event) {
	//	if (keycode == Controller::Key::BUTTON_PAUSE)
	//	{
	//		log("controller->onKeyUp(), keycode = %d", keycode);
	//		auto socketThread = SocketThread::GetInstance();
	//		if (socketThread->state == 0)
	//		{
	//			socketThread->stop();
	//		}
	//		Director::getInstance()->replaceScene(TransitionSlideInT::create(TRANSITION_TIME, DeviceControllerScene::createScene()));
	//		return;
	//	}
	//	ClientCore::dev_xinput::xinput_button input = getCode(controller, keycode);
	//	//log("controller->onKeyUp(), keycode = %d", keycode);
	//	if (input != ERROR_KEY_CODE)
	//	{
	//		_gamepadstate.sButtons &= ~input;
	//		sendMsg(_gamepadstate);
	//	}
	//};
	//m_controller->onKeyRepeat = [&](Controller * controller, int keycode, Event * event) {

	//};
	//m_controller->onAxisEvent = [&](Controller * controller, int keycode, Event * event) {

	//	ClientCore::XINPUT_GAMEPAD lastGamepadstate = _gamepadstate;
	//	switch (keycode)
	//	{
	//	case Controller::Key::JOYSTICK_LEFT_X:
	//		_gamepadstate.sThumbLX = (short)controller->getKeyStatus(keycode).value;
	//		//log("JOYSTICK_LEFT_X.sThumbLX  = %d", _gamepadstate.sThumbLX);
	//		break;
	//	case Controller::Key::JOYSTICK_LEFT_Y:
	//		_gamepadstate.sThumbLY = (short)controller->getKeyStatus(keycode).value;
	//		//log("JOYSTICK_LEFT_Y.sThumbLY  = %d", _gamepadstate.sThumbLY);
	//		break;
	//	case Controller::Key::JOYSTICK_RIGHT_X:
	//		_gamepadstate.sThumbRX = (short)controller->getKeyStatus(keycode).value;
	//		//log("JOYSTICK_RIGHT_X.sThumbRX  = %d", _gamepadstate.sThumbRX);
	//		break;
	//	case Controller::Key::JOYSTICK_RIGHT_Y:
	//		_gamepadstate.sThumbRY = (short)controller->getKeyStatus(keycode).value;
	//		//log("JOYSTICK_RIGHT_Y.sThumbRY  = %d", _gamepadstate.sThumbRY);
	//		break;
	//	case Controller::Key::AXIS_LEFT_TRIGGER:
	//		_gamepadstate.sLeftTrigger = (short)controller->getKeyStatus(keycode).value;
	//		//log("AXIS_LEFT_TRIGGER.sLeftTrigger  = %d", _gamepadstate.sLeftTrigger);
	//		break;
	//	case Controller::Key::AXIS_RIGHT_TRIGGER:
	//		_gamepadstate.sRightTrigger = (short)controller->getKeyStatus(keycode).value;
	//		//log("AXIS_RIGHT_TRIGGER.sRightTrigger  = %d", _gamepadstate.sRightTrigger);
	//		break;
	//	default:
	//		return;
	//	}
	//	if (memcmp(&lastGamepadstate, &_gamepadstate, sizeof(_gamepadstate)) != 0)
	//	{
	//		sendMsg(_gamepadstate);
	//	}
	//};
	//getEventDispatcher()->addEventListenerWithFixedPriority(m_controller, 1);


	////two times exit
	//auto listener = EventListenerKeyboard::create();
	//listener->onKeyPressed = CC_CALLBACK_2(MyVgcScene::keyboardKeyPressedCallback);
	//getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener);

	//Button *btn = Button::create();
	//btn->setScale9Enabled(true);
	//btn->loadTextureNormal("green_normal_button.9.png");
	//btn->setContentSize(Size(200, 100));
	//btn->setTitleFontSize(FONT_SIZE);
	//btn->setTitleText(getLockScreen() ? "Lock" : "unLock");
	//btn->setSwallowTouches(true);
	//btn->setPosition(Vec2(visibleSize.width /2, visibleSize.height/2));
	//btn->addTouchEventListener([&](Ref*ref, Widget::TouchEventType type){
	//	if (type == Widget::TouchEventType::ENDED)
	//	{
	//		ui::Button *button = (ui::Button *)ref;
	//		setLockScreen(!getLockScreen());
	//		button->setTitleText(getLockScreen() ? "Lock" : "unLock");
	//	}
	//});
	//addChild(btn, TOP_ZODER);
	//receiveODSocket();
	return true;
}











void MyVgcScene::keyboardKeyPressedCallback(EventKeyboard::KeyCode code, Event *event)
{
	if (code == EventKeyboard::KeyCode::KEY_ESCAPE)
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

void MyVgcScene::sendMsg(ClientCore::XINPUT_GAMEPAD xinput_gamepad)
{
	std::string sendMsg = StringUtils::format("{\"Type\":\"Joystick\",\"Buttons\":%d,\"LeftTrigger\":%d,\"RightTrigger\":%d,\"ThumbLX\":%d,\"ThumbLY\":%d,\"ThumbRX\":%d,\"ThumbRY\":%d}",
		xinput_gamepad.sButtons, xinput_gamepad.sLeftTrigger, xinput_gamepad.sRightTrigger, xinput_gamepad.sThumbLX, xinput_gamepad.sThumbLY, xinput_gamepad.sThumbRX, xinput_gamepad.sThumbRY);

	log("socket::send(...): called: %s", sendMsg.c_str());
	if(1)return;
#if(CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	ODSocket csocket = SocketThread::GetInstance()->getSocket();
	char tHeader[2] = { 0 };
	int tLen = sendMsg.length();
	tHeader[0] = (char)(tLen >> 8);
	tHeader[1] = (char)tLen;
	int sendResult = csocket.Send(tHeader, 2, 0);
	log("sendResult = %d", sendResult);
	if (sendResult == -1 && !isShowDialog)
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]()
		{
			if (isShowDialog)
			{
				return;
			}
			XMLParser xmlParser;
			if (xmlParser.parseWithFile("string/string.xml"))
			{
				showDialog(xmlParser["disconnect_gloud_title"], xmlParser["disconnect_gloud_msg"], xmlParser["ok_button"], 90.0f);
			}
		});
	}
	else
	{
		csocket.Send(sendMsg.c_str(), sendMsg.length(), 0);
	}
#endif
}
ClientCore::dev_xinput::xinput_button MyVgcScene::getCode(Controller * controller, int keycode)
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

void MyVgcScene::onEnter()
{
	Layer::onEnter();
}


void MyVgcScene::setLockScreen(bool isLock)
{
	isLockScreen = isLock;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//c++ --> java : setScreenOrientationLandscape();
	JniMethodInfo info;
	log("setScreenOrientation()");
	bool isRet = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/AppActivity", "setScreenOrientation", "(Z)V");
	if (isRet)
	{
		log("setScreenOrientation(),,, ret");
		jboolean tmpIsLock = isLock;
		info.env->CallStaticVoidMethod(info.classID, info.methodID, tmpIsLock);
		info.env->DeleteLocalRef(info.classID);
}
#endif
}

bool MyVgcScene::getLockScreen()
{
	return isLockScreen;
}

void MyVgcScene::JniVibrate(long milliseconds)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//c++ --> java : onVibrator(long milliseconds);
	JniMethodInfo info;
	bool isRet = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/AppActivity", "onVibrator", "(J)V");
	if (isRet)
	{
		log("onVibrator(),,, ret %ld", milliseconds);
		jlong time = milliseconds;
		info.env->CallStaticVoidMethod(info.classID, info.methodID, time);

	}
#else
	ToastShow(StringUtils::format("shake %ld", milliseconds));
#endif
}


void MyVgcScene::receiveODSocket()
{
	//shake
	getEventDispatcher()->addCustomEventListener(CONTROLLER_SHOCK_MSG, [&](EventCustom * event)
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
			JniVibrate(VIBRATE_TIME_LONG);
		}
	});
}

