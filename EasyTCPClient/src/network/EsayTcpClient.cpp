#include "EsayTcpClient.h"
#include <iostream>

#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")
#endif
EasyTcpClient::EasyTcpClient()
{
    m_sock = INVALID_SOCKET;
}

EasyTcpClient::~EasyTcpClient()
{
}

int EasyTcpClient::InitSocket()
{
    // 启动win sock 2.x环境
#ifdef _WIN32
    WORD ver = MAKEWORD(2, 2);
    WSADATA dat;
    WSAStartup(ver, &dat);
#endif
    m_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (m_sock == INVALID_SOCKET)
    {
        std::cout << "create socket faile" << std::endl;
        return -1;
    }
    std::cout << "create socket successful" << std::endl;
    return 0;
}

int EasyTcpClient::Conncet(const std::string& ipAddr, const int port)
{
    if (ipAddr.empty() || port <= 0)
    {
        std::cout << " ip or port error " << std::endl;
        return -1;
    }
    if (m_sock == INVALID_SOCKET)
    {
        if (InitSocket() < 0)
        {
            return -1;
        }
    }
    sockaddr_in _sever = {};
    _sever.sin_family = AF_INET;
    _sever.sin_port = htons(port);
#ifdef _WIN32
    _sever.sin_addr.S_un.S_addr = inet_addr(ipAddr.c_str());
#else
    _sever.sin_addr.s_addr = inet_addr(ipAddr.c_str());
#endif
    if (SOCKET_ERROR == connect(m_sock, (sockaddr*)&_sever, sizeof(sockaddr_in)))
    {
        std::cout << " connect sever failed " << std::endl;
        Close();
        return -1;
    }
    std::cout << " connect sever successful " << std::endl;
    return 0;
}

void EasyTcpClient::Close()
{
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

bool EasyTcpClient::SendMsg(const std::string& msg)
{
   // int ret = send(m_sock, msg.c_str(), msg.length(),0);
    int ret = send(m_sock, msg.c_str(), 10240, 0);
    return ret<=0?false:true;
}

bool EasyTcpClient::RecvMsg(std::string& msg)
{
    char buf[10240] = "";
    int recvLen = recv(m_sock, buf, sizeof(buf), 0);
    if (recvLen < 0)
    {
        std::cout << "sock = " << m_sock << " disconnect" << std::endl;
        return false;
    }
    msg.clear();
    msg.append(buf, recvLen);
    return true;
}

int EasyTcpClient::ProcessMsg(std::string& msg)
{
    if (msg.empty())
        return -1;
    std::cout << msg << std::endl;
    return 0;
}

SocketStatus EasyTcpClient::OnRun()
{
    if (IsRun())
    {
        fd_set fdReads;
        FD_ZERO(&fdReads);
        FD_SET(m_sock, &fdReads);
        timeval delayTime = { 0,0 };
        int ret = select(m_sock + 1, &fdReads, nullptr, nullptr, &delayTime);
        if (ret < 0)
        {
            std::cout << "sock=" << m_sock << " select exit" << std::endl;
            return SocketStatus::INVALID;
        }
        else if(ret>0) 
        {
            if (FD_ISSET(m_sock, &fdReads))
            {
                FD_CLR(m_sock, &fdReads);
                // 读取并处理接收到的数据
                return READYREADY;
            }
        }

    }
    return SocketStatus::INVALID;
}

bool EasyTcpClient::IsRun()
{
    return m_sock!=INVALID_SOCKET;
}
