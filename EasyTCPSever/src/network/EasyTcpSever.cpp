#include "EasyTcpSever.h"
#include <iostream>
#include <algorithm>
EasyTcpSever::EasyTcpSever()
{
	m_sock = INVALID_SOCKET;
}

EasyTcpSever::~EasyTcpSever()
{
	Close();
}

void EasyTcpSever::InitSocket()
{
#ifdef _WIN32
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);
#endif
	// 1.建立一个socket
	m_sock = socket(AF_INET, SOCK_STREAM, 0);
}

int EasyTcpSever::Bind(const std::string& ip, const unsigned short port)
{
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(port);
	if (ip.empty())
	{
#ifdef _WIN32
		_sin.sin_addr.S_un.S_addr = INADDR_ANY;//INADDR_ANY
#else
		_sin.sin_addr.s_addr = INADDR_ANY;
#endif
	}
	else
	{
#ifdef _WIN32
		_sin.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
#else
		_sin.sin_addr.s_addr = inet_addr(ip.c_str());
#endif
	}

	if (SOCKET_ERROR == bind(m_sock, (sockaddr*)&_sin, sizeof(sockaddr_in)))
	{
		std::cout << "bind port faile" << std::endl;
		return -1;
	}
	else
	{
		std::cout << "bind port successful" << std::endl;
	}
	return 0;
}

int EasyTcpSever::Listen(int num)
{
	if (SOCKET_ERROR == listen(m_sock, num))
	{
		std::cout << "listen port faile" << std::endl;
		return -1;
	}
	else
	{
		std::cout << "listen port successful" << std::endl;
	}
	return 0;
}
#if 1
std::list<ClientStatus> EasyTcpSever::Accept()
{

	fd_set fdRead;
	fd_set fdWrite;
	fd_set fdExp;

	FD_ZERO(&fdRead);
	FD_ZERO(&fdWrite);
	FD_ZERO(&fdExp);

	FD_SET(m_sock, &fdRead);
	FD_SET(m_sock, &fdWrite);
	FD_SET(m_sock, &fdExp);

	for (auto& iter : m_lClientSock)
	{
		FD_SET(iter, &fdRead);
	}
	auto maxSock = m_sock;
	if (!m_lClientSock.empty())
	{
		auto lsock = std::max_element(std::begin(m_lClientSock), std::end(m_lClientSock));
		maxSock = *lsock > m_sock ? *lsock : m_sock;
	}
	timeval delay = { 0,0 };
	//int ret = select(maxSock + 1, &fdRead, &fdWrite, &fdExp,&delay);
	int ret = select(maxSock + 1, &fdRead, &fdWrite, &fdExp, 0);
	if (ret < 0)
	{
		std::cout << " select error！！！" << std::endl;
		Close();
		return std::list<ClientStatus>();
	}
	else if (ret > 0)
	{
		// 存在可以操作的文件描述符
		// 描述符为服务端socket
		if (FD_ISSET(m_sock, &fdRead))
		{
			FD_CLR(m_sock, &fdRead);
			sockaddr_in clientAddr;
			int len = sizeof(sockaddr_in);
#ifdef _WIN32
			SOCKET client = accept(m_sock, (sockaddr*)&clientAddr, &len);
#else
			SOCKET client = accept(m_sock, (sockaddr*)&clientAddr, (socklen_t*)&len);
#endif
			if (client == INVALID_SOCKET)
			{
				std::cout << "recv a unable client" << std::endl;
			}
			else
			{
				std::cout << "recv from ip:" << inet_ntoa(clientAddr.sin_addr) << " connect " << std::endl;
				m_lClientSock.push_back(client);
			}
		}
		// 遍历已经连接的客户端的文件描述符
		std::list<ClientStatus> clientList;
		for (auto iter = m_lClientSock.begin(); iter != m_lClientSock.end(); iter++)
		{
			if (FD_ISSET(*iter, &fdRead))
			{
				FD_CLR(*iter, &fdRead);
				clientList.push_back(ClientStatus(*iter, SocketStatus::READYREADY));
			}
		}
		return clientList;
	}
	else if (ret == 0)
	{
		// 没有可以操作的描述符，干其他事情
	}

	return std::list<ClientStatus>();
}
#endif
void EasyTcpSever::Close()
{
	//关闭连接的客户端
	for (auto iter : m_lClientSock)
	{
		if (iter != INVALID_SOCKET)
		{
#ifdef _WIN32
			closesocket(iter);
#else
			close(iter);
#endif
		}
	}
	m_lClientSock.clear();
	// 关闭win sock 2.x 环境
#ifdef _WIN32
	if (m_sock != INVALID_SOCKET)
	{
		closesocket(m_sock);
	}
	WSACleanup();
#else
	if (m_sock != INVALID_SOCKET)
	{
		close(m_sock);
	}
#endif
}

bool EasyTcpSever::IsRun()
{
	return m_sock != INVALID_SOCKET;
}

bool EasyTcpSever::OnRun()
{
	if (!IsRun())
	{
		Close();
		return false;
	}

	return false;
}

int EasyTcpSever::RecvData(SOCKET sock, std::string& msg)
{
	msg.clear();
	// 缓冲区
	char buf[10240] = "";
	int recvLen = (int)recv(sock, buf, sizeof(buf), 0);
	if (recvLen <= 0)
	{
		std::cout << "sock = " << sock << "disconnect" << std::endl;
		auto iter = std::find(m_lClientSock.begin(), m_lClientSock.end(), sock);
		if (iter != m_lClientSock.end())
		{
			m_lClientSock.erase(iter);
		}
		return -1;
	}
	msg.append(buf, recvLen);
	return recvLen;
}

int EasyTcpSever::SendData(SOCKET sock, std::string& msg)
{
	if (IsRun() && !msg.empty())
	{
		return send(sock, msg.c_str(), msg.length(), 0);
	}
	return SOCKET_ERROR;
}

void EasyTcpSever::SendDataToAll(std::string& msg)
{
	for (auto& iter : m_lClientSock)
	{
		SendData(iter, msg);
	}
}
