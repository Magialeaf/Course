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

#define SIO_RCVALL  0x98000001
#define DEFAULT_BUFLEN 65535
#define DEFAULT_NAMELEN 512
void transform(char* res, int pro);
int main(int argc, char* argv[])
{

	WSADATA wsaData;
	SOCKET SnifferSocket = INVALID_SOCKET;
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;
	HOSTENT* local;
	char HostName[DEFAULT_NAMELEN];
	IN_ADDR addr;
	SOCKADDR_IN LocalAddr, RemoteAddr;
	int addrlen = sizeof(SOCKADDR_IN);
	int in = 0, i = 0;
	DWORD dwBufferLen;
	DWORD Optval = 1;
	DWORD dwBytesReturned = 0;

	//初始化套接字
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("初始化失败\n");
		return 1;
	}
	//创建套接字
	SnifferSocket = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
	if (INVALID_SOCKET == SnifferSocket)
	{
		printf("创建套接字失败，失败码：%d\n", WSAGetLastError());;
		WSACleanup();
		return 1;
	}
	//获取本机名称
	memset(HostName, 0, DEFAULT_NAMELEN);
	iResult = gethostname(HostName, sizeof(HostName));
	if (SOCKET_ERROR == iResult)
	{
		printf("获取本机名称失败，失败码：%d\n", WSAGetLastError());;
		WSACleanup();
		return 1;
	}
	//获取本机IP
	local = gethostbyname(HostName);
	printf("本机可用IP有：\n");
	if (NULL == local)
	{
		printf("获取本机IP失败，失败码：%d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	while (local->h_addr_list[i] != 0)
	{
		addr.s_addr = *(u_long*)local->h_addr_list[i++];
		printf("\t %d: \t%s\n", i, inet_ntoa(addr));
	}
	printf("请选择捕获数据包待使用的接口号：\n");
	scanf("%d", &in);
	memset(&LocalAddr, 0, sizeof(LocalAddr));
	memcpy(&LocalAddr.sin_addr.S_un.S_addr, local->h_addr_list[in - 1], sizeof(LocalAddr.sin_addr.S_un.S_addr));
	LocalAddr.sin_family = AF_INET;
	LocalAddr.sin_port = 0;
	//绑定
	iResult = bind(SnifferSocket, (SOCKADDR*)&LocalAddr, sizeof(LocalAddr));
	if (SOCKET_ERROR == iResult)
	{
		printf("绑定失败，失败码：%d\n", WSAGetLastError());
		closesocket(SnifferSocket);
		WSACleanup();
		return 1;
	}
	printf("成功绑定套接字和%d号接口地址\n", in);
	//设置套接字接收命令
//	iResult = WSAIoctl(SnifferSocket, SIO_RCVALL, &Optval, sizeof(Optval), &dwBufferLen, sizeof(dwBufferLen), &dwBytesReturned, NULL, NULL);
	iResult = ioctlsocket(SnifferSocket, SIO_RCVALL, &Optval);//, sizeof(Optval), &dwBufferLen, sizeof(dwBufferLen), &dwBytesReturned, NULL, NULL);
	if (SOCKET_ERROR == iResult)
	{
		printf("套接字设置失败，失败码：%d\n", WSAGetLastError());
		closesocket(SnifferSocket);
		WSACleanup();
		return 1;
	}
	//开始接收数据
	printf("开始接收数据\n");
	do
	{
		//接收数据
		iResult = recvfrom(SnifferSocket, recvbuf, DEFAULT_BUFLEN, 0, (SOCKADDR*)&RemoteAddr, &addrlen);
		if (iResult > 0)
		{
			printf("接收来自于 %s的数据包,长度为%d,协议为:", inet_ntoa(RemoteAddr.sin_addr), iResult);
			switch (recvbuf[9])
			{
			case 1:printf("ICMP\n"); break;
			case 2:printf("IGMP\n"); break;
			case 4:printf("IP\n"); break;
			case 6:printf("TCP\n"); break;
			case 17:printf("UDP\n"); break;
			case 47:printf("GRE\n"); break;
			case 89:printf("OSPF\n"); break;
			default:break;
			}
		}
		else
			printf("接收失败，失败码为：%d\n", WSAGetLastError());
	} while (iResult > 0);

	system("pause");
	return 0;
}
