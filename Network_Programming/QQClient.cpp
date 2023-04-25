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

#define MAX_NAME 20
#define DEFAULT_PORT "27015"
DWORD WINAPI get(LPVOID p)
{
	SOCKET news = (SOCKET)p;
	char buf[100] = { 0 };
	char name[MAX_NAME];
	int i = 0;
	while (1)
	{
		memset((void*)buf, 0, 100);
		int r = recv(news, buf, 100, 0);
		if (r > 0)
		{
			if (buf[0] == 2)
			{
				for (i = 0; i < MAX_NAME + 1; i++)
				{
					if (buf[i + 1] != ':')
						name[i] = buf[i + 1];
					else
					{
						name[i] = '\0';
						break;
					}
				}
				printf("[%s]%s\n>>>", name, buf + i + 1);
			}
			if (buf[0] == 3)
			{
				printf("[发送失败!用户%s不存在]\n>>>",buf + 1);
			}
		}
	}
	return 0;
}

int main(int argc, char* argv[])
{
	int r;
	WSADATA w;
	struct addrinfo* result = NULL;
	struct addrinfo hints;
	int iResult;
	r = WSAStartup(MAKEWORD(2, 2), &w);
	if (r == 0)
	{
		printf("客户端初始化成功!\n");
	}
	else
		printf("客户端初始化失败!");
	ZeroMemory(&hints, sizeof(hints));
	//声明IPv4地址族，流式套接字，TCP协议
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = 0;
	//解析服务器地址和端口号
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}
	SOCKET s = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	r = connect(s, result->ai_addr, (int)result->ai_addrlen);
	if (r != 0)
	{
		printf("连接服务器失败");
	}
	else
	{
		char buf[100] = { 0 };
		printf("输入用户名:");
		scanf("%s", buf + 1);
		buf[0] = 1;
		int r = send(s, buf, strlen(buf + 1) + 1, 0);
		if (r > 0)
		{
			memset((void*)buf, 0, 100);
			r = recv(s, buf, 100, 0);
			if (r > 0)
			{
				if (buf[0] == 1)
				{
					printf("登入成功，可以聊天了\n输入[接收方名称]:[发送内容]发送数据\n");
					printf("接受[发送方名称]:[接收内容]接受数据\n输入exit退出\n");
					CreateThread(0, 0, get, (LPVOID)s, 0, 0);
					while (1)
					{
						memset((void*)buf, 0, 100);
						printf(">>>");
						scanf("%s", buf + 1);
						if (!strcmp(buf + 1, "exit"))
						{
							buf[0] = 3;
							r = send(s, buf, 1, 0);
							break;
						}
						else
						{
							buf[0] = 2;
							r = send(s, buf, strlen(buf + 1) + 1, 0);
						}
						if (r > 0)
						{
							printf(">>>[发送成功]\n");
						}
					}
				}
				else
					if (buf[0] == 11)
					{
						printf("登入失败!\n");

					}
					else
						printf("数据报格式错误!\n");
			}
			else
				printf("recv bad\n");

		}
		else
		{
			printf("send bad");

		}
	}
	system("pause");
	closesocket(s);
	WSACleanup();
	return 0;
}

