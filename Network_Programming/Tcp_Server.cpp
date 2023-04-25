#undef UNICODE
#define WIN32_LEAN_AND_MEAN
#include<windows.h>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<stdlib.h>
#include<stdio.h>
//连接到 WinSock 2 对应的lib文件：Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
//定义默认的缓冲区长度和端口号
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

int __cdecl main(void)
{
	WSADATA wsaData;
	int iResult;
	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;
	struct addrinfo* result = NULL;
	struct addrinfo hints;
	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	//初始化WinSock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}
	ZeroMemory(&hints, sizeof(hints));
	//声明IPv4地址族，流式套接字，TCP协议
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	//解析服务器地址和端口号
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}
	//为面向连接的服务器创建套接字
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET)
	{
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}
	//为套接字绑定地址和端口号
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	freeaddrinfo(result);
	//监听连接请求
	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR)
	{
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	printf("Server initialization succeeded!\n");
	//接受客户端的连接请求，返回连接套接字ClientSocket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET)
	{
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	//在必须要监听套接字的情况下释放该套接字
	closesocket(ListenSocket);
	//持续接收数据，直到对方关闭连接
	do
	{
		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
		{
			//情况1：成功收到数据
			printf("Bytes received: %d\n", iResult);
			//将缓冲区的内容回送给客户端
			iSendResult = send(ClientSocket, recvbuf, iResult, 0);
			if (iSendResult == SOCKET_ERROR)
			{
				printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				return 1;
			}
			printf("Bytes sent: %d\n", iSendResult);
		}
		else if (iResult == 0)
		{
			//情况2：连接关闭
			printf("Connection closing...\n");
		}
		else
		{
			//情况3：接收错误
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}
	} while (iResult > 0);
	//关闭连接
	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR)
	{
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}
	//关闭套接字，释放资源
	closesocket(ClientSocket);
	WSACleanup();
	system("pause");
	return 0;
}