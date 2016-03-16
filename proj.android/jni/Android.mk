LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
			../../Classes/AppDelegate.cpp \
			../../Classes/BaseLayer.cpp \
			../../Classes/DeviceControllerScene.cpp \
			../../Classes/DeviceData.cpp \
			../../Classes/Dialog.cpp \
			../../Classes/Toast.cpp \
			../../Classes/FindDeviceScene.cpp \
			../../Classes/jsoncpp/src/json_reader.cpp \
			../../Classes/jsoncpp/src/json_value.cpp \
			../../Classes/jsoncpp/src/json_writer.cpp \
			../../Classes/MoreScene.cpp \
			../../Classes/PopupLayer.cpp \
			../../Classes/socket/ODSocket.cpp \
			../../Classes/socket/ResPonseThread.cpp \
			../../Classes/socket/SocketThread.cpp \
			../../Classes/TextTip.cpp \
			../../Classes/vgc/GameControllerButton.cpp \
			../../Classes/vgc/GameControllerDpad.cpp \
			../../Classes/vgc/GameControllerImage.cpp \
			../../Classes/vgc/GameControllerJoystick.cpp \
			../../Classes/vgc/GameControllerSwitch.cpp \
			../../Classes/vgc/GameControllerTrigger.cpp \
			../../Classes/vgc/VirtualGameController.cpp \
			../../Classes/VgcScene.cpp \
			../../Classes/XMLParser.cpp \
			../../Classes/JniAndroid.cpp \
			../../Classes/MyVgcScene.cpp \
			../../Classes/LaunchScene.cpp \
			../../Classes/UIPageViewStandard.cpp \
			../../Classes/GuideScene.cpp \
			../../Classes/SetupScene.cpp \
			../../Classes/LoadingLayer.cpp \
			../../Classes/StringUtil.cpp \
			../../Classes/UploadData.cpp
			

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
					$(LOCAL_PATH)/../../Classes/jsoncpp/include

LOCAL_STATIC_LIBRARIES := cocos2dx_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
