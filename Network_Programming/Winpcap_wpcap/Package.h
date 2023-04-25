#pragma once
#include <WinSock2.h>

#pragma pack(push,1)
typedef struct
{
	//u_int8_t ether_dhost[6];
	char ether_dhost[6];
	/* 目的以太网地址 */
	//u_int8_t ether_shost[6];
	char ether_shost[6];
	/* 源以太网地址 */
	//u_int16_t ether_type;
	u_short ether_type;
	/* 以太网类型 */
}EthHeader, * LPEthHeader;

typedef struct _IPHeader {
	u_char VIHL; //版本和首部长度 各占4bit
	u_char ToS; //服务类型
	u_short TotalLen; //总长度
	u_short ID; //标识号
	u_short Frag_Flags; //片偏移量
	u_char TTL; //生存时间
	u_char Protocol; //协议
	u_short Checksum; //首部校验和
	ULONG SrcIP;
	ULONG DestIP;

}IPHDR, * LPIPHDR;

typedef struct {
	unsigned short	source;
	unsigned short	dest;
	unsigned long seq;
	unsigned long	ack_seq;
	unsigned short flag;
	/*struct {

	unsigned short	doff:4;
	unsigned short	res:6;
	unsigned short	urg:1;
	unsigned short	ack:1;
	unsigned short	psh:1;
	unsigned short	rst:1;
	unsigned short	syn:1;
	unsigned short	fin:1;
		 } ts;*/
	unsigned short	window;
	unsigned short	check;
	unsigned short	urg_ptr;
} TCPHDR, * LPTCPHDR;

struct pseudo_header { //Needed for checksum calculation
	unsigned int source_address;
	unsigned int dest_address;
	unsigned char placeholder;
	unsigned char protocol;
	unsigned short tcp_length;
	TCPHDR tcp;
};

typedef struct {
	IPHDR ip;
	TCPHDR tcp;
}PackHead, * LPPackHead;

#pragma pack(pop)