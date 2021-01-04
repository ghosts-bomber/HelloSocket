#ifndef ESAYTCPCLIENT_HPP
#define ESAYTCPCLIENT_HPP
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define FD_SETSIZE      1024
#include <Windows.h>
#include <WinSock2.h>
#else
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#define SOCKET int
#define INVALID_SOCKET (SOCKET)(~0)
#define SOCKET_ERROR (-1)
#endif
#include <string>
enum SocketStatus {
	INVALID = 0,
	WRITEDREADY,
	READYREADY,
	WDRDREADY
};
class EasyTcpClient 
{
public:
	EasyTcpClient();
	virtual ~EasyTcpClient();
	// ��ʼ��socket
	int InitSocket();
	// ���ӷ�����
	int Conncet(const std::string& ipAddr,const int port);
	// �ر�socket
	void Close();
	// ��������
	bool SendMsg(const std::string& msg);
	// ��������
	bool RecvMsg(std::string& msg);
	// ��������
	int ProcessMsg(std::string& msg);
	SocketStatus OnRun();
	bool IsRun();
private:
	SOCKET m_sock;
};

#endif /*ESAYTCPCLIENT_HPP*/