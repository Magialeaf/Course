#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<stdio.h>
#include<conio.h>
#include<time.h>
#include<WinSock2.h>
#include<packet32.h>

#define DEFAULT_BUF 5000
#define MAX_NUM_ADAPTER 10
int main(int argc, char** argv)
{
	char packetbuf[DEFAULT_BUF];	//发送数据缓冲区
	LPADAPTER lpAdapter = 0;		//指向ADAPTER结构的指针
	LPPACKET lpPacket;				//指向PACKET结构的指针
	int i, npacks, Snaplen;
	DWORD dwErrorCode;
	char AdapterName[8192];			//保存网卡名称
	char* temp, * temp1;
	int AdapterNum = 0, Open;
	ULONG AdapterLength;
	float cpu_time;
	char AdapterList[MAX_NUM_ADAPTER][8192];
	printf("*流量发生器*\n");
	printf("本软件使用Packet.dll API向网络发送一组数据帧\n");
	if (argc == 1)
	{
		printf("[Usage: TrafficGenerator [-i adapter] -n npacks -s size]\n");
		printf("[size is between 60 and 1514.]\n");
		exit(0);
	}
	AdapterName[0] = 0;
	//解析命令行参数
	for (int i = 1; i < argc; i += 2)
	{
		switch (argv[i][1])
		{
		case 'i':
			sscanf(argv[i + 1], "%s", AdapterName);
			break;
		case 'n':
			sscanf(argv[i + 1], "%d", &npacks);
			break;
		case 's':
			sscanf(argv[i + 1], "%d", &Snaplen);
			break;
		}
	}
	printf("npacks:%d Snaplen:%d\n", npacks, Snaplen);
	if (AdapterName[0] == 0)
	{
		/* ****************************************************************************** */
		/*获取本地机器设备列表代码*/
		printf("[Adapters installed]:\n");
		i = 0;
		AdapterLength = sizeof(AdapterName);
		if (PacketGetAdapterNames((PTSTR)AdapterName, &AdapterLength) == FALSE)
		{
			printf("[Unable to get network card list.]\n");
			exit(0);
		}
		temp = AdapterName;
		temp1 = AdapterName;
		while ((*temp != '\0') || (*(temp - 1) != '\0'))
		{
			if (*temp == '\0')
			{
				memcpy(AdapterList[i], temp1, temp - temp1);
				AdapterList[i][temp - temp1] = '\0';
				temp1 = temp + 1;
				i++;
			}
			temp++;
		}
		/* ****************************************************************************** */
		/*打开指定的网卡代码*/
		AdapterNum = i;
		for (i = 0; i < AdapterNum; i++)
			printf("[%d- %s]\n", i + 1, AdapterList[i]);
		printf("\n");
		do
		{
			printf("[Please select the network card number to open.]\ninput:");
			scanf("%d", &Open);
			if (Open > AdapterNum)
				printf("[Number must be less than %d]\n", AdapterNum);
		} while (Open > AdapterNum);
		lpAdapter = PacketOpenAdapter(AdapterList[Open - 1]);
		if (!lpAdapter || (lpAdapter->hFile == INVALID_HANDLE_VALUE))
		{
			dwErrorCode = GetLastError();
			printf("[Unable to open network card, error code: %lx]\n");
			exit(0);
		}
	}
	else
	{
		lpAdapter = PacketOpenAdapter(AdapterName);
		if (!lpAdapter || (lpAdapter->hFile == INVALID_HANDLE_VALUE))
		{
			dwErrorCode = GetLastError();
			printf("[Unable to open network card, error code: %lx]\n");
			exit(0);
		}
	}
	/* ****************************************************************************** */
	/*填充并初始化_PACKET对象*/
	if ((lpPacket = PacketAllocatePacket()) == NULL)
	{
		printf("[Error: unable to allocate LPPACKET structure.]\n");
		exit(0);
	}
	//填充数据内容
	for (int j = 0; j < 6; j++)
		packetbuf[j] = 1;
	for (int j = 6; j < 12; j++)
		packetbuf[j] = 2;
	for (int j = 12; j < 1514; j++)
		packetbuf[j] = (char)j;
	PacketInitPacket(lpPacket, packetbuf, Snaplen);
	/* ****************************************************************************** */
	/*发送数据*/
	//设置发送次数
	if (PacketSetNumWrites(lpAdapter, npacks) == FALSE)
		printf("[You cannot send multiple messages on a single write operation!]\n");
	printf("\n[Generate %d data frame...]\n", npacks);
	cpu_time = (float)clock();
	if (PacketSendPacket(lpAdapter, lpPacket, TRUE) == FALSE)
	{
		printf("[Error in sending data frame.]\n");
		exit(0);
	}
	//计算总耗时
	cpu_time = (clock() - cpu_time) / CLK_TCK;
	printf("[总共耗时: %5.3f]\n",cpu_time);
	printf("[总共发送数据帧数 = %d]\n",npacks);
	printf("[总共产生的字节数 = %d]\n", (Snaplen + 24)* npacks);
	printf("[总共产生的比特数 = %d]\n", (Snaplen + 24)* npacks * 8);
	printf("[每秒平均帧数 = %d]\n", (unsigned int)((double)npacks / cpu_time));
	printf("[每秒平均字节数 = %d]\n", (unsigned int)((double)(Snaplen + 24)* npacks) / cpu_time);
	printf("[每秒平均比特数 = %d]\n", (unsigned int)((double)(Snaplen + 24)* npacks * 8) / cpu_time);
	printf("\n");
	//释放PACKET结构
	PacketFreePacket(lpPacket);
	//关闭网卡并退出
	PacketCloseAdapter(lpAdapter);
	return 0;
}