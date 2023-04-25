#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<stdio.h>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<stdlib.h>
//连接到WinSock2对应的文件:Ws2_32.lib, Mswsock.lib, Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")// IO_Server_Non_Block.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

#define DEFAULT_BUFLEN 256
#define DEFAULT_PORT 27015
//PER_TO_DATA结构用于保存单I/O操作的相关数据。
typedef struct
{
	OVERLAPPED Overlapped;	//重叠结构
	WSABUF DataBuf;			//缓冲区对象
	CHAR Buffer[DEFAULT_BUFLEN];	//缓冲区数组
	DWORD BytesRECV;		//接收的字节数
}PER_IO_DATA, * LPPER_IO_DATA;
//PER_HANDLE_DATA结构用于保存单句柄数据，此处为与客户进行通信的套接字。
typedef struct
{
	SOCKET Socket;
}PER_HANDLE_DATA, * LPPER_HANDLE_DATA;

DWORD WINAPI ServerWorkerThread(LPVOID CompletionPortID)
{
	HANDLE CompletionPort = (HANDLE)CompletionPortID;	//完成端口句柄
	DWORD BytesTransferred;								//数据传输的字节数
	LPPER_HANDLE_DATA PerHandleData;					//套接字句柄结构
	LPPER_IO_DATA PerIoData;							//I/O操作结构
	DWORD RecvBytes;									//接收的数量
	DWORD flags;										//WSARecv()函数中标志位

	while (TRUE)
	{
		//检查完成端口的状态
		if (GetQueuedCompletionStatus(CompletionPort, &BytesTransferred, (PULONG_PTR)&PerHandleData, (LPOVERLAPPED*)&PerIoData, INFINITE) == 0)
		{
			printf("GetQueuedCompletionStatus failed\n");
			exit(0);
		}
		//如果数据传完了，则退出
		if (BytesTransferred == 0)
		{
			printf("Closing socket %d\n", PerHandleData->Socket);
			//关闭套接字
			if (closesocket(PerHandleData->Socket) == SOCKET_ERROR)
			{
				printf("closesocket failed with error %d\n", WSAGetLastError());
				exit(0);
			}
			//释放结构资源
			GlobalFree(PerHandleData);
			GlobalFree(PerIoData);
			continue;
		}
		//如果还没有记录接收的数据数量，则将收到的字节数保存在PerIoData->BytesRECV中
		if (PerIoData->BytesRECV == 0)
			PerIoData->BytesRECV = BytesTransferred;
		//成功接收到数据
		printf("Bytes received: %d\n", BytesTransferred);
		//处理数据请求
		//......
		PerIoData->BytesRECV = 0;
		flags = 0;
		ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));
		PerIoData->DataBuf.len = DEFAULT_BUFLEN;
		PerIoData->DataBuf.buf = PerIoData->Buffer;
		RecvBytes = strlen((char*)&(PerIoData->DataBuf));
		//int iResult = WSARecv(PerHandleData->Socket, &(PerIoData->DataBuf), 1, &RecvBytes, &flags, &(PerIoData->Overlapped), NULL);
		int iResult = recv(PerHandleData->Socket, (char*)&(PerIoData->DataBuf), RecvBytes, 0);
		if (iResult == SOCKET_ERROR)
		{
			if (WSAGetLastError() != ERROR_IO_PENDING)
			{
				printf("WSARecv() failed with error %d\n", WSAGetLastError());
				exit(0);
			}
		}
		else
			send(PerHandleData->Socket, (char*)&(PerIoData->DataBuf), strlen((char*)&PerIoData->DataBuf), 0);
	}
}


