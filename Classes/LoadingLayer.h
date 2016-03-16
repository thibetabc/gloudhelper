#pragma once 
#include "Global.h"

class LoadingLayer : public Layer
{
public:
	LoadingLayer();
	~LoadingLayer();
	virtual bool init();
	virtual void onExit();
	CREATE_FUNC(LoadingLayer);
private:
	const int MAX_POINT = 5; //灰底的数量
	const int MAX_PADDING = 50; //loading点的间距
	const int MAX_SPRITE = 3; //移动loading点的个数,目前只有3张对应的图
	const float MOVE_TIMER = 0.12f;//移动速度
	//遮罩
	ui::Layout *mLayoutLoadingTop;//底板:三个移动的loading点
	Vec2 mLayoutLoadingTopPos;//底板position
	Sprite *mStencil;//模板
	ClippingNode *mClippingNode; //遮罩

};