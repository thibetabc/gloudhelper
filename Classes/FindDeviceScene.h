#ifndef __H_FINDDEVICESCENE_H__
#define __H_FINDDEVICESCENE_H__
#include "Global.h"
#include "BaseLayer.h"
#include "network/HttpClient.h"
//#include "../extensions/cocos-ext.h"
#include "JniAndroid.h"
#include "Toast.h"
using namespace network;
//USING_NS_CC_EXT;

class FindDeviceScene : public BaseLayer, JniAndroid,Toast
{
public:
	FindDeviceScene();
	~FindDeviceScene();
	static Scene *createScene();
	virtual bool init();
	CREATE_FUNC(FindDeviceScene);
	void addMsgJmDNS(std::string info);
	void replaceScene(bool isExitSearch);

private:
	void findDevice(std::string ip, std::string port, std::string name);
	void checkWifi(float dt);
public:
	//download
	void checkUpdate();
	void onHttpRequestCompleted(HttpClient *sender, HttpResponse *response);


private:
	int m_clientVersion;
	Layer *layerNoWifi;
	XMLParser xmlParser;
	

};

#endif //__H_FINDDEVICESCENE_H__
