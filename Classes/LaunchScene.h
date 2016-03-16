#pragma once
#include "Global.h"

class LaunchScene : public Layer
{
public:
	LaunchScene();
	~LaunchScene();
	CREATE_FUNC(LaunchScene);
	virtual bool init();
	static Scene *createScene();
public:
};