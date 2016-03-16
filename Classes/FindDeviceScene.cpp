#include "FindDeviceScene.h"
#include "DeviceControllerScene.h"
#include "MoreScene.h"
#include "DeviceData.h"
#include "VgcScene.h"
#include "UploadData.h"

using namespace ui;

FindDeviceScene::FindDeviceScene()
{
	DeviceData::getInstance()->removeAll();
	layerNoWifi = nullptr;
	xmlParser.parseWithFile("string/string.xml");
}
FindDeviceScene::~FindDeviceScene()
{

}
Scene *FindDeviceScene::createScene()
{
	Scene *scene = Scene::create();
	FindDeviceScene *layer = FindDeviceScene::create();
	layer->setTag(FIND_DEVICE_SCENE_TAG);
	scene->addChild(layer);
	return scene;
}
bool FindDeviceScene::init()
{
	if (!BaseLayer::init())
	{
		return false;
	}

	//loading_bg
	auto loadingBg = Sprite::create("loading_bg.png");
	loadingBg->setPosition(Size(mBaseLayout->getContentSize().width / 2, mBaseLayout->getContentSize().height / 2));
	mBaseLayout->addChild(loadingBg);

	//loading_bar
	auto loadingImg = Sprite::create("loading_bar.png");
	loadingImg->setPosition(Size(loadingBg->getContentSize().width / 2, loadingBg->getContentSize().height / 2));
	loadingBg->addChild(loadingImg, 100);
	auto rotate = RotateBy::create(2.0f, 360.0f);
	loadingImg->runAction(RepeatForever::create(rotate));

	//tip_txt
    mTipText->setString(xmlParser["find_device"]);
    scheduleOnce(schedule_selector(FindDeviceScene::checkWifi), 1);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = [&](EventKeyboard::KeyCode code, Event *event){
        if (code == EventKeyboard::KeyCode::KEY_ESCAPE)
        {
            if (layerNoWifi)
            {
                layerNoWifi->removeFromParent();
                layerNoWifi = nullptr;
                JniStartJmdns();
            }
            else
            {
                replaceScene(true);
            }
        }
    };
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
#endif
    
	return true;
}


void FindDeviceScene::findDevice(std::string ip, std::string port,std::string name)
{
	DeviceData::DataVector deviceData;
	deviceData.ip = ip;
	deviceData.port = port;
	deviceData.name = name;
	DeviceData::getInstance()->pushBack(deviceData);
}

void FindDeviceScene::replaceScene(bool isExitSearch)
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	addMsgJmDNS("GloudGame#10_0_1_123#9627#MAGICBOX");
	addMsgJmDNS("GloudGame#10_0_1_78#9627#YK_K1");
	//Director::getInstance()->replaceScene(TransitionCrossFade::create(TRANSITION_TIME, VgcScene::createScene()));

	auto scene = DeviceControllerScene::createScene();
	auto layer = (DeviceControllerScene*)scene->getChildByName(DEVICE_CONTROLLER_SCENE);
	Director::getInstance()->replaceScene(TransitionSlideInR::create(TRANSITION_TIME, scene));
#else
	auto scene = DeviceControllerScene::createScene();
	auto layer = (DeviceControllerScene*)scene->getChildByName(DEVICE_CONTROLLER_SCENE);
	if(!isExitSearch)
	{
		layer->sceneTag = layer->SHOW_NO_FIND_TAG;
	}
	Director::getInstance()->replaceScene(TransitionCrossFade::create(TRANSITION_TIME, scene));
#endif

}
void FindDeviceScene::checkWifi(float dt)
{
	NetWorkState state = JniGetConnectState();
	DeviceData::getInstance()->setNetworkState(state);
	if (state == NetWorkState::WIFI || state == NetWorkState::ETHERNET)
	{
		log("ok, is Wifi");
		checkUpdate();
	}
	else
	{
		log("sorry, is not Wifi");
		layerNoWifi = (Layer *)CSLoader::createNode("no_wifi_layer.csb");
		auto layoutBg = (Layout*)layerNoWifi->getChildByName("bg_layout");
		layoutBg->setTouchEnabled(true);
		layoutBg->setSwallowTouches(true);
		layoutBg->addClickEventListener([=](Ref *ref)
		{
			layerNoWifi->removeFromParent();
			layerNoWifi = nullptr;
			log("layoutBg->addClickEventListener(...) called");
			checkUpdate();
		});
		auto nowifiLayout = (Layout*)layoutBg->getChildByName("bg_image");
		auto setupButton = (Button*)nowifiLayout->getChildByName("setup_button");
		//nowifiLayout->setTouchEnabled(true);
		//nowifiLayout->setSwallowTouches(true);
		//nowifiLayout->addClickEventListener([=](Ref *ref)
		//{
		//	log("nowifiLayout->addClickEventListener(...) called");
		//});

		setupButton->addClickEventListener([=](Ref *ref)
		{
			layerNoWifi->removeFromParent();
			layerNoWifi = nullptr;
			JniStartJmdns();
			JniSetupWifi();
		});
		addChild(layerNoWifi, TOP_ZODER);
	}
}

void FindDeviceScene::addMsgJmDNS(std::string info)
{
	log("addMsgJmDNS(...), info = %s", info.c_str());
	auto deviceData = DeviceData::getInstance();
	deviceData->pushBack(deviceData->splitString(info));
}


