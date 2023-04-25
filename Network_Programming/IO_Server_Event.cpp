#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<stdio.h>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<stdlib.h>
#include <time.h>
//连接到WinSock2对应的文件:Ws2_32.lib, Mswsock.lib, Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")// IO_Server_Non_Block.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

#define DEFAULT_BUFLEN 128
#define DEFAULT_PORT "27015"
#define DEFAULT_PORT_INT 27015
//初始化Winsock()
void InitSock()
{
	WSADATA wsaData;
	long iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed with error: %d\n", iResult);
		exit(0);
	}
}
//创建监听套接字，返回套接字
SOCKET CreateTCPSocket()
{
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET)
	{
		printf("socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		exit(0);
	}
	return s;
}
////获得IP地址和端口号
//addrinfo* My_Getaddr(SOCKET s)
//{
//  struct addrinfo* result = NULL;
//	struct addrinfo hints;
//	ZeroMemory(&hints, sizeof(hints));
//	hints.ai_family = AF_INET;
//	hints.ai_socktype = SOCK_STREAM;
//	hints.ai_protocol = IPPROTO_TCP;
//	hints.ai_flags = AI_PASSIVE;
//	int iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
//	if (iResult != 0)
//	{
//		printf("getaddrinfo failed with error: %d\n", iResult);
//		//freeaddrinfo(&hints);
//		//freeaddrinfo(result);
//		closesocket(s);
// 		WSACleanup();
//		exit(0);
//	}
//	return result;
//}
////绑定端口号和IP地址
//void My_Bind(SOCKET s, addrinfo* result)
//{
//	int r = bind(s, result->ai_addr, (int)result->ai_addrlen);
//	if (r == SOCKET_ERROR)
//	{
//		printf("bind failed with error: %d\n", WSAGetLastError());
//		freeaddrinfo(result);
//		closesocket(s);
//		WSACleanup();
//		exit(0);
//	}
//}
//绑定端口号和IP地址
void My_Bind(SOCKET s, char* Ip, int port)
{
	struct sockaddr_in hints;
	hints.sin_family = AF_INET;
	hints.sin_port = htons(port);
	hints.sin_addr.S_un.S_addr = inet_addr(Ip);
	int r = bind(s, (const sockaddr*)&hints, sizeof(hints));
	if (r == SOCKET_ERROR)
	{
		printf("bind failed with error: %d\n", WSAGetLastError());
		closesocket(s);
		WSACleanup();
		exit(0);
	}
}
//监听绑定	
void My_Listen(SOCKET s)
{
	int iResult = listen(s, SOMAXCONN);
	if (iResult == SOCKET_ERROR)
	{
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(s);
		WSACleanup();
		exit(0);
	}
	printf("服务器初始化成功!\n");
}
//接受链接产生通讯套接字
SOCKET My_Accept(SOCKET s, sockaddr_in* addr)
{
	sockaddr_in hints;
	int len = sizeof(sockaddr);
	SOCKET Client_s = accept(s, (sockaddr FAR*) & hints, &len);
	if (Client_s == INVALID_SOCKET)
	{
		printf("socket failed with error: %ld\n", WSAGetLastError());
		closesocket(s);
		WSACleanup();
		exit(0);
	}
	else
	{
		addr->sin_family = hints.sin_family;
		addr->sin_port = hints.sin_port;
		addr->sin_addr = hints.sin_addr;
	}
	return Client_s;
}

