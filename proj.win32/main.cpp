#include "main.h"
#include "AppDelegate.h"
#include "cocos2d.h"
#include <stdio.h>
#include <Winsock2.h>
#include <iostream> 

USING_NS_CC;
#pragma comment(lib,"ws2_32.lib")

using namespace std;

static void gameSocket(const char *ip, int port)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(1, 1);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		return;
	}

	if (LOBYTE(wsaData.wVersion) != 1 ||
		HIBYTE(wsaData.wVersion) != 1) {
		WSACleanup();
		return;
	}
	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(ip);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(port);
	int connectResult = connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	if (connectResult == SOCKET_ERROR)
	{
		log("connectResult = %d", connectResult);
	}
	//std::string str = "{ "Type":"Joystick" }";
	int sendResult = send(sockClient, "hello", strlen("hello") + 1, 0);
	if (sendResult == SOCKET_ERROR)
	{
		log("sendResult = %d", sendResult);
	}
	char recvBuf[1024];
	memset(recvBuf, 0, sizeof(recvBuf));
	int recvResult = recv(sockClient, recvBuf, sizeof(recvBuf), 0);
	if (recvResult == SOCKET_ERROR)
	{
		log("recvResult = %d", recvResult);
	}
	std::string result = recvBuf;
	printf("%s\n", recvBuf);

	closesocket(sockClient);
	WSACleanup();
}

int myScoket(const char *ip, int port)
{
	const int BUF_SIZE = 1024;

	WSADATA wsd; //WSADATA����  
	SOCKET sHost; //�������׽���  
	SOCKADDR_IN servAddr; //��������ַ  
	char buf[BUF_SIZE]; //�������ݻ�����  
	char bufRecv[BUF_SIZE];
	int retVal; //����ֵ  
	//��ʼ���׽��ֶ�̬��  
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		cout << "WSAStartup failed!" << endl;
		return -1;
	}
	//�����׽���  
	sHost = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sHost)
	{
		cout << "socket failed!" << endl;
		WSACleanup();//�ͷ��׽�����Դ  
		return  -1;
	}

	//���÷�������ַ  
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(ip);
	servAddr.sin_port = htons((short)port);
	int nServAddlen = sizeof(servAddr);

	//���ӷ�����  
	retVal = connect(sHost, (LPSOCKADDR)&servAddr, sizeof(servAddr));
	if (SOCKET_ERROR == retVal)
	{
		cout << "connect failed!" << endl;
		closesocket(sHost); //�ر��׽���  
		WSACleanup(); //�ͷ��׽�����Դ  
		return -1;
	}
	while (true)
	{
		//���������������  
		ZeroMemory(buf, BUF_SIZE);
		cout << " ���������������:  ";
		cin >> buf;
		retVal = send(sHost, buf, strlen(buf), 0);
		if (SOCKET_ERROR == retVal)
		{
			cout << "send failed!" << endl;
			closesocket(sHost); //�ر��׽���  
			WSACleanup(); //�ͷ��׽�����Դ  
			return -1;
		}
		//RecvLine(sHost, bufRecv);  
		ZeroMemory(bufRecv, BUF_SIZE);
		recv(sHost, bufRecv, BUF_SIZE, 0); // ���շ������˵����ݣ� ֻ����5���ַ�  
		cout << endl << "�ӷ������������ݣ�" << bufRecv;
		cout << "\n";
	}
	//�˳�  
	closesocket(sHost); //�ر��׽���  
	WSACleanup(); //�ͷ��׽�����Դ
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // create the application instance
    AppDelegate app;

	//10_0_4_251#9527_9528_9529_9530
	//gameSocket("10.0.4.251", 9527);
	//myScoket("10.0.4.251", 9527);
    return Application::getInstance()->run();
}
