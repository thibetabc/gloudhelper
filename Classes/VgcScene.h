#ifndef __H_VGCSCENE_H__
#define __H_VGCSCENE_H__
#include "Global.h"
#include "Dialog.h"
#include "InputDev.h"
#include "Toast.h"
#include "JniAndroid.h"
#include "cocos/base/CCController.h"
#include "cocos/base/CCEventListenerController.h"

USING_NS_CC;

class VgcScene : public Dialog, Toast, JniAndroid
{
public:
	VgcScene();
	~VgcScene();
	static Scene *createScene();
	virtual bool init();
	virtual void onEnter();
	CREATE_FUNC(VgcScene);

private:
	ClientCore::dev_xinput::xinput_button getCode(Controller * controller, int keyCode);
	void sendMsg(ClientCore::XINPUT_GAMEPAD xinput_gamepad);
	void keyboardKeyPressedCallback(EventKeyboard::KeyCode code, Event *event);
	void receiveODSocket();

private:
	ClientCore::XINPUT_GAMEPAD _gamepadstate;
	const  ClientCore::dev_xinput::xinput_button ERROR_KEY_CODE = ClientCore::dev_xinput::xinput_button::XINPUT_GAMEPAD_ERROR;
	EventListenerController *m_controller;

	//anim
	ui::Button *mCloseButton;
	Sprite *mCloseSprite;
	bool mIsPhoneVibrate;
	bool mIsGameVibrate;
	ui::Text *mTextPosition;
	Layer *mLayerSetup;
	XMLParser xmlParser;


};

#endif //__H_VGCSCENE_H__