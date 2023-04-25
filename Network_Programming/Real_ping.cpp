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
#define REQ_DATASIZE 64

#pragma pack(1)

#define DEFAULT_PORT "0"
#define ICMP_ECHOREPLY	0
#define ICMP_ECHOREQ	8

// IP Header -- RFC 791
typedef struct tagIPHDR
{
	u_char  VIHL;			// Version and IHL
	u_char	TOS;			// Type Of Service
	short	TotLen;			// Total Length
	short	ID;				// Identification
	short	FlagOff;		// Flags and Fragment Offset
	u_char	TTL;			// Time To Live
	u_char	Protocol;		// Protocol
	u_short	Checksum;		// Checksum
	struct	in_addr iaSrc;	// Internet Address - Source
	struct	in_addr iaDst;	// Internet Address - Destination
}IPHDR, * PIPHDR;


// ICMP Header - RFC 792
typedef struct tagICMPHDR
{
	u_char	Type;			// Type
	u_char	Code;			// Code
	u_short	Checksum;		// Checksum
	u_short	ID;				// Identification
	u_short	Seq;			// Sequence

}ICMPHDR, * PICMPHDR;

// ICMP Echo Request
typedef struct tagECHOREQUEST
{
	ICMPHDR icmpHdr;
	DWORD	dwTime;
	char	cData[REQ_DATASIZE];
}ECHOREQUEST, * PECHOREQUEST;


// ICMP Echo Reply
typedef struct tagECHOREPLY
{
	IPHDR	ipHdr;
	ECHOREQUEST	echoRequest;
	char    cFiller[256];
}ECHOREPLY, * PECHOREPLY;

#pragma pack()

u_short in_cksum(u_short* pchBuffer, int iSize)
{
	u_long ulCksum = 0;
	while (iSize > 1) {
		ulCksum += *pchBuffer++;
		iSize -= sizeof(u_short);
	}
	if (iSize)
		ulCksum += *(UCHAR*)pchBuffer;
	ulCksum = (ulCksum >> 16) + (ulCksum & 0xffff);
	ulCksum += (ulCksum >> 16);
	return (USHORT)(~ulCksum);
}

int SendEchoRequest(SOCKET s, LPSOCKADDR_IN lpstToaddr)  //构造并发送ICMP ECHO请求数据包
{
	static ECHOREQUEST echoReq;
	static int nId = 1;
	static int nSeq = 1;
	int nRet;
	//填充ICMP ECHO请求
	echoReq.icmpHdr.Type = 8;
	echoReq.icmpHdr.Code = 0;
	echoReq.icmpHdr.Checksum = 0;
	echoReq.icmpHdr.ID = GetCurrentProcessId();
	echoReq.icmpHdr.Seq = nSeq++;
	//填充ICMP EHCO数据
	for (nRet = 0; nRet < REQ_DATASIZE; nRet++)
		echoReq.cData[nRet] = ' ' + nRet;
	//获得当前的时钟并填充
	echoReq.dwTime = GetTickCount64();
	//计算校验和
	echoReq.icmpHdr.Checksum = in_cksum((u_short*)&echoReq, sizeof(ECHOREQUEST));
	//Send the echo request
	//套接字，缓冲区，缓冲区长度，发送标志，目的地址，地址长度
	nRet = sendto(s, (LPSTR)&echoReq, sizeof(ECHOREQUEST), 0, (LPSOCKADDR)lpstToaddr, sizeof(SOCKADDR_IN));
	if (nRet == SOCKET_ERROR)
		printf("sendto()");  //reporterror
	return (nRet);
}

