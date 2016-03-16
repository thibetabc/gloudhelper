#ifndef __H_BASELAYER_H__
#define __H_BASELAYER_H__
#include "Global.h"

class BaseLayer:public Layer
{
public:
	BaseLayer();
	~BaseLayer();
	virtual bool init();
	virtual void onExit();
	CREATE_FUNC(BaseLayer);
	
private:

public:
	ui::Layout *mBaseLayout;
	ui::Text *mTipText;
	
};


#endif