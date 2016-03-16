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

	WSADATA wsd; //WSADATA变量  
	SOCKET sHost; //服务器套接字  
	SOCKADDR_IN servAddr; //服务器地址  
	char buf[BUF_SIZE]; //接收数据缓冲区  
	char bufRecv[BUF_SIZE];
	int retVal; //返回值  
	//初始化套结字动态库  
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		cout << "WSAStartup failed!" << endl;
		return -1;
	}
	//创建套接字  
	sHost = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sHost)
	{
		cout << "socket failed!" << endl;
		WSACleanup();//释放套接字资源  
		return  -1;
	}

	//设置服务器地址  
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(ip);
	servAddr.sin_port = htons((short)port);
	int nServAddlen = sizeof(servAddr);

	//连接服务器  
	retVal = connect(sHost, (LPSOCKADDR)&servAddr, sizeof(servAddr));
	if (SOCKET_ERROR == retVal)
	{
		cout << "connect failed!" << endl;
		closesocket(sHost); //关闭套接字  
		WSACleanup(); //释放套接字资源  
		return -1;
	}
	while (true)
	{
		//向服务器发送数据  
		ZeroMemory(buf, BUF_SIZE);
		cout << " 向服务器发送数据:  ";
		cin >> buf;
		retVal = send(sHost, buf, strlen(buf), 0);
		if (SOCKET_ERROR == retVal)
		{
			cout << "send failed!" << endl;
			closesocket(sHost); //关闭套接字  
			WSACleanup(); //释放套接字资源  
			return -1;
		}
		//RecvLine(sHost, bufRecv);  
		ZeroMemory(bufRecv, BUF_SIZE);
		recv(sHost, bufRecv, BUF_SIZE, 0); // 接收服务器端的数据， 只接收5个字符  
		cout << endl << "从服务器接收数据：" << bufRecv;
		cout << "\n";
	}
	//退出  
	closesocket(sHost); //关闭套接字  
	WSACleanup(); //释放套接字资源
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
