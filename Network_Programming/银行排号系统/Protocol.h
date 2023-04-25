#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<stdio.h>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<stdlib.h>
#include<conio.h>
#include <time.h>
//连接到WinSock2对应的文件:Ws2_32.lib, Mswsock.lib, Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")// IO_Server_Non_Block.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

/****************************************声明协议********************************************/
//默认数据长度
#define DEFAULT_BUFLEN 64
//默认端口号
#define DEFAULT_PORT "27015"
#define DEFAULT_PORT_INT 27015

//收到信息
#define Get_Message 0
#define Bus_Open 1        //机器已开启（业务机）(机器号：1,2,3全部)
#define Bus_Close 2       //机器未开启（业务机) (机器号：1,2,3全部)
//#define Get_Open 3        //机器已开启（取号机）
//#define Get_Close 4       //机器未开启（取号机）
//#define Call_Open 5       //机器已开启（叫号机）
#define Call_Close 6      //机器未开启（叫号机）
//取号
#define Get_Proc 10		 //取号程序
#define Get_Num 15	     //取号
#define Get_Num_Succ 11  //取号成功 ([状态 号码十位 号码个位])
#define Get_Num_Fail 12  //取号失败
//叫号( 协定[状态 机器号 机器状态 号码十位 号码个位] )机器号:0,2 状态：0当前,1下一)
#define Call_Proc 20     //叫号程序  
#define Call_Num 21      //叫号
#define Del_Num  22      //取消叫号 
//业务
#define Business_Proc 30 //业务程序
#define Bus_Now 31       //当前业务号码 (状态 十位 个位)
#define Bus_Next 32      //下一业务号码 (状态 十位 个位)
#define Bus_End 35       //业务结束
//#define Regist 36        //注册（账号与密码分个符为 | ）
//#define Login 37	     //登入（账号与密码分个符为 | ）
//#define Put_Money 38     //存钱
//#define Get_Money 39     //取钱
//#define Bus_Other 40     //其他业务
//#define Bus_Right 41     //操作正确
//#define Bus_Wrong 42     //操作错误

//#define Error_Type 99    //出错

/****************************************声明函数********************************************/
//定义存储数据
char buf[DEFAULT_BUFLEN];
//初始化Winsock()
void InitSock();
//创建套接字，使用套接字通信
SOCKET CreateTCPSocket();
//获得IP地址和端口号
addrinfo* My_GetAddr(SOCKET s);
//连接服务器
void My_Connect(SOCKET s, addrinfo* result);
//绑定端口号和IP地址
void My_Bind(SOCKET s, addrinfo* result);
//监听绑定	
void My_Listen(SOCKET s);
//接受连接产生通讯套接字
SOCKET My_Accept(SOCKET s, sockaddr_in* addr);
/****************************************定义函数********************************************/

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
addrinfo* My_GetAddr(SOCKET s)
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
		printf("[ 服务器未开启! ]\n");
		closesocket(s);
		WSACleanup();
		system("pause");
		exit(0);
	}
}

//绑定端口号和IP地址
void My_Bind(SOCKET s, addrinfo* result)
{
	int r = bind(s, result->ai_addr, (int)result->ai_addrlen);
	if (r == SOCKET_ERROR)
	{
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(s);
		WSACleanup();
		exit(0);
	}
}

//监听绑定	
void My_Listen(SOCKET s)
{
	int iResult = listen(s, SOMAXCONN);
	if (iResult == SOCKET_ERROR)
	{
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(s);
		WSACleanup();
		exit(0);
	}
}

//接受连接产生通讯套接字
SOCKET My_Accept(SOCKET s, sockaddr_in* addr)
{
	sockaddr_in hints;
	int len = sizeof(sockaddr);
	SOCKET Client_s = accept(s, (sockaddr FAR*) & hints, &len);
	if (Client_s == INVALID_SOCKET)
	{
		printf("socket failed with error: %ld\n", WSAGetLastError());
		closesocket(s);
		WSACleanup();
		exit(0);
	}
	else
	{
		addr->sin_family = hints.sin_family;
		addr->sin_port = hints.sin_port;
		addr->sin_addr = hints.sin_addr;
	}
	return Client_s;
}
/*
设计思路:
*/


//验证协议
//客户机 -> 服务器: 身份
//服务器 -> 客户机: 收到消息 身份 [业务号]

//(（正常状态下所有程序依照服务器关闭自动关闭），（服务器检测其他程序是否意外关闭，意外关闭要删除套接字）)
/*
协议：
取号程序-->服务器: 10告知身份 0收到信息 15取号
服务器-->取号程序:		   11取号成功 12号码已满
叫号程序-->服务器: 20告知身份 0收到信息
服务器-->叫号程序:		   21叫号 22取消叫号 
业务程序-->服务器: 30告知身份 0收到信息 35业务结束 36存钱 37取钱 38其他业务
服务器-->业务程序:		   31当前办理号码 32下一办理号码
*/