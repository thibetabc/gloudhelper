#pragma once
#include "Global.h"
class Dialog : public Layer
{
public:
	Dialog();
	~Dialog();
	CREATE_FUNC(Dialog);
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	void showDialog(std::string title, std::string msg, std::string buttonName, float rotation = 0.0f);
	void setCallback(std::function<void(Ref *ref)> funcb);
public:
	std::function<void(Ref * node)> m_callbackFun = nullptr;
	bool isShowDialog;
};