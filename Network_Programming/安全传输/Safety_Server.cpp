#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<stdio.h>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<stdlib.h>
#include <time.h>
#include "KEY_PRODUCE.h"
#include "des.h"
//连接到WinSock2对应的文件:Ws2_32.lib, Mswsock.lib, Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")// IO_Server_Non_Block.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

#define DEFAULT_BUFLEN 128
#define DEFAULT_PORT "27015"
#define DEFAULT_PORT_INT 27015

#pragma pack(push,1)
typedef struct {
	unsigned char cmd;
	int publickey;
	int model;
} RSAKEY, * LPRESKey;
typedef struct {
	unsigned char cmd;
	unsigned int key[8];
	int len;
	unsigned buf[100];
}DESDATA, * LPDESData;
#pragma pack(pop)

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

//接受连接产生通讯套接字
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
	//result = My_Getaddr(s);
	//My_Bind(s, result);
	My_Bind(s, (char*)("127.0.0.1"), DEFAULT_PORT_INT);
	My_Listen(s);
	printf("TCP server starting\n");
	char buf[DEFAULT_BUFLEN] = { 0 };



	WSAEVENT  h = WSACreateEvent();
	WSAResetEvent(h);
	WSAEventSelect(s, h, FD_ACCEPT);
	printf("网络数据安全传输----混合加密传输(服务器)\n");

	int len = sizeof(sockaddr_in);
	int iEventTotal = 0;
	WSAEVENT eventArray[WSA_MAXIMUM_WAIT_EVENTS] = { 0 };
	SOCKET sockArray[WSA_MAXIMUM_WAIT_EVENTS] = { 0 };
	int publickey[WSA_MAXIMUM_WAIT_EVENTS] = { 0 };
	int model[WSA_MAXIMUM_WAIT_EVENTS] = { 0 };
	eventArray[iEventTotal] = h;
	sockArray[iEventTotal] = s;
	iEventTotal++;

	while (1)
	{
		int index = WSAWaitForMultipleEvents(iEventTotal, eventArray, FALSE, WSA_INFINITE, FALSE);

		if (index == WSA_WAIT_FAILED)
		{
			printf("事件等待出错,程序退出\n");
			break;
		}
		else
		{
			index = index - WSA_WAIT_EVENT_0;
			WSANETWORKEVENTS newevent;

			WSAEnumNetworkEvents(sockArray[index], eventArray[index], &newevent);
			if (newevent.lNetworkEvents & FD_ACCEPT)
			{
				if (newevent.iErrorCode[FD_ACCEPT_BIT] == 0)
				{
					SOCKET Client = My_Accept(s, addr);
					eventArray[iEventTotal] = WSACreateEvent();
					sockArray[iEventTotal] = Client;
					WSAEventSelect(Client, eventArray[iEventTotal], FD_READ | FD_WRITE | FD_CLOSE);
					iEventTotal++;
					printf("连接成功  来自于:%s  \n", inet_ntoa(*(struct in_addr*)&(addrs.sin_addr.S_un.S_addr)));

				}
			} //if((FD_ACCEPT
			if (newevent.lNetworkEvents & FD_READ)
			{
				if (newevent.iErrorCode[FD_READ_BIT] == 0)
				{
					char buf[100] = { 0 };
					int r = recv(sockArray[index], buf, 100, 0);
					if (r > 0)
					{
						if (buf[0] == 1)
						{
							LPRESKey p = (LPRESKey)buf;
							publickey[index] = p->publickey;
							model[index] = p->model;
							printf("接收到公钥对:(%d %d)\n", publickey[index], model[index]);
							send(sockArray[index], buf, 1, 0);
						}
						if (buf[0] == 2)
						{
							unsigned int t[8] = { 0 };
							unsigned char deskey[8] = { 0 };
							LPDESData p = (LPDESData)buf;
							DeCode(p->key, t, 8, publickey[index], model[index]);
							printf("接收到的DES密钥为: ");
							for (int i = 0; i < 8; i++)
							{
								deskey[i] = (unsigned char)t[i];
								printf("%02x ", deskey[i]);
							}
							printf("\n");

							for (int i = 0; i < len; i++)
							{
								decrypt8((const unsigned char*)p->buf + i * 8, deskey, (unsigned char*)buf + i * 8);
							}
							printf("接收到的数据:%s\n", buf);
							buf[0] = 2;
							send(sockArray[index], buf, 1, 0);


						}
					}
					else
					{
						printf("接收失败！\n");
						closesocket(sockArray[index]);
					}

				}
			} //if((FD_READ
			if (newevent.lNetworkEvents & FD_CLOSE)
			{
				if (newevent.iErrorCode[FD_CLOSE_BIT] == 0)
				{

					printf("连接关闭\n");
					closesocket(sockArray[index]);
					WSACloseEvent(eventArray[index]);
					for (int i = index; i < iEventTotal - 1; i++)
					{
						sockArray[i] = sockArray[i + 1];
						eventArray[i] = eventArray[i + 1];

					}
					iEventTotal--;

				}
			} //if((FD_CLOSE
			if (newevent.lNetworkEvents & FD_WRITE)
			{
				if (newevent.iErrorCode[FD_WRITE_BIT] == 0)
				{



				}
			} //if((FD_WRITE

		}//if(index
	} //while

	closesocket(s);
	WSACleanup();
	return 0;
}