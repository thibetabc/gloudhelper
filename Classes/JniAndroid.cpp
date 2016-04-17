#if(CC_TARGET_PLATFORM != CC_PLATFORM_IOS)

#include "JniAndroid.h"
#include "FindDeviceScene.h"
#include "socket/SocketThread.h"
#include "DeviceControllerScene.h"

JniAndroid::JniAndroid()
{
	mJniRequestedOrientation = SCREEN_ORIENTATION_UNSPECIFIED;
}

JniAndroid::~JniAndroid()
{
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

/*
 * java  ---call---> c++ 
*/
extern "C"
{
	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_StartJmDNS_addMsgJmDNS(JNIEnv *env, jobject thiz, jstring message)
	{
		//addMsgJmDNS(); c++
		std::string tmpMessage = JniHelper::jstring2string(message);
		auto scene = Director::getInstance()->getRunningScene();
		auto layer = (FindDeviceScene *)scene->getChildByTag(FIND_DEVICE_SCENE_TAG);
		if (layer)
		{
			layer->addMsgJmDNS(tmpMessage);
		}
	}

	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_StartJmDNS_createSceneJNI(JNIEnv *env, jobject thiz)
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([]() {
			auto scene = Director::getInstance()->getRunningScene();
			auto layer = (FindDeviceScene *)scene->getChildByTag(FIND_DEVICE_SCENE_TAG);
			if (layer)
			{
				layer->replaceScene(false);
			}
		});

	}


	JNIEXPORT void JNICALL Java_cn_gloud_client_zxing_view_ScanQrActivity_JniCreateVgcScene(JNIEnv *env, jobject thiz, jint result, jstring str)
	{
		std::string strResult = JniHelper::jstring2string(str);
		auto scene = Director::getInstance()->getRunningScene();
		auto layer = (DeviceControllerScene *)scene->getChildByName(DEVICE_CONTROLLER_SCENE);
		if (layer)
		{
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {

				layer->dialogQrSan((int)result, strResult);
			});
		}
	}

	JNIEXPORT bool JNICALL Java_org_cocos2dx_cpp_StartJmDNS_JniIsStopSearch(JNIEnv *env, jobject thiz)
	{
		//JniIsStopSearch(); c++
		auto scene = Director::getInstance()->getRunningScene();
		auto layer = (FindDeviceScene *)scene->getChildByTag(FIND_DEVICE_SCENE_TAG);
		if (layer)
		{
			return false;
		}
		return true;
	}
}

/*
 *  c++  ---call---->  java
*/

void JniAndroid::JniSetRequestedOrientation(int requestedOrientation)
{
	//c++ --> java : setScreenOrientation();
	JniMethodInfo info;
	bool isRet = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/AppActivity", "setScreenOrientation", "(I)V");
	if (isRet)
	{
		jint tmpRequestedOrientation = requestedOrientation;
		info.env->CallStaticVoidMethod(info.classID, info.methodID, tmpRequestedOrientation);
		info.env->DeleteLocalRef(info.classID);
	}
	mJniRequestedOrientation = requestedOrientation;
}

int JniAndroid::JniGetRequestedOrientation()
{
	//c++ --> java : setScreenOrientation();
	JniMethodInfo info;
	bool isRet = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/AppActivity", "getScreenOrientation", "()I");
	if (isRet)
	{
		mJniRequestedOrientation = (jint)info.env->CallStaticIntMethod(info.classID, info.methodID);
		info.env->DeleteLocalRef(info.classID);
	}
	return mJniRequestedOrientation;
}

void JniAndroid::JniStartJmdns()
{
	JniMethodInfo info;
	bool isRet = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/AppActivity", "startJmDNS", "()V");
	if (isRet)
	{
		info.env->CallStaticVoidMethod(info.classID, info.methodID);
	}
}
void JniAndroid::JnidownloadHelper(const char *url)
{

	log("InstallHelperApk () ");
	JniMethodInfo info;
	bool ret = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/AppActivity", "InstallHelperApk", "(Ljava/lang/String;)V");
	if (ret)
	{
		jstring jurl = info.env->NewStringUTF(url);
		info.env->CallStaticVoidMethod(info.classID, info.methodID, jurl);
		info.env->DeleteLocalRef(info.classID);
		info.env->DeleteLocalRef(jurl);
	}

}
int JniAndroid::JniGetClientVersionCode()
{
	int version = 0;
	JniMethodInfo info;
	bool ret = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/AppActivity", "getClientVersionCode", "()I");
	if (ret)
	{
		version = (jint)info.env->CallStaticIntMethod(info.classID, info.methodID);
		info.env->DeleteLocalRef(info.classID);
	}

	log("JniGetClientVersionCode() = %d", version);
	return version;
}

std::string JniAndroid::JniGetClientVersionName()
{
	std::string versionName = "";
	JniMethodInfo info;
	bool ret = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/AppActivity", "getClientVersionName", "()Ljava/lang/String;");
	if (ret)
	{
		jstring name = (jstring)info.env->CallStaticObjectMethod(info.classID, info.methodID);
		versionName = JniHelper::jstring2string(name);
		info.env->DeleteLocalRef(info.classID);
	}

	return versionName;
}

std::string JniAndroid::JniGetWifiName()
{
	std::string name = "";
	JniMethodInfo info;
	bool isRet = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/AppActivity", "getConnectWifiName", "()Ljava/lang/String;");
	if (isRet)
	{
		jstring tmpWifiName = (jstring)info.env->CallStaticObjectMethod(info.classID, info.methodID);
		name = JniHelper::jstring2string(tmpWifiName);
	}
	return name;

}



