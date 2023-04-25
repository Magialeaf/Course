#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32  //指明当前系统
#include<stdio.h>
#include <pcap.h>
#include <WinSock2.h>
#include <time.h>
#include "Package.h"

#define filter "ip and tcp"

//对回调函数packet_handler声明
void packet_handler(u_char* param, const struct pcap_pkthdr* header, const u_char* pkt_data);
//param:在pcap_loop()函数中指定的参数user,header:指向pcap_pkthdr结构的指针,pkt_data:指向接收到的数据帧的内容的指针
void packet_handler(u_char* param, const struct pcap_pkthdr* header, const u_char* pkt_data)
{
	struct tm* ltime;
	char timestr[16];
	u_long dip = inet_addr("172.22.94.184");
	LPIPHDR ip;
	LPTCPHDR tcp;
	u_int ip_len;
	ip = (LPIPHDR)(pkt_data + 14);
	ip_len = (ip->VIHL & 0x0f) * 4;
	tcp = (LPTCPHDR)((u_char*)ip + ip_len);
	u_short sport, dport;
	u_short flags = htons(tcp->flag);

	time_t local_tv_sec;
	if (dip == ip->DestIP)
	{
		if ((flags & 0x02) && (flags & 0x10))
		{
			//将时间戳转化成可识别的格式
			local_tv_sec = header->ts.tv_sec;
			ltime = localtime(&local_tv_sec);
			strftime(timestr, sizeof timestr, "%H:%M:%S", ltime);
			//打印数据帧的时间戳和长度
			printf("[%s.%6d] [len:%d]", timestr, header->ts.tv_sec, header->len);
			//获得IP数据帧首部的位置
			//ip = (LPIPHDR)(pkt_data + 14);
			//获得TCP首部的位置
			//ip_len = (ip->VIHL & 0x0f) * 4;
			//tcp = (LPTCPHDR)((u_char*)ip + ip_len);
			//将网络字节序列转换成主机字节序列
			sport = ntohs(tcp->source);
			dport = ntohs(tcp->dest);
			u_char* Des = (u_char*)&ip->DestIP;
			u_char* Src = (u_char*)&ip->SrcIP;
			u_char* Link = (u_char*)(pkt_data);
			u_char* IP = (u_char*)ip;
			u_char* TCP = (u_char*)tcp;
			//打印IP地址和TCP端口
			printf("[%d.%d.%d.%d:%d -> %d.%d.%d.%d:%d]\n", Src[0], Src[1], Src[2], Src[3], sport, Des[0], Des[1], Des[2], Des[3], dport);
			//数据链路层
			printf("Link:");
			for (int i = 0; i < 14; i++)
			{
				if (Link[i] < 16)
					printf("0%x ", Link[i]);
				else
					printf("%x ", Link[i]);
			}
			//网络层
			printf("\nIP:");
			for (int i = 0; i < ip_len; i++)
			{ 
				if (IP[i] < 16)
					printf("0%x ", IP[i]);
				else
					printf("%x ", IP[i]);
			}
			//传输层
			printf("\nTCP:");
			for (int i = 0; i < 20; i++)
			{
				if (TCP[i] < 16)
					printf("0%x ", TCP[i]);
				else
					printf("%x ", TCP[i]);
			}
			printf("\n\n");
		}
	}
}
int main(int argc, char* argv[])
{
	pcap_if_t* alldevs;
	pcap_if_t* d;
	int inum;
	int i = 0;
	pcap_t* adhandle;
	char errbuf[PCAP_ERRBUF_SIZE];
	u_int netmask;
	char packet_filter[] = filter;
	struct bpf_program fcode;
	/* ****************************************************************************** */
	/*获取本地机器设备列表代码*/
	if (pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		fprintf(stderr, "[pcap_findalldevs error: %s]\n", errbuf);
		exit(0);
	}
	/*打印设备列表*/
	for (d = alldevs; d; d = d->next)
	{
		printf("%d.%s", ++i, d->name);
		if (d->description)
			printf("{%s}\n", d->description);
		else
			printf("{No description!}");
	}
	if (i == 0)
	{
		printf("[Unable to find the network interface! Make sure WinPcap is installed correctly.]\n");
		exit(0);
	}
	/* ****************************************************************************** */
	/*打开指定的网卡代码*/
	printf("[Please enter the network card number of data to be captured.]\n[Input]:");
	scanf("%d", &inum);
	if (inum<1 || inum>i)
	{
		printf("[Adapter number out of range.]\n");
		/*释放设备列表*/
		pcap_freealldevs(alldevs);
		exit(0);
	}
	//跳转到用户选择的网卡
	for (d = alldevs, i = 0; i < inum - 1; d = d->next, i++);
	//打开网卡
	//设备名，保证捕获到每个数据帧的全部内容，混杂模式，读取超时时间，错误缓冲区
	adhandle = pcap_open_live(d->name, 65536, 1, 1000, errbuf);
	if (adhandle == NULL)
	{
		fprintf(stderr, "[Unable to open the network card, WinPcap does not support:%s]\n");
		/*释放设备列表*/
		pcap_freealldevs(alldevs);
		exit(0);
	}
	//检查链接层。本程序只支持以太网
	if (pcap_datalink(adhandle) != DLT_EN10MB)
	{
		fprintf(stderr, "[This program is only available in Ethernet environment.]\n");
		/*释放设备列表*/
		pcap_freealldevs(alldevs);
		exit(0);
	}
	/* ****************************************************************************** */
	/*设置过滤规则代码*/
	if (d->addresses != NULL)
		//获得第一个接口地址的网络掩码
		netmask = ((struct sockaddr_in*)(d->addresses->netmask))->sin_addr.S_un.S_addr;
	else
		//如果没有地址，那么我们假设一个C类的掩码
		netmask = 0xffffff;
    //编译过滤器
	if (pcap_compile(adhandle, &fcode, packet_filter, 1, netmask) < 0)
	{
		fprintf(stderr, "[Unable to compile filtering rules, please check the correctness of grammar.]\n");
		/*释放设备列表*/
		pcap_freealldevs(alldevs);
		exit(0);
	}
	//设置过滤器
	if (pcap_setfilter(adhandle, &fcode) < 0)
	{
		fprintf(stderr, "[Error in setting filter]\n");
		/*释放设备列表*/
		pcap_freealldevs(alldevs);
		exit(0);
	}
	printf("[listening on %s]\n", d->description);
	pcap_freealldevs(alldevs);
	/* ****************************************************************************** */
	/*捕获数据帧*/
	pcap_loop(adhandle, 0, packet_handler, NULL);
	return 0;
}