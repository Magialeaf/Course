#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<stdio.h>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<stdlib.h>
//连接到WinSock2对应的文件:Ws2_32.lib, Mswsock.lib, Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#define DEFAULT_PORT "27015"
char UserList[20][30] = { "用户列表" };  //这个二维数组存放所有的用户名，数组第一行存放表头，用户名从第二行开始存放
int UserListEnd = 1;  //二维数组行的个数

DWORD WINAPI Thread(LPVOID p)
{
	SOCKET news = (SOCKET)p;
	char buf[100] = { 0 };
	char user[20] = { 0 };
	while (1)
	{
		memset((void*)buf, 0, 100);
		int r = recv(news, buf, 100, 0);
		if (r > 0)
		{

			if (buf[0] == 1)
			{
				strcpy(user, buf + 1);
				if (!strcmp(user, "lyb") || !strcmp(user, "abc"))
				{
					printf("登陆用户名为:%s\n", user);
					send(news, buf, 1, 0);
					printf("r:%d\n", (int)buf[0]);
				}
				else
				{
					buf[0] = 11;
					send(news, buf, 1, 0);
					closesocket(news);
					break;
				}


			}
			if (buf[0] == 2)
			{
				printf("[%s]的聊天信息:%s\n", user, buf + 1);
				send(news, buf, 1, 0);
			}
			if (buf[0] == 3)
			{
				printf("[%s]退出\n", user);
				closesocket(news);
				break;
			}
		}
		else
		{
			closesocket(news);
			break;
		}
	}
	return 0;

}
int main(int argc, char* argv[])
{
	int r;
	WSADATA w;
	int iResult;
	r = WSAStartup(MAKEWORD(2, 2), &w);
	if (r == 0)
	{
		printf("服务器初始化成功!\n");
	}
	else
		printf("服务器初始化失败!");
	struct addrinfo* result = NULL;
	struct addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}
	SOCKET s = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	r = bind(s, result->ai_addr, (int)result->ai_addrlen);
	if (r != 0)
	{
		printf("绑定服务器失败");
	}
	else
	{
		listen(s, 5);
		struct sockaddr_in  client;
		int l = sizeof(client);
		while (1)
		{
			SOCKET news = accept(s, (struct sockaddr FAR*) & client, &l);
			if (news == INVALID_SOCKET)
			{
				printf("接受连接失败\n");
				continue;
			}
			CreateThread(0, 0, Thread, (LPVOID)news, 0, 0);
		}
	}
	closesocket(s);
	WSACleanup();
	return 0;
	return 0;
}