void JniAndroid::JniVibrate(long milliseconds)
{
	JniMethodInfo info;
	bool isRet = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/AppActivity", "onVibrator", "(J)V");
	if (isRet)
	{
		//log("onVibrator(),,, ret %ld", milliseconds);
		jlong time = milliseconds;
		info.env->CallStaticVoidMethod(info.classID, info.methodID, time);

	}
}

void JniAndroid::JniDownload()
{
	log("android version ! go to [ Gloud Game ] download ");
	JniMethodInfo info;
	bool ret = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/AppActivity", "InstallApk", "(Ljava/lang/String;)V");
	if (ret)
	{
		jstring url = info.env->NewStringUTF("");
		info.env->CallStaticVoidMethod(info.classID, info.methodID, url);
		info.env->DeleteLocalRef(info.classID);
		info.env->DeleteLocalRef(url);
	}
}


bool JniAndroid::JniIsInstalledApk()
{
	bool isInstalled = false;
	log("JniIsInstalledApk()");
	JniMethodInfo info;
	bool ret = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/AppActivity", "isInstalledApk", "()Z");
	if (ret)
	{
		isInstalled = (jboolean)info.env->CallStaticBooleanMethod(info.classID, info.methodID);
		info.env->DeleteLocalRef(info.classID);
	}
	return isInstalled;

}
void JniAndroid::JniOpenApk()
{
	log("JniOpenApk()");
	JniMethodInfo info;
	bool ret = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/AppActivity", "startApp", "()V");
	if (ret)
	{
		info.env->CallStaticVoidMethod(info.classID, info.methodID);
		info.env->DeleteLocalRef(info.classID);
	}

}

void JniAndroid::JniSetupWifi()
{
	JniMethodInfo info;
	bool isRet = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/AppActivity", "setupWifi", "()V");
	if (isRet)
	{
		info.env->CallStaticVoidMethod(info.classID, info.methodID);
	}
}
JniAndroid::NetWorkState JniAndroid::JniGetConnectState()
{
	JniMethodInfo info;
	bool isRet = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/AppActivity", "getConnectState", "()I");
	if (isRet)
	{
		jint result = info.env->CallStaticIntMethod(info.classID, info.methodID);
		log("result = %d", result);
		switch(result)
		{
		case 0:
			return NetWorkState::ETHERNET;
		case 1:
			return NetWorkState::WIFI;
		case 2:
			return NetWorkState::MOBILE;
		case 3:
			return NetWorkState::NONE;
		}


	}
	return NetWorkState::NONE;
}

void JniAndroid::JniOpenSanQr()
{
	JniMethodInfo info;
	bool isRet = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/AppActivity", "openSanQr", "()V");
	if (isRet)
	{
		info.env->CallStaticVoidMethod(info.classID, info.methodID);
	}

	//JniMethodInfo info;
	//jobject activityObj;
	//bool isRet = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/AppActivity", "getObject", "()Ljava/lang/Object;");
	//if (isRet)
	//{
	//	activityObj = info.env->CallStaticObjectMethod(info.classID, info.methodID);
	//	isRet = JniHelper::getMethodInfo(info, "org/cocos2dx/cpp/AppActivity", "openSanQr", "()V");
	//	if(isRet)
	//	{
	//		info.env->CallVoidMethod(activityObj, info.methodID);
	//	}
	//}
}
std::string JniAndroid::JniGetPhoneModel()
{
	std::string phoneModel = "--";
	JniMethodInfo info;
	bool isRet = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/AppActivity", "getPhoneModel", "()Ljava/lang/String;");
	if (isRet)
	{
		jstring jPhoneModel = (jstring)info.env->CallStaticObjectMethod(info.classID, info.methodID);
		phoneModel = JniHelper::jstring2string(jPhoneModel);
	}
	return phoneModel;
}

void JniAndroid::getPhoneContacts(std::vector<Contacts> &mContacts)
{
    bool isRet = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/AppActivity", "testReadAllContacts", "()V");
    if (isRet)
    {
        info.env->CallStaticObjectMethod(info.classID, info.methodID);
    }
}
//http://www.mamicode.com/info-detail-503936.html
//http://www.cnblogs.com/zenny-chen/p/4273174.html
/******************************************************************************************************************************************************************/



#else   //win32

void JniAndroid::JniSetRequestedOrientation(int requestedOrientation)
{
	mJniRequestedOrientation = requestedOrientation;
}

int JniAndroid::JniGetRequestedOrientation()
{
	return mJniRequestedOrientation;
}

void JniAndroid::JniStartJmdns()
{
}
void JniAndroid::JnidownloadHelper(const char *url)
{
}
int JniAndroid::JniGetClientVersionCode()
{
	return 20160126;
}
std::string JniAndroid::JniGetClientVersionName()
{
	return "1.2.1";
}
std::string JniAndroid::JniGetWifiName()
{
	return "";
}
void JniAndroid::JniVibrate(long milliseconds)
{
}
void JniAndroid::JniDownload()
{
}
bool JniAndroid::JniIsInstalledApk()
{
	return false;
}
void JniAndroid::JniOpenApk()
{
}

void JniAndroid::JniSetupWifi()
{
}
JniAndroid::NetWorkState JniAndroid::JniGetConnectState()
{
	return NetWorkState::WIFI;
}

void JniAndroid::JniOpenSanQr()
{

}
std::string JniAndroid::JniGetPhoneModel()
{
	return "--";
}


#endif

#endif
