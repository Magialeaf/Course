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
//服务器接受回声程序
/*int main(int argc, char* argv[])
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

	bind(s, (struct sockaddr*)&hints, len_hints);


	printf("服务器初始化成功!\n");
	while (true)
	{
		int i = recvfrom(s, buf, 100, 0, (struct sockaddr*)&addr, &len_addr);
		if (i >= 0)
		{
			buf[i] = '\0';
			printf("接收的数据为:%s,客户端IP为:%s,端口号为%d\n", buf, inet_ntoa(addr.sin_addr), addr.sin_port);
			i = sendto(s, buf, strlen(buf), 0, (struct sockaddr*)&addr, len_addr);
		}
	}

	closesocket(s);
	WSACleanup();
	return 0;
}*/

//——————————————————————————————————————————————————————

//服务器计算面积功能
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

	char buf[100] = { 0 };
	struct sockaddr_in addr;
	int len_hints = sizeof(hints);
	int len_addr = sizeof(addr);

	bind(s, (struct sockaddr*)&hints, len_hints);


	printf("服务器初始化成功!\n");
	while (true)
	{
		int a, b, S;
		int i = recvfrom(s, buf, 100, 0, (struct sockaddr*)&addr, &len_addr);
		a = *(int*)buf;
		b = *(int*)(buf + sizeof(int));
		S = a * b;
		*(int*)(buf + sizeof(int) * 2) = S;
		printf("接受的长为:%d,宽为:%d,计算后的面积结果为:%d\n", a, b, S);
		sendto(s, buf, 100, 0, (struct sockaddr*)&addr, len_addr);
	}

	closesocket(s);
	WSACleanup();
	return 0;
}