int main(int argc, char* argv[])
{
	//struct addrinfo* result = NULL;
	sockaddr_in addrs;
	sockaddr_in* addr = &addrs;
	int addrlen = sizeof(addrs);
	InitSock();
	SOCKET s = CreateTCPSocket();
	//result = My_Getaddr(s, result);
	//My_Bind(s, result);
	My_Bind(s, (char*)("127.0.0.1"), DEFAULT_PORT_INT);
	My_Listen(s);


	//创建事件对象，并关联到套接字ServerSocket上，注册FD_ACCEPT事件
	WSAEVENT Event = WSACreateEvent();
	WSAResetEvent(Event);
	int iIndex = 0, i;
	int iEventTotal = 0;
	//事件句柄和套接字句柄
	WSAEVENT eventArray[WSA_MAXIMUM_WAIT_EVENTS];
	SOCKET sockArray[WSA_MAXIMUM_WAIT_EVENTS];
	WSAEventSelect(s, Event, FD_ACCEPT);
	//将新建的事件Event保存到eventArray数组中
	eventArray[iEventTotal] = Event;
	//将套接字s保存到sockArray数组中
	sockArray[iEventTotal] = s;
	iEventTotal++;
	//处理网络事件
	char buf[DEFAULT_BUFLEN] = { 0 };
	while (true)
	{
		//在所有事件对象上等待，只要有一个事件对象变成已授信状态，则函数返回
		iIndex = WSAWaitForMultipleEvents(iEventTotal, eventArray, FALSE, WSA_INFINITE, FALSE);
		//对每个事件调用WSAWaitForMultipleEvents()函数，以便确定它的状态
		//发生的事件对象的索引，一般是句柄数组中最前面的那一个
		//然后再用循环依次处理后面的事件
		iIndex = iIndex - WSA_WAIT_EVENT_0;
		for (i = iIndex; i < iEventTotal; i++)
		{
			int iResult = WSAWaitForMultipleEvents(1, &eventArray[i], TRUE, 1000, FALSE);
			if (iResult == WSA_WAIT_FAILED || iResult == WSA_WAIT_TIMEOUT)
			{
				printf("WSA_WAIT_FAILED or WSA_WAIT_TIMEOUT!\n");
				continue;
			}
			else
			{
				//获得到来的通知消息，WSAEnumNetworkEvents函数会自动重置授信事件
				WSANETWORKEVENTS newevent;
				WSAEnumNetworkEvents(sockArray[i], eventArray[i], &newevent);
				if (newevent.lNetworkEvents & FD_ACCEPT) //处理FD_ACCEPT通知消息
				{
					if (newevent.iErrorCode[FD_ACCEPT_BIT] == 0)
					{
						//如果处理FD_ACCEPT消息时没有错误
						if (iEventTotal > WSA_MAXIMUM_WAIT_EVENTS)
						{
							//连接太多暂不处理
							printf("Too many connections! \n");
							continue;
						}
						else
						{
							//连接到请求，得到和客户通信的套接字
							//AcceptSocket
							SOCKET Client_s = My_Accept(s, addr);
							printf("连接成功  来自于:%s  \n", inet_ntoa(*(struct in_addr*)&(addrs.sin_addr.S_un.S_addr)));
							//为新套接字创建事件对象
							WSAEVENT NewEvent = WSACreateEvent();
							//将新建的事件对象NewEvent关联到套接字AcceptSocket上
							//注册FD_READ|FD_CLOSE|FD_WRITE网络事件
							WSAEventSelect(Client_s, NewEvent, FD_READ | FD_CLOSE | FD_WRITE);
							//将新事件NewEvent保存到eventArray数组中
							eventArray[iEventTotal] = NewEvent;
							//将新套接字Client_s保存到sockArray数组中
							sockArray[iEventTotal] = Client_s;
							iEventTotal++;
						}
					}
				}
				if (newevent.lNetworkEvents & FD_READ) //处理FD_READ通知消息
				{
					if (newevent.iErrorCode[FD_READ_BIT] == 0)
					{
						//处理FD_READ没有错误
						//有数据到达
						memset(buf, 0, sizeof(buf));
						int r = recv(sockArray[i], buf, sizeof(buf), 0);
						if (r > 0)
						{
							printf("[Recv]:%s \n", buf);
							r = send(sockArray[i], buf, r, 0);
							if (r == strlen(buf))
							{
								printf(">>>[Send success!]\n");
							}
							else
							{
								printf(">>>[Send bad!]\n");
							}
						}
						else
						{
							printf("[Recv]:[Error!]\n");
							closesocket(sockArray[i]);
						}
					}
				}
				if (newevent.lNetworkEvents & FD_CLOSE) //处理FD_CLOSE通知消息
				{
					if (newevent.iErrorCode[FD_CLOSE_BIT] == 0)
					{

						printf("Current Connection closing...\n");
						closesocket(sockArray[i]);
						WSACloseEvent(eventArray[i]);
						for (int j = i; j < iEventTotal - 1; j++)
						{
							sockArray[j] = sockArray[j + 1];
							eventArray[j] = eventArray[j + 1];

						}
						iEventTotal--;
					}
				}
				if (newevent.lNetworkEvents & FD_WRITE) //处理FD_WRITE通知消息
				{
					if (newevent.iErrorCode[FD_WRITE_BIT] == 0)
					{
						printf(">>>[Send new!]\n");
					}
				}
			}
		}
	}
	closesocket(s);
	WSACleanup();
	return 0;
}
