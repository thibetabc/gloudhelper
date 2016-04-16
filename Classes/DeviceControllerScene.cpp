#include "DeviceControllerScene.h"
#include "VgcScene.h"
#include "MoreScene.h"
#include "FindDeviceScene.h"
#include "socket/SocketThread.h"
#include "socket/ResPonseThread.h"
#include "DeviceData.h"


using namespace ui;
using namespace std;

DeviceControllerScene::DeviceControllerScene()
{
	lastNode = nullptr;
	exitTime = 0;
	sceneTag = HIDE_NO_FIND_TAG;
	mTextWifi = nullptr;
	xmlParser.parseWithFile("string/string.xml");
}
DeviceControllerScene::~DeviceControllerScene()
{
	getEventDispatcher()->removeCustomEventListeners(FIND_DEVICE_MSG);
	getEventDispatcher()->removeCustomEventListeners(CONNECT_DEVICE_MSG);
    getEventDispatcher()->removeCustomEventListeners(QR_VIEW_CONTROLLER_RESULT);
}
Scene *DeviceControllerScene::createScene()
{
	auto scene = Scene::create();
	auto layer = DeviceControllerScene::create();
	scene->addChild(layer);
	return scene;
}

bool DeviceControllerScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto m_deviceDataVector = DeviceData::getInstance()->getDataVector();
	log("m_deviceDataVector.size() = %d", m_deviceDataVector.size());

	auto layer = CSLoader::createNode("helper_device_layer.csb");
	auto layoutBG = layer->getChildByName("layout_bg");
	auto baseLayout = layoutBG->getChildByName("base_layout");
	//refind button
	auto buttonQr = (Button*)baseLayout->getChildByName("button_qr");
	//buttonQr->setVisible(false);//大厅1.3.6之前屏蔽扫码
	buttonQr->addClickEventListener([=](Ref *ref)
	{
//		JniOpenSanQr();
        std::vector<Contacts> mContacts;
        getPhoneContacts(mContacts);

	});
	//refind button
	auto refindButton = (Button*)baseLayout->getChildByName("paired_button");
	refindButton->addClickEventListener([=](Ref *ref)
	{
		Director::getInstance()->replaceScene(TransitionSlideInL::create(TRANSITION_TIME, FindDeviceScene::createScene()));
	});
	//open controller button
	auto downLayout = layoutBG->getChildByName("down_layout");
	auto openControllerButton = (Button*)downLayout->getChildByName("open_controller_button");
	CCLOG("openControllerButton Size [%.2f, %.2f]", openControllerButton->getContentSize().width, openControllerButton->getContentSize().height);
	openControllerButton->addClickEventListener([=](Ref* ref)
	{
		//gameSocket(m_deviceDataVector[index].ip.c_str(), stoi(m_deviceDataVector[index].port[0]));
		//10_0_4_251#9527_9528_9529_9530
		auto tLastNode = getNode();
		if (tLastNode)
		{
			addConnectingLayout();
			int tIndex = tLastNode->getTag();
			connect(m_deviceDataVector[tIndex].ip.c_str(), atoi(m_deviceDataVector[tIndex].port.c_str()));
		}
	});
	//disable
	openControllerButton->setBright(false);
	openControllerButton->setTouchEnabled(false);

	//ListView
	auto listviewLayout = baseLayout->getChildByName("listview_layout");
	auto listview = listviewLayout->getChildByName("listview");
    Size listviewSize = listview->getContentSize();
	
	if(m_deviceDataVector.size() > 0)
	{
		//find nothing...
		auto findNothingText = listviewLayout->getChildByName("find_nothing_text");
		findNothingText->setVisible(false);
		int buttonTag = 0;
		int selectLogoTag = 0;
		float delayTime = 0.0f;
		for (auto deviceData : m_deviceDataVector) {
			//button
			auto button = Button::create("normal_gray_button.9.png",
					"select_gray_button.9.png");
			button->setScale9Enabled(true);
            button->setContentSize(cocos2d::Size(listviewSize.width, 80));
			button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			//button->setPosition();
			//button->setTitleText(deviceData.name);
			//button->setTitleColor(Color3B::WHITE);
			button->setVisible(false);
			button->setTag(buttonTag++);
			delayTime = ++delayTime / 2.0f;
			button->runAction(Sequence::createWithTwoActions(DelayTime::create(delayTime), CallFunc::create([button](){button->setVisible(true); })));
			button->addTouchEventListener([=](Ref *ref, Widget::TouchEventType eventType)
			{
				if (Widget::TouchEventType::ENDED == eventType)
				{
					Button *tButton = (Button*)ref;
					int buttonTag = tButton->getTag();
					log("button->getTag = %d", buttonTag);
					//reset old select
					auto tLastNode = getNode();
					if (tLastNode)
					{
						tLastNode->setVisible(false);
					}
					//set new select
					auto select = button->getChildByTag(buttonTag);
					select->setVisible(true);
					setNode(select);

					//Enabled
					openControllerButton->setBright(true);
					openControllerButton->setTouchEnabled(true);

				}
			});

			//buttonSize
			cocos2d::Size buttonSize = button->getContentSize();

			//icon
			auto icon = Sprite::create("highlight_icon.png");
			//icon->setScale(0.5f);
			icon->setPosition(Vec2(buttonSize.width / 7.5, buttonSize.height / 2));
			button->addChild(icon);

			//name
			auto name = Text::create(deviceData.name, "", FONT_SIZE);
			name->setPosition(Vec2(buttonSize.width / 4.0f, buttonSize.height / 2));
			name->setColor(Color3B::BLACK);
			name->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
			cocos2d::Size nameSize = name->getContentSize();
			name->ignoreContentAdaptWithSize(false);
            name->setTextAreaSize(cocos2d::Size(342.0f, buttonSize.height));
			name->setTextVerticalAlignment(TextVAlignment::CENTER);
			button->addChild(name);

			//select logo
			auto selectLogo = Sprite::create("select_logo.png");
			selectLogo->setPosition(Vec2(buttonSize.width * 0.9f, buttonSize.height / 2));
			selectLogo->setVisible(false);
			selectLogo->setTag(selectLogoTag++);
			button->addChild(selectLogo);
			listview->addChild(button);
		}

	}
	
	
	//wifi
	mTextWifi = (Text*)layoutBG->getChildByName("wifi_layout")->getChildByName("wifi_text");
	setWifiState(false);
	

	

	//more info button
	auto moreInfoButton = (Button*)downLayout->getChildByName("more_info_button");
	CCLOG("moreInfoButton Size [%.2f, %.2f]", moreInfoButton->getContentSize().width, moreInfoButton->getContentSize().height);
	moreInfoButton->addClickEventListener([=](Ref *ref)
	{
		Director::getInstance()->replaceScene(TransitionSlideInR::create(TRANSITION_TIME, MoreScene::createScene()));
	});
	addChild(layer);

	//解析连接返回结果
	getEventDispatcher()->addCustomEventListener(FIND_DEVICE_MSG, [&](EventCustom * event)
	{
		rapidjson::Document *data = (rapidjson::Document *)event->getUserData();

		const rapidjson::Value &pCode = data->operator[]("Code");
		const rapidjson::Value &pPosition = data->operator[]("Position");
		int code = pCode.GetInt();
		int position = pPosition.GetInt();
		log("code = %d, position = %d", code, position);
		if (code == 0)
		{
			auto deviceData = DeviceData::getInstance();
			deviceData->setGloudVersionNameInteger(0);//避免扫描时没重置
			/*
			 * 1.2.0(含)之前的版本没有VersionName字段，
			 *  为了兼容大厅和小格旧版本，
			 *  收到 responseForRequest == 0 发送一条消息，
			 *  收到 responseForRequest =＝ 1 才进入手柄界面
			 */
			if(data->HasMember("ResponseForRequest"))
			{
				//VersionName获取数字再转int
				std::string versionName = get_all_number(data->operator[]("VersionName").GetString());
				int responseForRequest = data->operator[]("ResponseForRequest").GetInt();
				log("versionName = %s, responseForRequest = %d", versionName.c_str(), responseForRequest);
				if(versionName.size() > 0)
				{
					deviceData->setGloudVersionName(data->operator[]("VersionName").GetString());
					deviceData->setGloudVersionNameInteger(atoi(versionName.c_str()));
				}
				if (data->HasMember("Version"))
				{
					deviceData->setGloudVersion(data->operator[]("Version").GetUint());
				}
				if (data->HasMember("Channel"))
				{
					deviceData->setGloudChannel(data->operator[]("Channel").GetString());
				}
				if (data->HasMember("AccountID"))
				{
					deviceData->setGloudAccountID(data->operator[]("AccountID").GetInt());
				}
				if (data->HasMember("DeviceUUID"))
				{
					deviceData->setGloudDeviceUUID(data->operator[]("DeviceUUID").GetString());
				}
				if (data->HasMember("LoginToken"))
				{
					deviceData->setGloudLoginToken(data->operator[]("LoginToken").GetString());
				}
				if(responseForRequest == 0)
				{
					log("SocketThread::GetInstance()->state = %d", SocketThread::GetInstance()->state);
					if(0 == SocketThread::GetInstance()->state)
					{
						ODSocket csocket = SocketThread::GetInstance()->getSocket();
						std::string sendMsg = StringUtils::format("{\"Type\":\"LoginRequest\",\"Version\":%d,\"Name\":\"%s\"}",JniGetClientVersionCode(),JniGetPhoneModel().c_str());
						log("socket::send: %s", sendMsg.c_str());
						csocket.SendToJava(sendMsg.c_str(), sendMsg.length(), 0);
					}
				}
				else
				{
					deviceData->setPosition(position);
					Director::getInstance()->replaceScene(TransitionSlideInR::create(TRANSITION_TIME, VgcScene::createScene()));
				}
			}
			else
			{
				deviceData->setPosition(position);
				Director::getInstance()->replaceScene(TransitionSlideInR::create(TRANSITION_TIME, VgcScene::createScene()));
			}
		}
		else
		{
			//no position
			delConnectingLayout();
			showDialog(xmlParser["tip_title"], xmlParser["tip_msg"], xmlParser["ok_button"]);
		}
	});

	//提示连接失败
	getEventDispatcher()->addCustomEventListener(CONNECT_DEVICE_MSG, [&](EventCustom * event)
	{
		delConnectingLayout();
		showDialog(xmlParser["no_connect_title"], xmlParser["error_connect_msg"], xmlParser["ok_button"]);
	});


	//two times exit
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = [&](EventKeyboard::KeyCode code, Event *event){
		if (code == EventKeyboard::KeyCode::KEY_ESCAPE)
		{
			if ((millisecondNow() - exitTime) > 2000L)
			{
				exitTime = millisecondNow();
				ToastShow(xmlParser["exit_tip"],0.5f, -90.0f);
			}
			else
			{
				Director::getInstance()->end();
			}
		}
	};
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	this->setName(DEVICE_CONTROLLER_SCENE);
	return true;
}
void DeviceControllerScene::onEnter()
{
	Layer::onEnter();
	if (sceneTag == SHOW_NO_FIND_TAG)
	{
		if (DeviceData::getInstance()->getDataVector().size() <= 0)
		{

			//no find anything...
			auto findoutLayer = CSLoader::createNode("findout_layer.csb");
			auto findoutLayout = (Layout*)findoutLayer->getChildByName("bg_layout")->getChildByName("findout_layout");
			auto skipButton = (Button*)findoutLayout->getChildByName("skip_button");
			auto retryButton = (Button*)findoutLayout->getChildByName("retry_button");

			auto title = (Text*)findoutLayout->getChildByName("title_text");
			auto msgInfo = (Text*)findoutLayout->getChildByName("info_text");

			title->setString(xmlParser["timeout_title"]);
			msgInfo->setString(xmlParser["timeout_msg"]);
			skipButton->setTitleText(xmlParser["timeout_cancel"]);
			retryButton->setTitleText(xmlParser["timeout_ok"]);

			findoutLayout->setTouchEnabled(true);
			findoutLayout->setSwallowTouches(true);
			findoutLayout->addClickEventListener([=](Ref *ref)
			{
				log("findoutLayout->addClickEventListener(...) called");
			});
			skipButton->addClickEventListener([=](Ref *ref)
			{
				findoutLayer->removeFromParent();
			});
			retryButton->addClickEventListener([=](Ref *ref)
			{
				Director::getInstance()->replaceScene(TransitionSlideInR::create(TRANSITION_TIME, FindDeviceScene::createScene()));
			});
			addChild(findoutLayer, TOP_ZODER);
		}
	}

}
void DeviceControllerScene::onExit()
{
	Layer::onExit();
}
void DeviceControllerScene::setNode(Node *node)
{
	lastNode = node;
}
Node *DeviceControllerScene::getNode()
{
	return lastNode;
}


