#include "EasyTcpSever.h"
#include <string>
#include <iostream>
#include <thread>
#include <list>
int main()
{
	EasyTcpSever sever01;
	sever01.InitSocket();
	sever01.Bind(std::string(), 4567);
	sever01.Listen(10);
	std::thread thread01([&]() {
		while (true)
		{
			std::list<ClientStatus> clientList = sever01.Accept();
			for (auto iter : clientList)
			{
				if (iter.statue == SocketStatus::READYREADY)
				{
					std::string msg;
					sever01.RecvData(iter.sock,msg);
					//std::cout << "接收到来自sock="<<iter.sock<<"的消息为：" << msg << std::endl;
				}
			}
		}
		sever01.Close();
	});
	thread01.detach();
	while (true)
	{

	}
	return 0;
}