int WaitForEchoReply(SOCKET s)  //等待ICMP ECHO应答
{
	struct timeval Timeout;
	fd_set readfds;
	//设置读等待套接字组
	readfds.fd_count = 1;
	readfds.fd_array[0] = s;
	Timeout.tv_sec = 5;
	Timeout.tv_usec = 0;
	//等待套接字上的网络事件
	return(select(1, &readfds, NULL, NULL, &Timeout));
}
DWORD RecvEchoReply(SOCKET s, LPSOCKADDR_IN lpsaFrom, u_char* pTTL)  //接收ICMP ECHO应答
{
	ECHOREPLY echoReply;
	int nRet;
	int nAddrLen = sizeof(struct sockaddr_in);
	//接收回射应答
	//套接字，接收缓冲区，缓冲区长度，发送标志，数据来源地址，来源地址长度
	nRet = recvfrom(s, (LPSTR)&echoReply, sizeof(ECHOREPLY), 0, (LPSOCKADDR)lpsaFrom, &nAddrLen);
	//判断接收返回至
	if (nRet == SOCKET_ERROR)
		printf("recvfrom()");  //reporterror
	//返回发送时间和IP头中的TTL
	*pTTL = echoReply.ipHdr.TTL;
	return(echoReply.echoRequest.dwTime);
}

//自动获取地址
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
void Ping(LPCSTR pstrHost)  //Ping主体功能
{
	//定义变量
	SOCKET rawSocket;
	LPHOSTENT lpHost;
	struct sockaddr_in saDest;
	struct sockaddr_in saSrc;
	DWORD dwTimeSent;
	DWORD dwElapsed;
	u_char cTTL;
	int nLoop;
	int nRet;
	//创建原始套接字，指定协议类型为IPPROTO_ICMP
	rawSocket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (rawSocket == SOCKET_ERROR)
	{
		printf("socket()");//reporterror
		return;
	}
	//根据用户指定的地址获得目标IP
	lpHost = gethostbyname(pstrHost);
	if (lpHost == NULL)
	{
		fprintf(stderr, "\nHost not found: %s\n", pstrHost);
		return;
	}
	//填充套接字的目的端点地址
	saDest.sin_addr.S_un.S_addr = *((u_long far*) (lpHost->h_addr));
	saDest.sin_family = AF_INET;
	saDest.sin_port = 0;
	//手动填充地址
	//saSrc.sin_addr.S_un.S_addr = inet_addr("172.22.51.73");
	//saSrc.sin_family = AF_INET;
	//saSrc.sin_port = 0;
	//自动获取地址
	addrinfo* hint = GetAddr(rawSocket);
	saSrc.sin_addr.S_un.S_addr = (ULONG)hint->ai_addr;
	saSrc.sin_family = hint->ai_family;
	saSrc.sin_port = 0;
	printf("\nPinging %s [%s] with %d bytes of data:\n", pstrHost, inet_ntoa(saDest.sin_addr), REQ_DATASIZE);//question
	//对目标地址连续ping 4次
	for (nLoop = 0; nLoop < 4; nLoop++)
	{
		//发送ICMP请求
		SendEchoRequest(rawSocket, &saDest);
		//等待响应到达
		nRet = WaitForEchoReply(rawSocket);
		if (nRet == SOCKET_ERROR)
		{
			printf("Select()");//reporterror
			break;
		}
		if (!nRet)
		{
			printf("\nTimeOut");
			break;
		}
		//接收响应
		dwTimeSent = RecvEchoReply(rawSocket, &saSrc, &cTTL);
		//计算响应间隔时间
		dwElapsed = GetTickCount() - dwTimeSent;
		//输出结果
		printf("\nReply from: %s: bytes=%d time=%ldms TTL=%d", inet_ntoa(saSrc.sin_addr), REQ_DATASIZE, dwElapsed, cTTL);
	}
	printf("\n");
	//关闭套接字
	nRet = closesocket(rawSocket);
	if (nRet == SOCKET_ERROR)
		printf("closesocket()");//reporterror
}

int main(int argc, char** argv)
{
	WSADATA wsaData;
	if (argc != 2)
	{
		fprintf(stderr, "\nUsage: ping hostname\n");
		return 1;
	}
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}
	char name[100] = { 0 };
	gethostname(name, 100);
	printf("本机名%s\n", name);
	//调用ping()
	Ping(argv[1]);
	//释放WinSock
	WSACleanup();
}