void DeviceControllerScene::connect(const char *ip, int port)
{
	SocketThread::GetInstance()->start(ip, port);
}


void DeviceControllerScene::addConnectingLayout()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto layout = Layout::create();
	layout->setContentSize(visibleSize);
	layout->setBackGroundColor(Color3B::BLACK);
	layout->setBackGroundColorOpacity(40);
	layout->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	layout->setTouchEnabled(true);
	layout->setSwallowTouches(true);
	layout->addClickEventListener([=](Ref *ref)
	{
        SocketThread::GetInstance()->stop();
        layout->removeFromParent();
		log("nowifiLayout->addClickEventListener(...) called");
	});
	auto connectingSprite = Sprite::create("connecting.png");
	connectingSprite->setPosition(Vec2(layout->getContentSize().width / 2, layout->getContentSize().height / 2));
	auto rotate = RotateBy::create(1.5f, 360.0f);
	connectingSprite->runAction(RepeatForever::create(rotate));
	connectingSprite->setScale(0.3f);
	layout->addChild(connectingSprite);
	addChild(layout, TOP_ZODER, CONNECTING_LAYOUT_NAME);
}

void DeviceControllerScene::delConnectingLayout()
{
	auto layout = this->getChildByName(CONNECTING_LAYOUT_NAME);
	if (layout)
	{
		layout->removeFromParent();
	}
}

