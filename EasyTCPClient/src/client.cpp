﻿#include "EsayTcpClient.h"
#include <string>
#include <thread>
int main()
{
	for(int i=0;i<1;i++)
	{ 
	std::thread thread01([]() {
		EasyTcpClient client01;
		client01.InitSocket();
		//client01.Conncet(std::string("192.168.43.198"), 4567);
		client01.Conncet(std::string("127.0.0.1"), 4567);
		while (true)
		{
			std::string msg(10240,'a');
			client01.SendMsg(msg);
			/*
			if(client01.OnRun()==SocketStatus::READYREADY)
			{
				std::string msg;
				client01.RecvMsg(msg);
				client01.ProcessMsg(msg);
			}
			*/
		}
		client01.Close();
	});
		thread01.detach();
	}
	while (true)
	{

	}
	return 0;
}
