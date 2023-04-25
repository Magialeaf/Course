//修改：加入临界区,防止同时对用户列表写入，删除，读取。
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
#define MAX_USER 20    //用户最多数量
#define MAX_NAME 20    //名字最长长度
char UserList[MAX_USER][MAX_NAME] = {0};  //这个二维数组存放所有的用户名，数组第一行存放表头，用户名从第二行开始存放
SOCKET UserSocket[MAX_USER] = { 0 };      //存放所有的套接字
int UserListEnd = 0;  //二维数组行的个数

DWORD WINAPI Thread(LPVOID p)
{
	SOCKET news = (SOCKET)p;
	char buf[100] = { 0 };
	char new_buf[100] = { 0 };
	char name[20] = { 0 };
	char user[20] = { 0 };
	int i = 0,j = 0;
	while (1)
	{
		memset((void*)buf, 0, 100);
		int r = recv(news, buf, 100, 0);
		if (r > 0)
		{

			if (buf[0] == 1)
			{
				strcpy(user, buf + 1);
				if (UserListEnd < MAX_USER)
				{
					for (j = 0; j < UserListEnd; j++)
						if(strcmp(user,UserList[j]) == 0)
						{
							buf[0] = 11;
							send(news, buf, 1, 0);
							closesocket(news);
							break;
						}
					if (j == UserListEnd)
					{
						printf("[%s]登入\n", user);
						strcpy(UserList[UserListEnd], user);
						UserSocket[j] = news;
						UserListEnd++;
						send(news, buf, 1, 0);
					}
				}
			}
			if (buf[0] == 2)
			{
				for (i = 0; i < MAX_NAME + 1; i++)
				{
					if (buf[i + 1] != ':')
						name[i] = buf[i + 1];
					else
					{
						name[i] = '\0';
						i++;
						break;
					}
				}
				memset((void*)new_buf, 0, 100);
				strcpy(new_buf + 1, name);
				strcpy(new_buf + strlen(name) + 1, buf + i);
				for (j = 0; j < UserListEnd; j++)
					if (strcmp(name, UserList[j]) == 0)
					{
						new_buf[0] = 2;
						send(UserSocket[j], new_buf, strlen(new_buf), 0);
						break;
					}
				if (j == UserListEnd)
				{
					memset((void*)new_buf, 0, 100);
					new_buf[0] = 3;
					strcpy(new_buf + 1, name);
					send(news, new_buf, strlen(new_buf), 0);
					printf("[%s]->[%s]不存在\n", user, name);
				}
				else
					printf("[%s]->[%s]:%s\n", user, name, buf + i + 1);
			}
			if (buf[0] == 3)
			{
				printf("[%s]退出\n", user);
				for (j = 0; j < UserListEnd; j++)
					if (strcmp(user, UserList[j]) == 0)
					{
						memset((void*)UserList[j], 0, 20);
						UserSocket[j] = 0;
						break;
					}
				for (j; j < UserListEnd; j++)
				{
					strcpy(UserList[j], UserList[j + 1]);
					UserSocket[j] = UserSocket[j + 1];
				}
				UserListEnd--;
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
}



