#include <stdio.h>
#include "ODSocket.h"

#ifdef WIN32
#pragma comment(lib, "wsock32")
#endif

ODSocket::ODSocket(SOCKET sock) {
	m_sock = sock;
}

ODSocket::~ODSocket() {
}

int ODSocket::Init() {
#ifdef WIN32
	/*
	 http://msdn.microsoft.com/zh-cn/vstudio/ms741563(en-us,VS.85).aspx

	 typedef struct WSAData {
	 WORD wVersion;								//winsock version
	 WORD wHighVersion;							//The highest version of the Windows Sockets specification that the Ws2_32.dll can support
	 char szDescription[WSADESCRIPTION_LEN+1];
	 char szSystemStatus[WSASYSSTATUS_LEN+1];
	 unsigned short iMaxSockets;
	 unsigned short iMaxUdpDg;
	 char FAR * lpVendorInfo;
	 }WSADATA, *LPWSADATA;
	 */
	WSADATA wsaData;
	//#define MAKEWORD(a,b) ((WORD) (((BYTE) (a)) | ((WORD) ((BYTE) (b))) << 8))
	WORD version = MAKEWORD(2, 0);
	int ret = WSAStartup(version, &wsaData); //win sock start up
	if (ret) {
//		cerr << "Initilize winsock error !" << endl;
		return -1;
	}
#endif

	return 0;
}
//this is just for windows
int ODSocket::Clean() {
#ifdef WIN32
	return (WSACleanup());
#endif
	return 0;
}

ODSocket& ODSocket::operator =(SOCKET s) {
	m_sock = s;
	return (*this);
}

ODSocket::operator SOCKET() {
	return m_sock;
}
//create a socket object win/lin is the same
// af:
bool ODSocket::Create(int af, int type, int protocol) {
	m_sock = socket(af, type, protocol);
	if (m_sock == INVALID_SOCKET) {
		return false;
	}
//#ifdef WIN32
//	DWORD nMode = 1;
//	int nRes = ioctlsocket(m_sock, FIONBIO, &nMode);
//	if (nRes == SOCKET_ERROR) {
//		Close();
//		return false;
//	}
//#else
//	// ����Ϊ��������ʽ
//	fcntl(m_sockClient, F_SETFL, O_NONBLOCK);
//#endif
	return true;
}

int ODSocket::Connect(const char* ip, unsigned short port) {
	struct sockaddr_in svraddr;
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = inet_addr(ip);
	svraddr.sin_port = htons(port);
	return connect(m_sock, (struct sockaddr*) &svraddr, sizeof(svraddr));
}

bool ODSocket::Bind(unsigned short port) {
	struct sockaddr_in svraddr;
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = INADDR_ANY;
	svraddr.sin_port = htons(port);

	int opt = 1; 
	if (setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (char*) &opt, sizeof(opt))
			< 0)
		return false;

	int ret = bind(m_sock, (struct sockaddr*) &svraddr, sizeof(svraddr));
	if (ret == SOCKET_ERROR) {
		return false;
	}
	return true;
}
//for server
bool ODSocket::Listen(int backlog) {
	int ret = listen(m_sock, backlog);
	if (ret == SOCKET_ERROR) {
		return false;
	}
	return true;
}

bool ODSocket::Accept(ODSocket& s, char* fromip) {
	struct sockaddr_in cliaddr;
	socklen_t addrlen = sizeof(cliaddr);
	SOCKET sock = accept(m_sock, (struct sockaddr*) &cliaddr, &addrlen);
	if (sock == SOCKET_ERROR) {
		return false;
	}

	s = sock;
	if (fromip != NULL)
		sprintf(fromip, "%s", inet_ntoa(cliaddr.sin_addr));

	return true;
}

int ODSocket::Select(){
	timeval timeout;
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	fd_set writeset, exceptset;
	FD_ZERO(&writeset);
	FD_ZERO(&exceptset);
	FD_SET(m_sock, &writeset);
	FD_SET(m_sock, &exceptset);

	int ret = select(FD_SETSIZE, NULL, &writeset, &exceptset, &timeout);
	if (ret == 0 || ret < 0) {
		Close();
		return -4;
	}
	else	// ret > 0
	{
		ret = FD_ISSET(m_sock, &exceptset);
		if (ret)		// or (!FD_ISSET(m_sockClient, &writeset)
		{
			Close();
			return -3;
		}
	}
	return -2;
}



int ODSocket::Send(const char* buf, int len, int flags) {
	int bytes = 0;
	int count = 0;
	int timeout = 2000; //2s
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	setsockopt(m_sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));
#else
	struct timeval tv;
	tv.tv_sec = timeout / 1000;
	tv.tv_usec = timeout % 1000;
	setsockopt(m_sock, SOL_SOCKET, SO_SNDTIMEO, (const char*)&tv, sizeof(tv));
#endif
	while (count < len) {
		const char* a= buf + count;
		bytes = send(m_sock, buf + count, len - count, flags);
		if (bytes == -1 || bytes == 0)
			return -1;
		count += bytes;
	}
	return count;
}

int ODSocket::SendToJava(const char* buf, int len, int flags) {
	char tHeader[2] = { 0 };
	std::string sendMsg = buf;
	int tLen = sendMsg.length();
	tHeader[0] = (char)(tLen >> 8);
	tHeader[1] = (char)tLen;
	int sendResult = Send(tHeader, 2, 0);
	if (sendResult == -1)
	{
		return sendResult;
	}
	int bytes = 0;
	int count = 0;
	int timeout = 2000; //2s
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	setsockopt(m_sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));
#else
	struct timeval tv;
	tv.tv_sec = timeout / 1000;
	tv.tv_usec = timeout % 1000;
	setsockopt(m_sock, SOL_SOCKET, SO_SNDTIMEO, (const char*)&tv, sizeof(tv));
#endif

	while (count < len) {
		const char* a = buf + count;
		bytes = send(m_sock, buf + count, len - count, flags);
		if (bytes == -1 || bytes == 0)
			return -1;
		count += bytes;
	}
	return count;
}

int ODSocket::Recv(char* buf, int len, int flags) {
	return (recv(m_sock, buf, len, flags));
}

int ODSocket::Close() {
#ifdef WIN32
	return (closesocket(m_sock));
#else
	return (close(m_sock));
#endif
}

int ODSocket::GetError() {
#ifdef WIN32
	return (WSAGetLastError());
#else
	return -1;
#endif
}

bool ODSocket::DnsParse(const char* domain, char* ip) {
	struct hostent* p;
	if ((p = gethostbyname(domain)) == NULL)
		return false;

	sprintf(ip, "%u.%u.%u.%u", (unsigned char) p->h_addr_list[0][0],
			(unsigned char) p->h_addr_list[0][1],
			(unsigned char) p->h_addr_list[0][2],
			(unsigned char) p->h_addr_list[0][3]);

	return true;
}
