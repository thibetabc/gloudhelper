#include "MoreScene.h"
#include "DeviceData.h"

DeviceData *DeviceData::g_getInstance = nullptr;
DeviceData::DeviceData()
{
	m_dataVector.clear();
 	memset(&m_dataVector, 0, sizeof(m_dataVector));
	position = 0;
	position = true;
	networkState = NetWorkState::NONE;
	gloudVersionNameInteger = 0;
	channel = 0;
	version = JniGetClientVersionCode();
	versionName = JniGetClientVersionName();
    clientType = CLIENT_TYPE_ANDROID;
    checkUpdate = true;
}
DeviceData::~DeviceData()
{
}

DeviceData *DeviceData::getInstance()
{
	if (!g_getInstance)
	{
		g_getInstance = new DeviceData();
	}
	return g_getInstance;
}
void DeviceData::destroyInstance()
{
	if (g_getInstance)
	{
		removeAll();
		delete g_getInstance;
		g_getInstance = nullptr;
	}
}

void DeviceData::pushBack(DataVector data)
{	
	if (!data.ip.empty())
	{
		m_dataVector.push_back(data);
	}
}

void DeviceData::popBack()
{
	m_dataVector.pop_back();
}
void DeviceData::removeAll()
{
	m_dataVector.clear();
}


std::vector<DeviceData::DataVector> DeviceData::getDataVector()
{
	return m_dataVector;

}

//str: GloudGame#10_0_1_132#9627#MAGICBOX
DeviceData::DataVector DeviceData::splitString(std::string str)
{
	DeviceData::DataVector deviceData;
	if (!str.empty())
	{
		std::vector<std::string> result = split(str, "#");
		if (result.size() == 4 && result[JMDNS_STRING::GLOUG_TAG].compare("GloudGame") == 0)
		{
			deviceData.gloudTag = result[JMDNS_STRING::GLOUG_TAG];
			deviceData.ip = replace_all_distinct(result[JMDNS_STRING::IP], "_", ".");
			deviceData.port = result[JMDNS_STRING::PORT];
			deviceData.name = result[JMDNS_STRING::DEVICE_NAME];
		}
	}
	return deviceData;
}

std::string DeviceData::getChannelName()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    // IOS更新
    // gloud：去应用商店更新.
    // gloudc：去企业发布的服务器更新.
    return clientType == CLIENT_TYPE_IOS_APP_STORE ? "gloud" : "gloudc";
#else
    //Android更新
	return channel == SPECIAL_DOWNLOAD_TAG ? "gloud" : "gloudc";
#endif
}
