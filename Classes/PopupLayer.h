#pragma once
#include "Global.h"
using namespace ui;

class PopupLayer :public Layer
{
public:
	PopupLayer();
	~PopupLayer();
	CREATE_FUNC(PopupLayer);
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();

	static PopupLayer * create(const char* backgroundImage);
	void setTitle(const char *title, int fontSize = 20, cocos2d::Color3B color = cocos2d::Color3B::WHITE);
	void setContentText(const std::string text, const std::string& systemFont = "", float fontSize = 20.0f, cocos2d::Color3B color = cocos2d::Color3B::WHITE, int padding = 50, int paddingTop = 150);
	void setCallbackFunc(const std::function<void(Ref* ref)> &callback);
	bool addButton(const char* normalImage, const char* selectedImage, const char* title, int tag = 0);
	

private:
	void buttonCallback(Ref* pSender);
	CC_SYNTHESIZE_RETAIN(Scale9Sprite*, m_s9BackGround, Sprite9BackGround);
	CC_SYNTHESIZE_RETAIN(Text*, m_title, TextTitle);
	CC_SYNTHESIZE_RETAIN(Text*, m_contentText, ContentText);
	void backgroundFinish();

private:
	int m_contentPadding;
	int m_contentPaddingTop;
	std::function<void(Ref* ref)> m_callback;
	Vector<Button *> m_vectorButton;
};