#pragma once
#include "Global.h"
#include "Toast.h"
class GuideScene : public Layer, Toast
{
public:
	GuideScene();
	~GuideScene();
	CREATE_FUNC(GuideScene);
	virtual bool init();
	static Scene *createScene();
public:
	long long exitTime;
};