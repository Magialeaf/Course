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

int main(int argc, char* argv[])
{
	int r;
	WSADATA w;
	r = WSAStartup(MAKEWORD(2, 2), &w);
	if (r == 0)
	{
		printf("客户端初始化成功!\n");
	}
	else
		printf("客户端初始化失败!");
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in  hints;
	hints.sin_family = AF_INET;
	hints.sin_port = htons(27015);
	hints.sin_addr.S_un.S_addr = inet_addr("172.22.17.81");
	r = connect(s, (struct sockaddr FAR*) & hints, sizeof(hints));
	if (r != 0)
	{
		printf("连接服务器失败");
	}
	else
	{
		char buf[100] = { 0 };
		printf("输入用户名");
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
					printf("登入成功，可以聊天了\n输入要发送的数据，输入#退出\n");
					while (1)
					{
						memset((void*)buf, 0, 100);
						scanf("%s", buf + 1);
						if (!strcmp(buf + 1, "#"))
						{
							buf[0] = 3;
							send(s, buf, 1, 0);
							break;
						}
						else
						{
							buf[0] = 2;
							send(s, buf, strlen(buf + 1) + 1, 0);
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
	closesocket(s);
	WSACleanup();
	return 0;
}

