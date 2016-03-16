#pragma once
// ������Ҫ ������������͹�������Ϣ

#include "Global.h"
//#include "BaseResponseMsg.h"
//typedef void (cocos2d::CCObject::*ResPonseThreadEvent)(BaseResponseMsg*);
#define callFunc_selectormsg(_SELECTOR) (ResPonseThreadEvent)(&_SELECTOR)

#define M_ADDCALLBACKEVENT(varName)\
protected: cocos2d::CCObject* m_##varName##listener;ResPonseThreadEvent varName##selector;\
public: void add##varName##ListenerEvent(ResPonseThreadEvent m_event,cocos2d::CCObject* listener)  { m_##varName##listener=listener;varName##selector=m_event; }

class ResPonseThread
{
public:	
	~ResPonseThread(void);
	static ResPonseThread*   GetInstance(); // ��ȡ����ĵ���
	int start (void * =NULL); //�������߳����������������������������ָ�롣
	void stop();     //������ֹ��ǰ�̡߳�
	void sleep (int tesec); //�����õ�ǰ�߳����߸���ʱ�䣬��λΪ�����롣
	void detach();   //
	void * wait();
	bool isExit();
	void setExit(bool isExit);
    	
private:
	ResPonseThread(void);
	pthread_t handle; 
	bool started;
	bool detached;
	static void * threadFunc(void *);
	static ResPonseThread* m_pInstance;	
	bool mIsExit;
	//M_ADDCALLBACKEVENT(msg);// ����ص�����
	//M_ADDCALLBACKEVENT(notcon);//�����ص�����
	
};
