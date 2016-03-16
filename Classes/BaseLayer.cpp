#include "BaseLayer.h"

BaseLayer::BaseLayer()
{

	mBaseLayout = nullptr;
	mTipText = nullptr;
}

BaseLayer::~BaseLayer()
{

}

bool BaseLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto layer = CSLoader::createNode("helper_base_layer.csb");
	auto layoutBg = layer->getChildByName("layout_bg");
	mBaseLayout = (ui::Layout*)layoutBg->getChildByName("base_layout");
	mTipText = (ui::Text*)layoutBg->getChildByName("tip_layout")->getChildByName("tip_text");
	addChild(layer);
	return true;
}
void BaseLayer::onExit()
{
	Layer::onExit();
}