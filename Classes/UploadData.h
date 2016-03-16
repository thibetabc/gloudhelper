#pragma once
#include "Global.h"
#include "JniAndroid.h"
#include "network/HttpClient.h"


class UploadData : public JniAndroid, Ref
{
public:
	enum TYPE
	{
		NONE = 0, 
		START,    //开始
		END,      //结束
	};

public:
	UploadData();
	~UploadData();
	static UploadData *getInstance();
	void destroyInstance();
	void upload(TYPE type);
	void startSchedule();
	void deleteSchedule();

private:
	long systemCurrentTime();//系统时间-秒
	long systemCurrentTimeMillis();//系统时间-毫秒
	void systemTimestamp(std::string &strTimestamp);//时间戳
	void onHttpRequestCompleted(network::HttpClient *sender, network::HttpResponse *response);
	void removeFile();
	void initUrl(std::string &url);
	void save();

private:
	//结束时间 = web下发时间戳 + 持续时间
	long long startTime; //web下发时间戳
	long long durationTime; //持续时间
	static UploadData *g_getInstance;
	Node *mNodeSchedule;
	TYPE mType;
};
