#include "UploadData.h"
#include "DeviceData.h"

#define JSON_VALUE "JSON_VALUE"
#define DELAY_SCHEDULE 60

UploadData *UploadData::g_getInstance = nullptr;

UploadData::UploadData()
{
	startTime = 0LL;
	mNodeSchedule = nullptr;
	mType = NONE;
	durationTime = 0;
	
}
UploadData::~UploadData()
{
}

UploadData *UploadData::getInstance()
{
	if (!g_getInstance)
	{
		g_getInstance = new UploadData();
	}
	return g_getInstance;
}
void UploadData::destroyInstance()
{
	if (g_getInstance)
	{
		deleteSchedule();
		delete g_getInstance;
		g_getInstance = nullptr;
	}
}
void UploadData::startSchedule()
{
	if (!mNodeSchedule)
	{
		//std::chrono::steady_clock;//cpu time
		//std::chrono::system_clock; //system time
		durationTime = 0;
		mNodeSchedule = Node::create();
		mNodeSchedule->schedule([&](float dt){
			save();
		}, DELAY_SCHEDULE, "delaySchedule");
		mNodeSchedule->retain();
		mNodeSchedule->onEnter();
	}
}
void UploadData::deleteSchedule()
{	
	if (mNodeSchedule)
	{
		save();
		mNodeSchedule->onExit();
		mNodeSchedule->release();
		mNodeSchedule = nullptr;
	}
}
long UploadData::systemCurrentTime()
{
	struct timeval nowTimeval;
	gettimeofday(&nowTimeval, NULL);
	return nowTimeval.tv_sec;
}

long UploadData::systemCurrentTimeMillis()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void UploadData::systemTimestamp(std::string &strTimestamp)
{
	struct timeval nowTimeval;
	time_t time_sec;
	gettimeofday(&nowTimeval, NULL);
	time_sec = nowTimeval.tv_sec;
	struct tm *tm = localtime(&time_sec);
	char currentTime[1024];
	strftime(currentTime, sizeof(currentTime), "%Y-%m-%d %H:%M:%S", tm);
	strTimestamp = currentTime;
	log("systemTimestamp %s", strTimestamp.c_str());
	//log("nowTimeval.tv_sec[%ld],nowTimeval.tv_usec[%ld],%d-%02d-%02d %02d:%02d:%02d",
	//	nowTimeval.tv_sec,
	//	nowTimeval.tv_usec,
	//	tm->tm_year + 1900,
	//	tm->tm_mon + 1,
	//	tm->tm_mday,
	//	tm->tm_hour,
	//	tm->tm_min,
	//	tm->tm_sec);
	//return currentTime;

}

void UploadData::save()
{	
	durationTime += DELAY_SCHEDULE;
	auto deviceData = DeviceData::getInstance();
	std::string json = StringUtils::format("{\"helperVersionName\":\"%s\",\"helperChannel\":\"%s\",\"gloudVersion\":%d,\"gloudVersionName\":\"%s\",\"gloudVersionNameInteger\":%d,\"gloudChannel\":\"%s\",\"gloudAccountID\":%d,\"gloudDeviceUUID\":\"%s\",\"gloudLoginToken\":\"%s\",\"startTime\":%lld,\"endTime\":%lld,\"durationTime\":%lld}",
		deviceData->getVersionName().c_str(),
		deviceData->getChannelName().c_str(),
		deviceData->getGloudVersion(),
		deviceData->getGloudVersionName().c_str(),
		deviceData->getGloudVersionNameInteger(),
		deviceData->getGloudChannel().c_str(),
		deviceData->getGloudAccountID(),
		deviceData->getGloudDeviceUUID().c_str(),
		deviceData->getGloudLoginToken().c_str(),
		startTime,
		startTime + durationTime,
		durationTime);
	log("UploadData: save json ==> %s", json.c_str());
	UserDefault::getInstance()->setStringForKey(JSON_VALUE, json);
}

