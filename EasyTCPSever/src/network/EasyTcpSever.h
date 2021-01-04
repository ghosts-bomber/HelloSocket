#ifndef EASYTCPSEVER_H
#define EASYTCPSEVER_H

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define FD_SETSIZE      1024
#include <Windows.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#define SOCKET int
#define INVALID_SOCKET (SOCKET)(~0)
#define SOCKET_ERROR (-1)
#endif

#include <string>
#include <list>
enum SocketStatus {
	INVALID = 0,
	WRITEDREADY,
	READYREADY,
	WDRDREADY
};
struct ClientStatus
{
	ClientStatus() {
		sock = INVALID_SOCKET;
		statue = SocketStatus::INVALID;
	}
	ClientStatus(SOCKET sock,SocketStatus statue) {
		this->sock = sock;
		this->statue = statue;
	}
	SOCKET sock;
	SocketStatus statue;     
};
class EasyTcpSever
{
public:
	EasyTcpSever();
	virtual ~EasyTcpSever();

	// 初始化socket
	void InitSocket();
	// 绑定端口
	int Bind(const std::string& ip, const unsigned short port);
	// 监听端口
	int Listen(int num);
	// 接收客户端连接
	std::list<ClientStatus> Accept();
	// 关闭socket
	void Close();
	// 处理消息
	// 是否在工作
	bool IsRun();
	bool OnRun();
	// 接收数据 处理粘包 拆分包
	int RecvData(SOCKET sock,std::string& msg);
	// 发送数据
	int SendData(SOCKET sock, std::string& msg);
	void SendDataToAll(std::string& msg);
private:
	SOCKET m_sock=INVALID_SOCKET;
	std::list<SOCKET> m_lClientSock;
};


#endif // !EASYTCPSEVER_H


