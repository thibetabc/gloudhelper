#include "LoadingLayer.h"

using namespace ui;
LoadingLayer::LoadingLayer()
{

}
LoadingLayer::~LoadingLayer()
{

}
bool LoadingLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//固定的底部loading
	for (int i = 0; i < MAX_POINT; i++)
	{
		Sprite *sp = Sprite::create("img_loading_0.png");
		sp->setPosition(Vec2(MAX_PADDING * i, 0));
		sp->setTag(i);
		addChild(sp);
	}


	//设置显示区域大小
	Size spriteSize = Size::ZERO;
	//遮罩底板内容
	mLayoutLoadingTop = Layout::create();
	//三个loading加到底板显示内容上
	for (int i = 0; i < MAX_SPRITE; i++)
	{
		Sprite *sp = Sprite::create(StringUtils::format("img_loading_%d.png", i + 1));
		sp->setPositionX(MAX_PADDING * i);
		sp->setTag(i);
		mLayoutLoadingTop->addChild(sp);
		if (spriteSize.equals(Size::ZERO))
		{
			spriteSize = sp->getContentSize();
		}
	}

	//底板宽
	auto sp = mLayoutLoadingTop->getChildByTag(MAX_SPRITE - 1);
	mLayoutLoadingTop->setContentSize(Size(sp->getPositionX(), spriteSize.height));
	mLayoutLoadingTopPos = mLayoutLoadingTop->getPosition();

	//开启定时器：滚动底板内容
	schedule([&](float dt){
		//滚动底板
		mLayoutLoadingTop->setPositionX(mLayoutLoadingTop->getPositionX() + MAX_PADDING);
		if (mLayoutLoadingTop->getPositionX() > mLayoutLoadingTopPos.x + mStencil->getContentSize().width)
		{
			mLayoutLoadingTop->setPositionX(mLayoutLoadingTopPos.x - mLayoutLoadingTop->getContentSize().width);
		}
	}, MOVE_TIMER, "schedule_loading_move");

	//模板
	mStencil = Sprite::create();
	mStencil->setTextureRect(Rect(0, 0, getChildByTag(MAX_POINT - 1)->getPositionX() + spriteSize.width, spriteSize.height));
	mStencil->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	mStencil->setPositionX(-spriteSize.width/2);

	//1.创建遮罩 
	//2.设置模板
	mClippingNode = ClippingNode::create(mStencil);

	//3.显示模板内的内容
	mClippingNode->addChild(mLayoutLoadingTop);

	//4.设置模板内的属性
	mClippingNode->setInverted(false);
	mClippingNode->setAlphaThreshold(1);

	//5.添加遮罩节点
	addChild(mClippingNode);

	Rect rect = mStencil->getTextureRect();
	this->setContentSize(Size(rect.size.width,rect.size.height));
	return true;
}

void LoadingLayer::onExit()
{
	Layer::onExit();
}