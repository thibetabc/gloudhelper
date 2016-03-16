#ifndef __H_MORESCENE_H__
#define __H_MORESCENE_H__

#include "Global.h"
#include "BaseLayer.h"
#include "JniAndroid.h"

class MoreScene :public BaseLayer, JniAndroid
{
public:
	MoreScene();
	~MoreScene();
	CREATE_FUNC(MoreScene);
	virtual bool init();
	static Scene *createScene();
private:
	void download(Ref *ref);
	

};

#endif //__H_MORESCENE_H__