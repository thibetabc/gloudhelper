#include "SocketThread.h"
#include "cocos2d.h"
#include "ResPonseThread.h"
USING_NS_CC;
int SocketThread::start(const char *ip, int port){
	int errCode = 0;
	m_ip = ip;
	m_port = port;
	log("SocketThread::start(%s, %d)", m_ip, port);
	do{
		pthread_attr_t tAttr;
		errCode = pthread_attr_init(&tAttr);
		CC_BREAK_IF(errCode!=0);
		//但是上面这个函数其他内容则主要为你创建的线程设定为分离式
		errCode = pthread_attr_setdetachstate(&tAttr, PTHREAD_CREATE_DETACHED);
		log("SocketThread::start code : %d", errCode);
		if (errCode!=0) {
			pthread_attr_destroy(&tAttr);
			break;
		}		
		errCode = pthread_create(&pid,&tAttr,start_thread,this);
	}while (0);
	return errCode;
} 


void* SocketThread::start_thread(void *arg)   {  
	SocketThread* thred=(SocketThread*)arg;
	ODSocket cdSocket;
	cdSocket.Init();	
	bool isok=cdSocket.Create(AF_INET,SOCK_STREAM,0);	
	int iscon = cdSocket.Connect(m_pInstance->m_ip, m_pInstance->m_port);
	int timeout = 1000; //1s
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	setsockopt(cdSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
	setsockopt(cdSocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));
#else
	struct timeval tv;
	tv.tv_sec = timeout / 1000;
	tv.tv_usec = timeout % 1000;
	setsockopt(cdSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));
	setsockopt(cdSocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv, sizeof(tv));
#endif
	thred->csocket = cdSocket;
	if (iscon == SOCKET_ERROR || iscon == INVALID_SOCKET)
	{
		thred->state = 1;	
		//show dialog:　can't connect network
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([]() {
			Director::getInstance()->getEventDispatcher()->setEnabled(true);
			Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CONNECT_DEVICE_MSG, nullptr);
		});
	}
	else
	{
		thred->state=0;
		ResPonseThread::GetInstance()->start();// 启动响应参数
		CCLOG("conection");

	}	
	return NULL;                                                                                    
}
ODSocket SocketThread::getSocket(){
	return this->csocket;
}

SocketThread* SocketThread::m_pInstance=nullptr; 
SocketThread* SocketThread::GetInstance(){	
	if (!m_pInstance)
	{	
		m_pInstance = new SocketThread;
	}
	return m_pInstance;
}
void SocketThread::destroyInstance()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

void SocketThread::stop(){
	//pthread_kill(pid, SIGUSR2);
	//http://bbs.rosoo.net/thread-10289-1-1.html
	//http://www.rosoo.net/a/201112/15523.html
	//pthread_cancel(pid);
	ResPonseThread::GetInstance()->setExit(true);
	csocket.Close();
	pthread_detach(pid); 
}

SocketThread::SocketThread(void)
{
	m_ip = nullptr;
	m_port = -1;
	state = 1;
}


SocketThread::~SocketThread(void)
{
	
}
