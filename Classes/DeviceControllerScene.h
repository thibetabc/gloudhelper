#ifndef __H_DEVICECONTROLLERSCENE_H__
#define __H_DEVICECONTROLLERSCENE_H__
#include "Global.h"
#include "Dialog.h"
#include "Toast.h"
#include "JniAndroid.h"
#include "StringUtil.h"

class DeviceControllerScene : public Dialog, Toast, JniAndroid, StringUtil
{
public:
	DeviceControllerScene();
	~DeviceControllerScene();
	CREATE_FUNC(DeviceControllerScene);
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	static Scene *createScene();
	void setWifiState(bool isUpdate);
	void dialogQrSan(int result, std::string strResult);

private:
	void setNode(Node *node);
	Node *getNode();
	void addConnectingLayout();
	void delConnectingLayout();
	void connect(const char *ip, int port);
public:
	int sceneTag;
	const int SHOW_NO_FIND_TAG = 1;//显示-没找到提示
	const int HIDE_NO_FIND_TAG = 0; //隐藏-没找到提示（了解更多界面返回 、按返回键退出搜索）
private:
	Node *lastNode;
	const std::string CONNECTING_LAYOUT_NAME = "connecting_layout_name";
	long long exitTime;
	ui::Text *mTextWifi;
	XMLParser xmlParser;

};
#endif //DeviceControllerScene
