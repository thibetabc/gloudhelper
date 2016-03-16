#ifndef __GLOBAL_H__
#define __GLOBAL_H__

//// system header
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <pthread.h>
#include <vector>
// cocos2dx header
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "jsoncpp/include/json/json.h"
#include "XMLParser.h"

USING_NS_CC;
// system specific definition
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <IPHlpApi.h>
#define usleep(time) Sleep(time / 1000)
#define COCOS2DX_DESIGNED_FPS 30
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <unistd.h>
#include <sys/socket.h>
#include <linux/in.h>
#include <asm/ioctls.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <android/native_window_jni.h>
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#endif
// pthread library
#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib, "pthreadd")
#else
#pragma comment(lib, "pthread")
#endif



#define COCOS2DX_DESIGNED_WIDTH     1280
#define COCOS2DX_DESIGNED_HEIGHT    720
#define GLOUD_API_URL_PREFIX        "http://c4a.51ias.com/api.php"
#define JOYSTICK_MULTIPLE			0.99
#define FONT_SIZE 36
#define TRANSITION_TIME 0.3f
#define TOP_ZODER 9999999
#define FIND_DEVICE_SCENE_TAG  10162015
//socket msg
#define FIND_DEVICE_MSG "find_device_msg"
#define CONTROLLER_JOYSTICK_MSG "controller_joystick_msg"
#define CONTROLLER_SHOCK_MSG "controller_shock_msg"
#define CUSTOM_SWAP_POSITION_MSG "custom_swap_position_msg"
#define CONNECT_DEVICE_MSG "connect_device_msg"
#define REPORT_JOYSTICK_STATUS_MSG "report_joystick_status_msg"

//pass parameter
#define FIND_DEVICE_SCENE "find_device_scene" 
#define DEVICE_CONTROLLER_SCENE "device_controller_scene"

//#define EPSINON 0.1f //float误差值
//#define VGC_SCENE_ROTATION_LEFT 90  //rotation scene_left; defalut: scene_left
//#define VGC_SCENE_ROTATION_RIGHT -90  //rotation scene_right
//#define SCENE_ROTATION_KEY "scene_rotation_key"

#define SPECIAL_DOWNLOAD_TAG 1 // 1:官方带下载链接gloud; 0:特殊渠道不带下载链接gloudc。数据要在data/data.xml修改
#define USER_DEFAULT_GUIDE_KEY "user_default_guide_key"
#define USER_DEFAULT_PHONE_VIBRATE_KEY "user_default_phone_vibrate_key"
#define USER_DEFAULT_GAME_VIBRATE_KEY "user_default_game_vibrate_key"
#define USER_DEFAULT_SCREEN_ORIENTATION_KEY "user_default_screen_orientation_key"

//vibrate_time
#define VIBRATE_CANCEL -1L
#define VIBRATE_TIME_SHORT 30L
#define VIBRATE_TIME 100L
#define VIBRATE_TIME_LONG 5000L

#define GLOUD_GAME_VERSION 134 //134不支持更换位置


#define QR_VIEW_CONTROLLER_RESULT "qr_view_controller_result"
#define NET_SERVICE_BROWSER_OVER "NET_SERVICE_BROWSER_OVER"

#define CLIENT_TYPE_ANDROID            0  //android
#define CLIENT_TYPE_WIN32              1  //win32
#define CLIENT_TYPE_LINUX              2  //linux
#define CLIENT_TYPE_IOS_APP_STORE      3  //ios 商店
#define CLIENT_TYPE_IOS_ENTERPRISE     4  //ios 企业版

#endif