int main(int argc, char** argv)
{
	SOCKADDR_IN InternetAddr;				//服务器地址
	SOCKET ServerSocket = INVALID_SOCKET;	//监听套接字
	SOCKET AcceptSocket = INVALID_SOCKET;	//与客户进行通信的套接字
	HANDLE CompletionPort;					//完成端口句柄
	SYSTEM_INFO SystemInfo;					//系统信息（这里主要用于获取CPU数量）
	LPPER_HANDLE_DATA PerHandleData;		//套接字句柄结构
	LPPER_IO_DATA PerIoData;				//I/O操作结构
	DWORD RecvBytes;						//接收到的字节数
	DWORD flags;							//WSARecv()函数中指定的标志位
	DWORD ThreadID;							//工作线程编号
	WSADATA wsaData;						//Windows Socket初始化信息
	DWORD Ret;								//函数返回值

	//创建新的完成端口
	if ((CompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0)) == NULL)
	{
		printf("CreateIoCompletionPort failed! \n");
		exit(0);
	}
	//获取系统信息
	GetSystemInfo(&SystemInfo);
	//根据CPU数量启动线程
	for (int i = 0; i < SystemInfo.dwNumberOfProcessors * 2; i++)
	{
		HANDLE ThreadHandle;
		//创建线程，运行ServerWorkerThread()函数
		if ((ThreadHandle = CreateThread(NULL, 0, ServerWorkerThread, CompletionPort, 0, &ThreadID)) == NULL)
		{
			printf("CreateThread() failed with error: %d\n", GetLastError());
			exit(0);
		}
		CloseHandle(ThreadHandle);
	}
	//初始化Windows Sockets环境
	if ((Ret = WSAStartup(0x0202, &wsaData)) != 0)
	{
		printf("WSAStartup failed with error: %d\n", Ret);
		exit(0);
	}
	//创建监听套接字
	ServerSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (ServerSocket == INVALID_SOCKET)
	{
		printf("WSASocket() failed with error: %d\n", WSAGetLastError());
		exit(0);
	}
	//绑定到本地地址端口
	InternetAddr.sin_family = AF_INET;
	InternetAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	InternetAddr.sin_port = htons(DEFAULT_PORT);
	int iResult = bind(ServerSocket, (PSOCKADDR)&InternetAddr, sizeof(InternetAddr));
	if (iResult == SOCKET_ERROR)
	{
		printf("bind() failed with error: %d\n", WSAGetLastError());
		exit(0);
	}
	//开始监听
	if (listen(ServerSocket, 5) == SOCKET_ERROR)
	{
		printf("listen() failed with error: %d\n", WSAGetLastError());
		exit(0);
	}
	printf("TCP server starting\n");
	//监听端口打开，就开始在这里循环，一有套接字连上，WSAAccept就创建一个套接字，
	//这个套接字和完成端口关联上
	sockaddr_in addrClient;
	int addrClientlen = sizeof(sockaddr_in);
	while (true)
	{
		//等待客户连接
		AcceptSocket = WSAAccept(ServerSocket, (sockaddr*)&addrClient, &addrClientlen, NULL, 0);
		if (AcceptSocket == SOCKET_ERROR)
		{
			printf("WSAAccept() failed with error: %d\n", WSAGetLastError());
			exit(0);
		}
		//分配并设置套接字句柄结构
		PerHandleData = (LPPER_HANDLE_DATA)GlobalAlloc(GPTR, sizeof(PER_HANDLE_DATA));
		if (PerHandleData == NULL)
		{
			printf("GlobalAlloc() failed with error: %d\n", GetLastError());
			exit(0);
		}
		PerHandleData->Socket = AcceptSocket;
		//将与客户进行通信的套接字Accept与完成端口CompletionPort相关联
		if (CreateIoCompletionPort((HANDLE)AcceptSocket, CompletionPort, (DWORD)PerHandleData, 0) == NULL)
		{
			printf("CreateIoCompletionPort failed!\n");
			exit(0);
		}
		//为I/O操作结构分配内存空间
		PerIoData = (LPPER_IO_DATA)GlobalAlloc(GPTR, sizeof(PER_IO_DATA));
		if (PerIoData == NULL)
		{
			printf("GlobalAlloc() failed with error: %d\n", GetLastError());
			exit(0);
		}
		//初始化I/O操作结构
		ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));
		PerIoData->BytesRECV = 0;
		PerIoData->DataBuf.len = DEFAULT_BUFLEN;
		PerIoData->DataBuf.buf = PerIoData->Buffer;
		flags = 0;
		//接收数据，放到PerIoData中，通过工作线程函数取出
		iResult = WSARecv(AcceptSocket, &(PerIoData->DataBuf), 1, &RecvBytes, &flags, &(PerIoData->Overlapped), NULL);
		if (iResult == SOCKET_ERROR)
		{
			if (WSAGetLastError() != ERROR_IO_PENDING)
			{
				printf("WSARecv() failed! \n");
				exit(0);
			}
		}
	}
	return 0;
}
