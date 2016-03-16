#include "PopupLayer.h"

PopupLayer::PopupLayer() :
 m_contentPadding(0)
, m_contentPaddingTop(0)
, m_callback(nullptr)
, m_s9BackGround(NULL)
, m_contentText(NULL)
, m_title(NULL)
{
}
PopupLayer::~PopupLayer()
{
	CC_SAFE_RELEASE(m_contentText);
	CC_SAFE_RELEASE(m_title);
	CC_SAFE_RELEASE(m_s9BackGround);
}
bool PopupLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	//°ëÍ¸Ã÷²ã
	Size visibleSize = Director::getInstance()->getVisibleSize();
	LayerColor *layerColor = LayerColor::create(Color4B(0, 0, 0, 100), visibleSize.width, visibleSize.height);
	this->addChild(layerColor);

	////ÆÁ±ÎÏÂ²ãµã»÷ÊÂ¼þ
	EventListenerTouchOneByOne *listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [this](Touch *touch, Event *event)
	{		
		return true;
	};
	listener->onTouchEnded = [this](Touch *touch, Event *event)
	{
		//±³¾°¿òÍâÇøÓòÉ¾³ýµ¯³ö¿ò
	/*	Scale9Sprite *s9pbackGround = this->getSprite9BackGround();
		if (!s9pbackGround->getBoundingBox().containsPoint(touch->getLocation()))
		{
			removeFromParent();
		}*/
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}
PopupLayer* PopupLayer::create(const char *backgroundImage)
{
	PopupLayer* ml = PopupLayer::create();
	ml->setSprite9BackGround(Scale9Sprite::create(backgroundImage));
	return ml;
}

void PopupLayer::setTitle(const char *title, int fontSize/* = 20*/, cocos2d::Color3B color/* = cocos2d::Color3B::WHITE*/)
{
	Text* ltfTitle = Text::create(title, "", fontSize);
	setTextTitle(ltfTitle);
	ltfTitle->setColor(color);
}
void PopupLayer::setContentText(const std::string text, const std::string& systemFont/* = ""*/, float fontSize/* = 20.0f*/, Color3B color /*= cocos2d::Color3B::WHITE*/, int padding/* = 50*/, int paddingTop/* = 150*/)
{
	Text* ltf = Text::create(text, systemFont, fontSize);
	setContentText(ltf);
	ltf->setColor(color);
	m_contentPadding = padding;
	m_contentPaddingTop = paddingTop;
}

void PopupLayer::setCallbackFunc(const std::function<void(Ref* ref)> &callback)
{
	m_callback = callback;
}
bool PopupLayer::addButton(const char *normalImage, const char *selectedImage, const char *title, int tag)
{
	ui::Button *button = ui::Button::create(normalImage, selectedImage, selectedImage);
	button->setTitleText(title);
	button->setTitleFontSize(button->getContentSize().height / 2);
	button->setTag((int)m_vectorButton.size());
	button->addTouchEventListener([&](Ref *ref, ui::Widget::TouchEventType type){
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			buttonCallback(ref);
			removeFromParent();
		}
	});
	m_vectorButton.pushBack(button);
	return true;
}
void PopupLayer::buttonCallback(Ref *ref)
{
	if (m_callback)
	{
		m_callback(ref);
	}
}
void PopupLayer::onEnter()
{
	Layer::onEnter();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 center = Vec2(visibleSize.width / 2, visibleSize.height / 2);
	Size m_backGroundSize = getContentSize();
	//±³¾°
	ui::Scale9Sprite *s9pbackGround = this->getSprite9BackGround();
	s9pbackGround->setContentSize(m_backGroundSize);
	s9pbackGround->setPosition(center);
	this->addChild(s9pbackGround);	
	//µ¯³öÐ§¹û
	s9pbackGround->runAction(
		Sequence::create(
		ScaleTo::create(0.0f, 0.0f),
		ScaleTo::create(0.06f, 1.05f),
		ScaleTo::create(0.08f, 0.95f),
		ScaleTo::create(0.08f, 1.0f),
		CallFunc::create(CC_CALLBACK_0(PopupLayer::backgroundFinish, this)),
		NULL));

}
void PopupLayer::onExit()
{
	CCLOG("popup on exit.");
	Layer::onExit();
}

void PopupLayer::backgroundFinish()
{

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 center = Vec2(visibleSize.width / 2, visibleSize.height / 2);
	Size m_backGroundSize = getContentSize();
	//±³¾°
	ui::Scale9Sprite *s9pbackGround = this->getSprite9BackGround();
	Text *lbTitle = this->getTextTitle();
	if (lbTitle)
	{
		lbTitle->setPosition(Vec2(m_backGroundSize.width / 2, m_backGroundSize.height - lbTitle->getContentSize().height));
		s9pbackGround->addChild(lbTitle);

	}
	//ÄÚÈÝ
	Text *lbContentText = this->getContentText();
	if (lbContentText)
	{
		lbContentText->setPosition(Vec2(m_backGroundSize.width / 2, m_backGroundSize.height / 2));
		lbContentText->ignoreContentAdaptWithSize(false);
		lbContentText->setTextAreaSize(Size(m_backGroundSize.width - m_contentPadding * 2, m_backGroundSize.height - m_contentPaddingTop));
		lbContentText->setTextHorizontalAlignment(TextHAlignment::CENTER);
		s9pbackGround->addChild(lbContentText);
	}

	//°´Å¥
	auto m_keyMax = m_vectorButton.size();
	if (m_keyMax > 0)
	{
		float buttonWidth = m_backGroundSize.width / (m_keyMax + 1);
		int index = 0;
		for (auto it : m_vectorButton)
		{
			it->setPosition(Vec2(buttonWidth *(++index), it->getContentSize().height / 2 + 20));
			s9pbackGround->addChild(it);
		}
	}
}