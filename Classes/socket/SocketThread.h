#pragma once
#include "ODSocket.h"
#include "Global.h"
class SocketThread
{
public:	
	~SocketThread(void);
	static SocketThread *GetInstance();
	static void destroyInstance();
	int start(const char *ip, int port);
	ODSocket getSocket();
	int state;// 0 ��ʾ���ӳɹ� 1 ��ʾ����ʧ��
	ODSocket csocket;	
	void stop();//������ֹ��ǰ�̡߳�
private:
	pthread_t pid;	
	static void* start_thread(void *);//��̬��Ա����,�൱��C�е�ȫ�ֺ��� 	
	SocketThread(void);
private:
	static SocketThread* m_pInstance;	
	const char *m_ip;
	int m_port;
};

