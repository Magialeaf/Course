#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAM_AND_MEAN
#include<stdio.h>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<stdlib.h>
//连接到WinSock2对应的文件:Ws2_32.lib, Mswsock.lib, Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
//定义默认的缓冲区长度和端口号
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

int __cdecl main(int argc, char** argv)
{
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo* result = NULL, * ptr = NULL, hints;
	const char* sendbuf = "this is a test";
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;

	//验证参数合法性
	if (argc != 2)   
	{
		printf("usage: %s server-name\n", argv[0]);
		return 1;
	}
	//初始化套接字
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}
	ZeroMemory(&hints, sizeof(hints));//用0填充一片内存
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	//解析服务器地址和端口号
	iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}
	//尝试连接服务器，直到成功
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{
		//创建套接字
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET)
		{
			printf("socket failed with error: %d\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}
		//向服务器请求连接
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR)
		{
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}
	freeaddrinfo(result);
	if (ConnectSocket == INVALID_SOCKET)
	{
		printf("Unable to connect to server\n");
		WSACleanup();
		return 1;
	}
	//发送缓冲区中的数据
	iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR)
	{
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}
	printf("Bytes Sent: %ld\n", iResult);
	//数据发送结束，调用shutdown()函数声明不再发送数据，此时客户端仍可以接受数据
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR)
	{
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}
	//持续接收数据，直到服务器关闭
	do
	{
		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
			printf("Bytes received: %d\n", iResult);
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed with error: %d\n",WSAGetLastError());
	} while (iResult > 0);
	//关闭套接字
	closesocket(ConnectSocket);
	//释放资源
	WSACleanup();
	return 0;
}

