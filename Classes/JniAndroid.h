#pragma once
#include "Global.h"

class JniAndroid
{

public:
	enum
	{
		SCREEN_ORIENTATION_UNSPECIFIED = -1,
		SCREEN_ORIENTATION_LANDSCAPE = 0,
		SCREEN_ORIENTATION_PORTRAIT,
		SCREEN_ORIENTATION_USER,
		SCREEN_ORIENTATION_BEHIND,
		SCREEN_ORIENTATION_SENSOR,
		SCREEN_ORIENTATION_NOSENSOR,
		SCREEN_ORIENTATION_SENSOR_LANDSCAPE,
		SCREEN_ORIENTATION_SENSOR_PORTRAIT,
		SCREEN_ORIENTATION_REVERSE_LANDSCAPE,
		SCREEN_ORIENTATION_REVERSE_PORTRAIT,
		SCREEN_ORIENTATION_FULL_SENSOR,
		SCREEN_ORIENTATION_USER_LANDSCAPE,
		SCREEN_ORIENTATION_USER_PORTRAIT,
		SCREEN_ORIENTATION_FULL_USER,
		SCREEN_ORIENTATION_LOCKED,
	};

	enum class NetWorkState
	{
		ETHERNET,
		WIFI,
		MOBILE,
        NetWorkType_2G,
        NetWorkType_3G,
        NetWorkType_4G,
		NONE
	};
public:
	JniAndroid();
	~JniAndroid();
	void JniSetRequestedOrientation(int requestedOrientation);
	int JniGetRequestedOrientation();
	int JniGetClientVersionCode();
	std::string JniGetClientVersionName();
	void JnidownloadHelper(const char *url);
	void JniStartJmdns();
    void JniRemoveJmdns();
    void JniCreateScene();
	std::string JniGetWifiName();
	void JniVibrate(long milliseconds);
	void JniDownload();
	bool JniIsInstalledApk();
	void JniOpenApk();
	void JniSetupWifi();
	NetWorkState JniGetConnectState();
	void JniOpenSanQr();
	std::string JniGetPhoneModel();
private:
	int mJniRequestedOrientation;
public:
    struct Contacts
    {
        std::string name;
        std::vector<std::string> strTelephone;
        
    };
public:
    void getPhoneContacts(std::vector<Contacts> &mContacts);
};
