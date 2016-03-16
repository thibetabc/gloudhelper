#pragma once
#include "Global.h"
#include "JniAndroid.h"
#include "StringUtil.h"

class DeviceData : public JniAndroid,StringUtil
{
public:
	enum JMDNS_STRING
	{
		GLOUG_TAG,
		IP,
		PORT,
		DEVICE_NAME,
	};
	struct DataVector{
		std::string gloudTag;
		std::string ip;
		std::string port;
		std::string name;
	};
    
    enum PLATFORM
    {
        ANDROID,
        IOS
    };
public:
	DeviceData();
	~DeviceData();
	static DeviceData *getInstance();
	void destroyInstance();

	void pushBack(DataVector data);
	void popBack();
	void removeAll();
	std::vector<DataVector> getDataVector();
	CC_SYNTHESIZE(NetWorkState, networkState, NetworkState);
	DataVector splitString(std::string str);
	CC_SYNTHESIZE(bool, checkUpdate, CheckUpdate);
	std::string getChannelName();

	//小格
	CC_SYNTHESIZE(int, version, Version);
    CC_SYNTHESIZE(std::string, versionName, VersionName);
    CC_SYNTHESIZE(int, channel, Channel);
    CC_SYNTHESIZE(int, clientType, ClientType);

	//大厅
	CC_SYNTHESIZE(int, position, Position);
	CC_SYNTHESIZE(int, gloudVersion, GloudVersion);
	CC_SYNTHESIZE(int, gloudVersionNameInteger, GloudVersionNameInteger);
	CC_SYNTHESIZE(std::string, gloudVersionName, GloudVersionName);
	CC_SYNTHESIZE(std::string, gloudChannel, GloudChannel);
	CC_SYNTHESIZE(int, gloudAccountID, GloudAccountID);
	CC_SYNTHESIZE(std::string, gloudDeviceUUID, GloudDeviceUUID);
	CC_SYNTHESIZE(std::string, gloudLoginToken, GloudLoginToken);
private:
	std::vector<DataVector> m_dataVector;
	static DeviceData *g_getInstance;

};
