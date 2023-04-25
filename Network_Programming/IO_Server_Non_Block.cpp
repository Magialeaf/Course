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
//设置套接字为非阻塞模式
void My_Ioctlsocket(SOCKET s)
{	
	int iMode = 1;
	int iResult = ioctlsocket(s, FIONBIO, (u_long*)&iMode);
	if (iResult == SOCKET_ERROR)
	{
		printf("ioctlsocket failed with error: ld%\n", WSAGetLastError());
		closesocket(s);
		WSACleanup();
		exit(0);
	}
}
//循环等待客户请求建立连接，并处理连接请求
SOCKET My_Accept(SOCKET s, sockaddr_in* addr)
{
	sockaddr_in hints;
	SOCKET Client_s;
	int err;
	int len = sizeof(sockaddr);
	while (true)
	{
		Client_s = accept(s, (sockaddr FAR*) & hints, &len);
		if (Client_s == INVALID_SOCKET)
		{
			err = WSAGetLastError();
			if (err == WSAEWOULDBLOCK)
			{
				//printf("The operation on the non-blocking socket cannot be completed immediately.\n");
				Sleep(1000);
				continue;
			}
			else
			{
				printf("socket failed with error: %ld\n", WSAGetLastError());
				closesocket(s);
				WSACleanup();
				exit(0);
			}
		}
		else
		{
			printf("Get Client!\n");
			addr->sin_family = hints.sin_family;
			addr->sin_port = hints.sin_port;
			addr->sin_addr = hints.sin_addr;
			break;
		}
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
	My_Ioctlsocket(s);
	My_Listen(s);
	printf("TCP server starting\n");
	char buf[DEFAULT_BUFLEN] = { 0 };
	SOCKET client = My_Accept(s, addr);
	//循环接收数据
	while (true)
	{
		memset(buf, 0, sizeof(buf));
		int iResult = recv(client, buf, sizeof(buf), 0);
		if (iResult > 0)
		{
			//情况1：成功接收到数据
			printf("[Recv]:%s\n",buf);
			send(client, buf, strlen(buf), 0);
			continue;
		}
		else if (iResult == 0)
		{
			//情况2：连接关闭
			printf("Current Connection closing, waiting for the next connection...\n");
			closesocket(client);
			break;
		}
		else
		{
			//情况3：接收发生错误
			int err = WSAGetLastError();
			if (err = WSAEWOULDBLOCK)
			{
				//无法立即完成非阻塞套接字上的操作
				//printf("The operation on the non-blocking socket cannot be completed immediately.\n");
				Sleep(1000);
				continue;
			}
			else
			{
				printf("recv failed with error: %d\n", err);
				closesocket(client);
				closesocket(s);
				exit(0);
			}
		}
	}
	closesocket(client);
	closesocket(s);
	WSACleanup();
	return 0;
}
