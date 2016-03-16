#include "Dialog.h"

using namespace ui;
Dialog::Dialog()
{
	m_callbackFun = nullptr;
	isShowDialog = false;
}
Dialog::~Dialog()
{
}
bool Dialog::init()
{
	if (Layer::init())
	{
		return true;
	}
	return false;
}
void Dialog::onEnter()
{
	Layer::onEnter();
}
void Dialog::onExit()
{
	Layer::onExit();
}
void Dialog::showDialog(std::string title, std::string msg, std::string buttonName, float rotation)
{
	
	//can not connect
	auto layer = CSLoader::createNode("no_wifi_layer.csb");
	auto noconnectLayout = (Layout*)layer->getChildByName("bg_layout")->getChildByName("bg_image");
	auto setupButton = (Button*)noconnectLayout->getChildByName("setup_button");
	noconnectLayout->setTouchEnabled(true);
	noconnectLayout->setSwallowTouches(true);
	noconnectLayout->addClickEventListener([=](Ref *ref)
	{
		log("nowifiLayout->addClickEventListener(...) called");
	});
	setupButton->addClickEventListener([=](Ref *ref)
	{
		if (m_callbackFun)
		{
			m_callbackFun(ref);
		}
		layer->removeFromParent();
		isShowDialog = false;
	});

	auto noconnectTitleText = (Text*)noconnectLayout->getChildByName("title_text");
	auto noconnectMsgText = (Text*)noconnectLayout->getChildByName("info_text");
	noconnectTitleText->setString(title);
	noconnectMsgText->setString(msg);
	setupButton->setTitleText(buttonName);
	addChild(layer, TOP_ZODER);
	noconnectLayout->setRotation(rotation);
	isShowDialog = true;
}

//…Ë÷√ªÿµ˜
void Dialog::setCallback(std::function<void(Ref *ref)> funcb)
{
	m_callbackFun = funcb;
}