void UploadData::initUrl(std::string &url)
{
	std::string webUrl = "http://c4test2.51ias.com/api.php?m=Anony&a=accept_phonehelp_result&phonehelp_ver=%s&phonehelp_pid=%s&client_ver=%d&client_name=%s&client_pid=%s&account_id=%d&device_uuid=%s&logintoken=%s&start_time=%lld&continue_time=%lld&type=%d";
	switch (mType)
	{
	case UploadData::START:
	{
		auto deviceData = DeviceData::getInstance();
		url = StringUtils::format(webUrl.c_str(),
		deviceData->getVersionName().c_str(),
		deviceData->getChannelName().c_str(),
		deviceData->getGloudVersion(),
		deviceData->getGloudVersionName().c_str(),
		deviceData->getGloudChannel().c_str(),
		deviceData->getGloudAccountID(),
		deviceData->getGloudDeviceUUID().c_str(),
		deviceData->getGloudLoginToken().c_str(),
		0LL, 0LL, 1);
		log("UploadData: 开始===> %s", url.c_str());
	}
		break;
	case UploadData::END:
	{
		do
		{
			std::string jsStr = UserDefault::getInstance()->getStringForKey(JSON_VALUE, "");
			rapidjson::Document doc;
			doc.Parse<0>(jsStr.c_str());
			if (doc.HasParseError())
			{
				log("UploadData: HasParseError parse json error! jsStr = %s", jsStr.c_str());
				break;
			}
			if (doc.HasMember("helperVersionName") && doc.HasMember("gloudVersionName"))
			{
				//上传web的start_time就是endTime结束时间
				DeviceData::getInstance()->setGloudVersionNameInteger(doc["gloudVersionNameInteger"].GetInt());
				url = StringUtils::format(webUrl.c_str(),
				doc["helperVersionName"].GetString(),
				doc["helperChannel"].GetString(),
				doc["gloudVersion"].GetInt(),
				doc["gloudVersionName"].GetString(),
				doc["gloudChannel"].GetString(),
				doc["gloudAccountID"].GetInt(),
				doc["gloudDeviceUUID"].GetString(),
				doc["gloudLoginToken"].GetString(),
				doc["endTime"].GetInt64(),
				doc["durationTime"].GetInt64(),
				2);				
				log("UploadData: 结束或有缓存数据===> %s", url.c_str());
			}
		} while (0);
	}
		break;
	default:
		break;
	}
}

void UploadData::removeFile()
{
	UserDefault::getInstance()->setStringForKey(JSON_VALUE, "");
}

void UploadData::upload(TYPE type)
{
	std::string url;
	mType = type;
	initUrl(url);
	//大厅136以上
	if (DeviceData::getInstance()->getGloudVersionNameInteger() > GLOUD_GAME_VERSION + 1 && url.length() > 0)
	{
		network::HttpRequest* request = new (std::nothrow) network::HttpRequest();
		log("UploadData 正在上传 ===> %s", url.c_str());
		request->setUrl(url.c_str());
		request->setRequestData(nullptr, 0);
		request->setRequestType(network::HttpRequest::Type::GET);
		request->setResponseCallback(this, httpresponse_selector(UploadData::onHttpRequestCompleted));
		network::HttpClient * client = network::HttpClient::getInstance();
		client->setTimeoutForConnect(2);
		client->setTimeoutForRead(2);
		client->sendImmediate(request);
		request->release();
	}
}

void UploadData::onHttpRequestCompleted(network::HttpClient *sender, network::HttpResponse *response)
{
	bool isError = true;
	if (!response)
	{
		log("UploadData: error response == null");
		return;
	}
	if (!response->isSucceed())
	{
		log("UploadData: error buffer: %s", response->getErrorBuffer());
		return;
	}
	std::vector<char> *buffer = response->getResponseData();
	std::string resultJson = "";
	for (unsigned int i = 0; i < buffer->size(); i++)
	{
		resultJson += (*buffer)[i];
	}
	log("UploadData: Http json = %s", resultJson.c_str());
	do
	{

		rapidjson::Document _doc;
		_doc.Parse<0>(resultJson.c_str());
		if (_doc.HasParseError())
		{
			log("UploadData: HasParseError parse Http json error!");
			break;
		}
		if (!_doc.IsObject())
		{
			break;
		}
		if (_doc.HasMember("ret"))
		{
			if (_doc["ret"].GetInt() == 0)
			{
				startTime = _doc["data"].GetInt64();
				removeFile();
				log("UploadData: 上传成功, 删除缓存成功 startTime = %lld", startTime);
			}
		}
	} while (0);
}