void DeviceControllerScene::setWifiState(bool isUpdate)
{
	NetWorkState state;
	if (isUpdate)
	{
		state = JniGetConnectState();
		DeviceData::getInstance()->setNetworkState(state);
	}
	else
	{
		state = DeviceData::getInstance()->getNetworkState();
	}
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
		if (state == NetWorkState::WIFI)
		{
			mTextWifi->setString(StringUtils::format(xmlParser["text_wifi"].c_str(), JniGetWifiName().c_str()));
		}
		else if (state == NetWorkState::ETHERNET)
		{
			mTextWifi->setString("Ethernet");
		}
		else
		{
			mTextWifi->setString(StringUtils::format(xmlParser["text_wifi"].c_str(), xmlParser["no_wifi"].c_str()));
		}
	});
}


void DeviceControllerScene::dialogQrSan(int result, std::string strResult)
{

	DeviceData::DataVector data;
	if (result == 0){
		auto deviceData = DeviceData::getInstance();
		data = deviceData->splitString(strResult);
	}
	if (result != 0 || data.ip.length() <= 0)
	{
		ToastShow(xmlParser["text_qr_scan_error"], 2.0f, -90.0f);
		return;
	}
	//no find anything...
	auto findoutLayer = CSLoader::createNode("findout_layer.csb");
	auto findoutLayout = (Layout*)findoutLayer->getChildByName("bg_layout")->getChildByName("findout_layout");
	auto skipButton = (Button*)findoutLayout->getChildByName("skip_button");
	auto retryButton = (Button*)findoutLayout->getChildByName("retry_button");

	auto title = (Text*)findoutLayout->getChildByName("title_text");
	auto msgInfo = (Text*)findoutLayout->getChildByName("info_text");
	title->setString(xmlParser["title_qr_scan"]);
	msgInfo->setString(StringUtils::format(xmlParser["text_qr_scan_msg"].c_str(), data.name.c_str()));
	skipButton->setTitleText(xmlParser["cancel_button"]);
	retryButton->setTitleText(xmlParser["ok_button"]);
	findoutLayout->setTouchEnabled(true);
	findoutLayout->setSwallowTouches(true);
	findoutLayout->addClickEventListener([=](Ref *ref)
	{
		log("findoutLayout->addClickEventListener(...) called");
	});
	skipButton->addClickEventListener([=](Ref *ref)
	{
		findoutLayer->removeFromParent();
	});
	retryButton->addClickEventListener([=](Ref *ref)
	{
		findoutLayer->removeFromParent();
		addConnectingLayout();
		connect(data.ip.c_str(), atoi(data.port.c_str()));
	});
	addChild(findoutLayer, TOP_ZODER);

}
