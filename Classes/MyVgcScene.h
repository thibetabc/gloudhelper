#ifndef __H_MYVGCSCENE_H__
#define __H_MYVGCSCENE_H__
#include "Global.h"
#include "Dialog.h"
#include "InputDev.h"
#include "Toast.h"

USING_NS_CC;

class MyVgcScene : public Dialog, public Toast
{
public:
	MyVgcScene();
	~MyVgcScene();
	static Scene *createScene();
	virtual bool init();
	virtual void onEnter();
	CREATE_FUNC(MyVgcScene);

private:
	void initView();
	void touchEventListener(Ref* ref, ui::Widget::TouchEventType type);
	void onKeyUp(int keyCode);
	void onKeyDown(int keyCode);
	ClientCore::dev_xinput::xinput_button getCode(Controller * controller, int keyCode);
	void sendMsg(ClientCore::XINPUT_GAMEPAD xinput_gamepad);
	void JniVibrate(long milliseconds); //from java
	void keyboardKeyPressedCallback(EventKeyboard::KeyCode code, Event *event);
	void receiveODSocket();
	void setLockScreen(bool isLock);
	bool getLockScreen();

private:
	ClientCore::XINPUT_GAMEPAD _gamepadstate;
	const  ClientCore::dev_xinput::xinput_button ERROR_KEY_CODE = ClientCore::dev_xinput::xinput_button::XINPUT_GAMEPAD_ERROR;
	EventListenerController *m_controller;

	//anim
	ui::Button *mCloseButton;
	Sprite *mCloseSprite;
	ui::VirtualGameController *mVgc;
	bool isLockScreen;
private:
	enum TYPE
	{
		JOYSTICK_X,
		JOYSTICK_Y,
	};
	enum{
		UNKNOWN,

		BUTTON_LOCK,
		BUTTON_CLOSE,
		BUTTON_BACK,
		BUTTON_START,
		BUTTON_LB,
		BUTTON_RB,
		BUTTON_LT,
		BUTTON_RT,
		BUTTON_X,
		BUTTON_Y,
		BUTTON_A,
		BUTTON_B,
		BUTTON_GAMEPAD_UP,
		BUTTON_GAMEPAD_DOWN,
		BUTTON_GAMEPAD_LEFT,
		BUTTON_GAMEPAD_RIGHT,
		BUTTON_GAMEPAD_LEFT_UP,
		BUTTON_GAMEPAD_LEFT_DOWN,
		BUTTON_GAMEPAD_RIGHT_UP,
		BUTTON_GAMEPAD_RIGHT_DOWN,
		BUTTON_JOYSTICK_LEFT,
		BUTTON_JOYSTICK_RIGHT,

		LTHUMB,
		RTHUMB,
		// axis
		LX,
		LY,
		RX,
		RY,
	};

};

#endif //__H_MYVGCSCENE_H__