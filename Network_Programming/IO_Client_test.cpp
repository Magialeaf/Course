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

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
//初始化Winsock()
void InitSock()
{
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed with error: %d\n", iResult);
		exit(0);
	}
	printf("客户端启动成功\n");
}
//创建套接字，使用套接字通信
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
addrinfo* GetAddr(SOCKET s)
{
	struct addrinfo hints;
	struct addrinfo* result;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = 0;
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
//连接服务器
void My_Connect(SOCKET s, addrinfo* result)
{
	int iResult = connect(s, result->ai_addr, (int)result->ai_addrlen);
	if (iResult != 0)
	{
		closesocket(s);
		WSACleanup();
		exit(0);
	}
	printf("客户端连接成功\n");
}

int main(int argc, char** argv)
{
	struct addrinfo* result = NULL;
	InitSock();
	SOCKET s = CreateTCPSocket();
	result = GetAddr(s);
	My_Connect(s,result);

	char buf[100] = { 0 };
	printf("Input exit to exit\n");
	while (true)
	{
		printf(">>>");
		scanf("%s", &buf);
		if (strcmp(buf, "exit") == 0)
		{
			printf(">>>[exit!]\n");
			int r = send(s, buf, 0, 0);
			break;
		}
		else
		{
			int r = send(s, buf, strlen(buf), 0);
			if (r == strlen(buf))
			{
				printf(">>>[Send success!]\n");
			}
			else
			{
				printf(">>>[Send failed!]\n");
			}
			memset(buf, 0, sizeof(buf));
			int res = recv(s, buf, sizeof(buf),0);
			if (res > 0)
			{
				printf(">>>[Recv]:%s\n",buf);
			}
			else
			{
				printf(">>>[Recv failed!]\n");
			}
		}
	}
	closesocket(s);
	WSACleanup();
	return 0;
}