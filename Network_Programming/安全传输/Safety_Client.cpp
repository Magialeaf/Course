#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<stdio.h>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<stdlib.h>
#include <time.h>
#include "KEY_PRODUCE.h"
#include "des.h"
//连接到WinSock2对应的文件:Ws2_32.lib, Mswsock.lib, Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

#pragma pack(push,1)
typedef struct {
	unsigned char cmd;
	int publickey;
	int model;
} RSAKEY, * LPRESKey;
typedef struct {
	unsigned char cmd;
	unsigned int key[8];
	int len;
	unsigned buf[100];
}DESDATA, * LPDESData;
#pragma pack(pop)

//初始化Winsock()
void InitSock()
{
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
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
	My_Connect(s, result);


	unsigned int in[8] = { 0 };
	unsigned int out[8] = { 0 };
	char code[100] = { 0 };
	int PublicKey = 0;
	int PrivateKey = 0;
	int Model = 0;
	GetKey(PublicKey, PrivateKey, Model);
	printf("生成公钥对(%d %d)，私公钥对(%d %d)\n", PublicKey, Model, PrivateKey, Model);

	srand(time(0));
	unsigned char deskey[8] = { 0 };
	for (int i = 0; i < 8; i++) deskey[i] = (unsigned char)rand();
	printf("生成的DES密钥为: ");
	for (int i = 0; i < 8; i++) printf("%02x ", deskey[i]);
	printf("\n");

	printf("数据要发送的数据\n");
	scanf("%s", code);

	RSAKEY rsa;
	rsa.cmd = 1;
	rsa.publickey = PublicKey;
	rsa.model = Model;
	int r = send(s, (const char*)&rsa, sizeof(rsa), 0);
	if (s < 0)
	{
		printf("发送数据失败!\n");
		closesocket(s);
		return 0;
	}
	char c;
	r = recv(s, (char*)&c, 1, 0);
	if (r <= 0)
	{
		printf("接收数据失败!\n");
		closesocket(s);
		return 0;
	}
	if (c == 1)
	{
		printf("公钥发送成功!\n");
	}
	else
	{
		printf("公钥发送失败!\n");
		closesocket(s);
		return 0;

	}

	for (int i = 0; i < 8; i++) in[i] = deskey[i];
	EnCode(in, out, 8, PrivateKey, Model);

	unsigned char out1[100] = { 0 };

	int i = strlen(code);
	int len = 0;
	if ((i % 8) != 0) len = (i / 8) + 1;
	else len = i / 8;

	for (i = 0; i < len; i++)
	{
		encrypt8((unsigned char*)code + i * 8, deskey, out1 + i * 8);
	}

	DESDATA des;
	des.cmd = 2;
	memcpy((void*)des.key, (const void*)out, 8 * sizeof(int));
	memcpy((void*)des.buf, (const void*)out1, len * 8);
	des.len = len;
	r = send(s, (const char*)&des, sizeof(des), 0);
	if (s < 0)
	{
		printf("发送数据失败!\n");
		closesocket(s);
		return 0;
	}
	r = recv(s, (char*)&c, 1, 0);
	if (r <= 0)
	{
		printf("接收数据失败!\n");
		closesocket(s);
		return 0;
	}
	if (c == 2)
	{
		printf("传输数据发送成功!\n");
	}
	else
	{
		printf("传输数据发送失败!\n");

	}

	closesocket(s);
	WSACleanup();
	return 0;
}