void FindDeviceScene::checkUpdate()
{
	bool isCheck = DeviceData::getInstance()->getCheckUpdate();
	if (isCheck)
	{
		DeviceData::getInstance()->setCheckUpdate(false);
		m_clientVersion = JniGetClientVersionCode();
		XMLParser xmlParser;
		xmlParser.parseWithFile("data/data.xml");
		int flag = atoi(xmlParser["download_gloud_flags"].c_str());
        int clientType = atoi(xmlParser["client_type"].c_str());
        DeviceData::getInstance()->setChannel(flag);
        DeviceData::getInstance()->setClientType(clientType);
		UploadData::getInstance()->upload(UploadData::TYPE::END);
	}
	if (isCheck && m_clientVersion > 0)
	{
		this->pause();
		/*
		pid: 官方带下载链接的包渠道号：gloud; 特殊渠道不带下载链接的包渠道号：gloudc
		ver: 当前版本
		product： 3表示小格助手
		client_type： android渠道
		*/
		std::string url = StringUtils::format("http://b2.51ias.com/api.php?a=client_ver&m=Anony&pid=%s&ver=%d&product=3&client_type=%d", DeviceData::getInstance()->getChannelName().c_str(), m_clientVersion, DeviceData::getInstance()->getClientType());
		log("HttpRequest URL===> %s", url.c_str());
		HttpRequest* request = new (std::nothrow) HttpRequest();
		request->setUrl(url.c_str());
		//request->setRequestType(HttpRequest::Type::DELETE);
		request->setRequestData(nullptr, 0);
		request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
		request->setResponseCallback(this,httpresponse_selector(FindDeviceScene::onHttpRequestCompleted));
		HttpClient * client = HttpClient::getInstance();
		client->setTimeoutForConnect(2);
		client->setTimeoutForRead(2);
		client->sendImmediate(request);
		request->release();
	}
	else
	{
		JniStartJmdns();
	}
}

void FindDeviceScene::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
{
	bool isError = true;
	if (!response)
	{
		isError = false;
	}
	if (!response->isSucceed())
	{
		log("error buffer: %s", response->getErrorBuffer());
		isError = false;
	}
	if (isError)
	{
		// dump data
		std::vector<char> *buffer = response->getResponseData();
		std::string resultJson = "";
		for (unsigned int i = 0; i < buffer->size(); i++)
		{
			resultJson += (*buffer)[i];
		}
		log("Http json = %s", resultJson.c_str());

		int curVer = 0;
		int latestVer = 0;
		int forceUpdate = 0; //forceUpdate:2
		std::string url = "";
		std::string updateInfo = "";
		//json
		do
		{

			rapidjson::Document *_doc = new rapidjson::Document;
			_doc->Parse<0>(resultJson.c_str());
			CC_BREAK_IF(_doc->HasParseError());
			if (!_doc->IsObject())
				break;
			if (_doc->HasMember("ver"))
			{

				const rapidjson::Value &pType = _doc->operator[]("ver");
				if (pType.HasMember("current") && pType.HasMember("latest"))
				{
					const rapidjson::Value &valueCurrent = pType["current"];
					const rapidjson::Value &valueLatest = pType["latest"];
					curVer = valueCurrent["ver"].IsString() ? atoi(valueCurrent["ver"].GetString()) : valueCurrent["ver"].GetInt();
					latestVer = valueLatest["ver"].IsString() ? atoi(valueLatest["ver"].GetString()) : valueLatest["ver"].GetInt();
					forceUpdate = valueLatest["force_update"].IsString() ? atoi(valueLatest["force_update"].GetString()) : valueLatest["force_update"].GetInt();
					url = valueLatest["url"].GetString();
					updateInfo = valueLatest["desc"].GetString();
					log("latestVer = %d, curVer = %d, updateUrl = %s", latestVer, curVer, url.c_str());
				}
			}
		} while (0);

		//new version
		if (latestVer > m_clientVersion && latestVer > curVer)
		{

			auto findoutLayer = CSLoader::createNode("update_layer.csb");
			auto findoutLayout = (Layout*)findoutLayer->getChildByName("bg_layout")->getChildByName("layout_info_bg");
			auto skipButton = (Button*)findoutLayout->getChildByName("skip_button");
			auto retryButton = (Button*)findoutLayout->getChildByName("retry_button");
			auto updateMsg = (Text *)findoutLayout->getChildByName("info_text");
			updateMsg->setString(updateInfo);
			findoutLayout->setTouchEnabled(true);
			findoutLayout->setSwallowTouches(true);
			findoutLayout->addClickEventListener([=](Ref *ref)
			{
				log("findoutLayout->addClickEventListener(...) called");
			});
			skipButton->addClickEventListener([=](Ref *ref)
			{
				//forceUpdate
				if (forceUpdate == 2)
				{
					Director::getInstance()->end();
				}
				else
				{
					this->resume();
					findoutLayer->removeFromParent();
					JniStartJmdns();
				}
			});
			retryButton->addClickEventListener([=](Ref *ref)
			{
				findoutLayer->removeFromParent();
				this->resume();
				if (url.length() > 10)
				{
					JnidownloadHelper(url.c_str());
				}
				else
				{
					ToastShow("download url is error", 0.5f, -90.0f);
					JniStartJmdns();
				}
			});
			addChild(findoutLayer, TOP_ZODER);

		}
		else
		{
			this->resume();
			JniStartJmdns();
		}
	}
	else
	{
		this->resume();
		JniStartJmdns();
	}
}

