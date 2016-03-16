#pragma once
#include "Global.h"
#include "Toast.h"
#include "JniAndroid.h"

class SetupScene : public Layer, Toast, JniAndroid
{
public:
	SetupScene();
	~SetupScene();
	virtual bool init();
	CREATE_FUNC(SetupScene);
	static Scene *createScene();
protected:
private:
	void buttonPositionClickEvent(Ref *ref);
	void requestJoystickStatus();
	void removeLayerLoading();
private:
	XMLParser xmlParser;
	Node *mLayoutPositionItem;
	ui::Text *mTextPositionVersionTips;//仅支持1.3.5以上版本
	Layer *mLayerLoading;


};
