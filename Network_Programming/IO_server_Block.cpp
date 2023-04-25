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
#pragma comment (lib, "AdvApi32.lib")

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
//获得IP地址和端口号
addrinfo* My_Getaddr(SOCKET s)
{
  struct addrinfo* result = NULL;
	struct addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	int iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		printf("getaddrinfo failed with error: %d\n", iResult);
		//freeaddrinfo(&hints);
		//freeaddrinfo(result);
		closesocket(s);
 		WSACleanup();
		exit(0);
	}
	return result;
}
//绑定端口号和IP地址
void My_Bind(SOCKET s, addrinfo* result)
{
	int r = bind(s, result->ai_addr, (int)result->ai_addrlen);
	if (r == SOCKET_ERROR)
	{
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(s);
		WSACleanup();
		exit(0);
	}
}
////绑定端口号和IP地址
//void My_Bind(SOCKET s, char* Ip, int port)
//{
//	struct sockaddr_in hints;
//	hints.sin_family = AF_INET;
//	hints.sin_port = htons(port);
//	hints.sin_addr.S_un.S_addr = inet_addr(Ip);
//	int r = bind(s, (const sockaddr*)&hints, sizeof(hints));
//	if (r == SOCKET_ERROR)
//	{
//		printf("bind failed with error: %d\n", WSAGetLastError());
//		closesocket(s);
//		WSACleanup();
//		exit(0);
//	}
//}
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
SOCKET My_Accept(SOCKET s,sockaddr_in* addr)
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
	InitSock();
	SOCKET s = CreateTCPSocket();
	addrinfo* result = My_Getaddr(s);
	My_Bind(s, result);
	//My_Bind(s, (char*)("172.22.188.40"), DEFAULT_PORT_INT);
	My_Listen(s);

	char buf[DEFAULT_BUFLEN] = { 0 };
	while (true)
	{
		SOCKET Client_s = My_Accept(s, addr);
		while (true)
		{
			memset(buf, 0, sizeof(buf));
			int iResult = recv(Client_s, buf, sizeof(buf), 0);
			if (iResult > 0)
			{
				//成功接受
				printf("[%s:%d]:%s\n", inet_ntoa(addr->sin_addr), addr->sin_port,buf);
				send(Client_s, buf, strlen(buf), 0);
				continue;
			}
			else if (iResult == 0)
			{
				printf("[%s:%d]:[close!]\n", inet_ntoa(addr->sin_addr), addr->sin_port);
				closesocket(Client_s);
				break;
			}
			else
			{
				printf("recv failed with error: %d\n", WSAGetLastError());
				closesocket(Client_s);
				closesocket(s);
				WSACleanup();
				return 0;
			}
		}
	}
	closesocket(s);
	WSACleanup();
	return 0;
}

