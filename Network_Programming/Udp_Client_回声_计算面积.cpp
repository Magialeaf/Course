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

//客户端回声程序
int main(int argc, char* argv[])
{
	//初始化
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}
	//创建套接字
	SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (s == INVALID_SOCKET)
	{
		printf("socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 0;
	}
	//初始化地址
	struct sockaddr_in hints;
	hints.sin_family = AF_INET;
	hints.sin_port = htons(4000);
	hints.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	char buf[100];
	struct sockaddr_in addr;
	int len_hints = sizeof(hints);
	int len_addr = sizeof(addr);
	while (true)
	{
		printf("输入发送数据（输入'#'退出）:");
		scanf("%s", buf);
		if (buf[0] == '#')
			break;
		int i = sendto(s, buf, strlen(buf), 0, (struct sockaddr*)&hints, len_hints);
		if (i >= 0)
		{
			printf("发送的数据为:%s\n", buf);
			memset(buf, 0, 100);
			i = recvfrom(s, buf, 100, 0, (struct sockaddr*)&addr, &len_addr);
			if (i >= 0)
			{
				printf("接收的数据为:%s\n", buf);
			}
			else
				printf("无返回数据!\n");
		}
	}
	closesocket(s);
	WSACleanup();
	return 0;
}

//——————————————————————————————————————————————————————

//客户端面积输入
int main(int argc, char* argv[])
{
	//初始化
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}
	//创建套接字
	SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (s == INVALID_SOCKET)
	{
		printf("socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 0;
	}
	//初始化地址
	struct sockaddr_in hints;
	hints.sin_family = AF_INET;
	hints.sin_port = htons(4000);
	hints.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	char buf[100];
	struct sockaddr_in addr;
	int len_hints = sizeof(hints);
	int len_addr = sizeof(addr);

	int a, b;
	printf("输入长:");
	scanf("%d", &a);
	printf("输入宽:");
	scanf("%d", &b);
	*(int*)buf = a;
	*(int*)(buf + sizeof(int)) = b;
	int i = sendto(s, buf, 100, 0, (struct sockaddr*)&hints, len_hints);
	if (i >= 0)
	{
		printf("输入的矩形长为:%d,宽为:%d\n", *(int*)buf, *(int*)(buf + sizeof(int)));
		i = recvfrom(s, buf, 100, 0, (struct sockaddr*)&addr, &len_addr);
		if (i >= 0)
		{
			printf("该矩形面积为:%d\n", *(int*)(buf + sizeof(int) * 2));
		}
		else
			printf("无返回数据!\n");
	}

	closesocket(s);
	WSACleanup();
	return 0;
}