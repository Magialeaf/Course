#include "../服务器/Protocol.h"

#define Max_No 10     //服务器号码数量
#define WSA_MAXIMUM 5 //最大链接数量
//号码变量
char No[Max_No] = { 0 };       //号码单向循环（0未使用，1-Max_No + 1为使用权值（1号机,2号机,3下一位...））
int Max_Num = 2;               //被取最大号码值
int PWait_Num = 0;             //等待尾指针

//事件属性
WSAEVENT eventArray[WSA_MAXIMUM];
SOCKET sockArray[WSA_MAXIMUM];
char idArray[WSA_MAXIMUM];     //链接程序身份
int Proc[3] = { 0 };           //链接程序数量(0:取号,1:叫号,2:业务(1号机,2号机,3全部))

/****************************************声明函数********************************************/
//计算下一号码指针位置
void Addnext();
//发现目标号码（目标号码）（返回索引值，-1是未找到）
int Find_No(int no);
//降低权重
void Down_No();
//——————————————————————————————————————————————————————————
//发送给叫号机的包(套接字号 操作 机器号 机器状态 号码十位 号码个位)
void Send_Call(int x, int title, int no, int state, char decade, char unit);
//发送给取号机的包(套接字号，操作，号码十位，号码个位)
void Send_Get(int x, int title, char decade, char unit);
//发送给业务机的包(套接字号，操作，号码十位，号码个位)
void Send_Bus(int x, int title, char decade, char unit);
//——————————————————————————————————————————————————————————
//事件模型（套接字）
void Event(SOCKET s);
//身份验证,初始化机器状态
int Identity(SOCKET client, int i, int total);
//验证程序包(套接字,程序ID)
int Send_Iden(SOCKET client, int title);
//通知新开的业务机更新号码(业务号码)
void Update_No(int open,int total);
//——————————————————————————————————————————————————————————
//取号程序响应处理
void GetProc(int i, int total);
//业务程序响应处理
void BusProc(int i, int total);
//多线程选择关闭服务器和查看队列
DWORD WINAPI Close_Server(PVOID p);
/****************************************定义函数********************************************/

//计算下一号码指针位置
void Addnext()
{
	PWait_Num++;
	if (PWait_Num >= Max_No)
		PWait_Num = 0;
}	

//发现目标号码（目标号码）（返回索引值，-1是未找到）
int Find_No(int no)
{
	int res = -1;
	int P = PWait_Num;
	for (int i = 0; i < Max_No; i++)
	{
		if (No[PWait_Num] == no)
		{
			res = PWait_Num;
			break;
		}
		else
			Addnext();
	}
	PWait_Num = P;
	return res;
}

//降低权重
void Down_No()
{
	for (int i = 0; i < Max_No; i++)
	{
		if (No[i] == 3)
			No[i] = 0;
		else if (No[i] > 3)
			No[i]--;
	}
}

//发送给叫号机的包(套接字号 操作 机器号 机器状态 号码十位 号码个位)
void Send_Call(int x,int title,int no,int state, char decade, char unit)
{
	memset(buf, 0, DEFAULT_BUFLEN);
	if (title == Call_Num || title == Del_Num || title == Bus_Open || title == Bus_Close)
		buf[0] = title;
	if (no >= 0 && no <= 3)
		buf[1] = no;
	if (state == 0 || state == 1)
		buf[2] = state;
	buf[3] = decade;
	buf[4] = unit;
	send(sockArray[x], buf, 5, 0);
	if (title == Call_Num)
	{
		if(state == 0)
			printf("[ 发送给叫号机当前号码:A0%c%c ]\n", decade, unit);
		else if(state == 1)
			printf("[ 发送给叫号机下一号码:A0%c%c ]\n", decade, unit);
	}
		
	else if (title == Del_Num)
	{
		if (decade == -1 && unit == -1)
			printf("[ 发送给叫号机下一号码:空闲 ]\n");
		else
			printf("[ 收回叫号机当前号码:A0%c%c ]\n", decade, unit);
	}	
	else if (title == Bus_Open)
		printf("[ 发送给叫号机:业务机已开启 ]\n");
	else if (title == Bus_Close)
		printf("[ 发送给叫号机:业务机已关闭 ]\n");
}

//发送给取号机的包(套接字号，操作，号码十位，号码个位)
void Send_Get(int x, int title, char decade, char unit)
{
	memset(buf, 0, DEFAULT_BUFLEN);
	if (title == Get_Num_Succ || title == Get_Num_Fail || title == Call_Close || title == Bus_Close)
		buf[0] = title;
	buf[1] = decade;
	buf[2] = unit;
	send(sockArray[x], buf, 3, 0);
	if(title == Get_Num_Succ )
		printf("[ 发送给取号机号码:A0%c%c ]\n", decade, unit);
	else if (title == Get_Num_Fail )
		printf("[ 服务器号码已被取完! ]\n");
	else if (title == Call_Close)
		printf("[ 叫号机未开启,无法取号! ]\n");
	else if (title == Bus_Close)
		printf("[ 业务机未开启,无法取号! ]\n");
}

//发送给业务机的包(套接字号，操作，号码十位，号码个位)
void Send_Bus(int x, int title, char decade, char unit)
{
	memset(buf, 0, DEFAULT_BUFLEN);
	if (title == Bus_Now || title ==  Bus_Next)
		buf[0] = title;
	buf[1] = decade;
	buf[2] = unit;
	send(sockArray[x], buf, 5, 0);
	if(title == Bus_Now)
		printf("[ 发送给业务机%d当前号码:A0%c%c ]\n", idArray[x] - Business_Proc,decade, unit);
	else if (title == Bus_Next)
	{
		if(decade == 0 && unit == 0)
			printf("[ 发送给业务机%d下一号码:空闲 ]\n", idArray[x] - Business_Proc);
		else
			printf("[ 发送给业务机%d下一号码:A0%c%c ]\n", idArray[x] - Business_Proc, decade, unit);
	}
		
}

//事件模型（套接字）
void Event(SOCKET s)
{
	int res;
	struct sockaddr_in addr;
	struct sockaddr_in* paddr = &addr;
	int iIndex = 0, i;
	int iEventTotal = 0;

	WSAEVENT Event = WSACreateEvent();
	WSAResetEvent(Event);

	WSAEventSelect(s, Event, FD_ACCEPT);
	eventArray[iEventTotal] = Event;
	sockArray[iEventTotal] = s;
	idArray[iEventTotal] = Get_Message;
	iEventTotal++;

	while (true)
	{
		iIndex = WSAWaitForMultipleEvents(iEventTotal, eventArray, FALSE, WSA_INFINITE, FALSE);
		iIndex = iIndex - WSA_WAIT_EVENT_0;
		i = iIndex;
		WSANETWORKEVENTS newevent;
		WSAEnumNetworkEvents(sockArray[i], eventArray[i], &newevent);
		if (newevent.lNetworkEvents & FD_ACCEPT) //处理FD_ACCEPT通知消息
		{
			if (newevent.iErrorCode[FD_ACCEPT_BIT] == 0)
			{
				if (iEventTotal > WSA_MAXIMUM)
				{
					printf("[ 连接过多! ] \n");
					continue;
				}
				else
				{
					SOCKET Client_s = My_Accept(s, paddr);
					res = Identity(Client_s, i, iEventTotal);
					if (res > 0)
					{
						WSAEVENT NewEvent = WSACreateEvent();
						WSAEventSelect(Client_s, NewEvent, FD_READ | FD_CLOSE);  // 不用 FD_WRITES事件
						eventArray[iEventTotal] = NewEvent;
						sockArray[iEventTotal] = Client_s;
						idArray[iEventTotal] = res;
						iEventTotal++;
						if (Proc[2] > 0)
							Update_No(res,iEventTotal);
					}
				}
			}
		}
		if (newevent.lNetworkEvents & FD_READ) //处理FD_READ通知消息
		{
			if (newevent.iErrorCode[FD_READ_BIT] == 0)
			{
				memset(buf, 0, DEFAULT_BUFLEN);
				res = recv(sockArray[i], buf, DEFAULT_BUFLEN, 0);
				if (res > 0)
				{
					//确认消息不做处理
					if (buf[0] == Get_Message);
					//取号程序发来消息
					else if (idArray[i] == Get_Proc)
						GetProc(i, iEventTotal);
					//业务程序发来消息
					else if (idArray[i] == (Business_Proc + 1) || idArray[i] == (Business_Proc + 2))
						BusProc(i, iEventTotal);
				}
			}
		}
		if (newevent.lNetworkEvents & FD_CLOSE) //处理FD_CLOSE通知消息
		{
			char tip[5];
			char tip_one[] = "意外";
			char tip_two[] = "正常";
			if (newevent.iErrorCode[FD_CLOSE_BIT] == 0)
				strcpy(tip, tip_two);
			else
				strcpy(tip, tip_one);
			closesocket(sockArray[i]);
			WSACloseEvent(eventArray[i]);
			if (idArray[i] == Get_Proc)
			{
				printf("[ 取号机%s关闭! ]\n",tip);
				Proc[0]--;
			}
			else if (idArray[i] == Call_Proc)
			{
				printf("[ 叫号机%s关闭! ]\n",tip);
				Proc[1]--;
			}
			else if (idArray[i] == Business_Proc + 1 || idArray[i] == Business_Proc + 2)
			{
				int bus = idArray[i] - Business_Proc;
				int cur = Find_No(idArray[i] - Business_Proc);
				printf("[ 业务机%d%s关闭! ]\n", bus, tip);
				if (cur != -1)
					No[cur] = 0;
				Proc[2] -= bus;
				for (int x = 0; x < WSA_MAXIMUM; x++)
					if (idArray[x] == Call_Proc)
						Send_Call(x, Bus_Close, bus, 0, 0, 0);
			}
			for (int j = i; j < iEventTotal - 1; j++)
			{
				sockArray[j] = sockArray[j + 1];
				eventArray[j] = eventArray[j + 1];
				idArray[j] = idArray[j + 1];
			}
			iEventTotal--;
			printf("[ 相关信息已清空! ]\n");
		}
	}
}

//身份验证,初始化机器状态
int Identity(SOCKET client,int i, int total)
{
	int res;
	memset(buf, 0, DEFAULT_BUFLEN);
	int r = recv(client, buf, 1, 0);
	if (r > 0)
	{
		res = Send_Iden(client, buf[0]);
		if (res == Call_Proc)
		{
			if (Proc[2] != 0)
				Send_Call(i, Bus_Open, Proc[2], 0, 0, 0);
		}
		else if (res == Business_Proc + 1 || res == Business_Proc + 2)
		{
			if (Proc[2] == 1 || Proc[2] == 2)
			{
				//普通通知叫号程序打开
				for (int x = 0; x < total; x++)
					if (idArray[x] == Call_Proc)
						Send_Call(x, Bus_Open, Proc[2], 0, 0, 0);
			}
			else if (Proc[2] == 3)
			{
				//后开的进行计算号码
				int open = res - Business_Proc;
				//通知叫号程序打开
				for (int x = 0; x < total; x++)
					if (idArray[x] == Call_Proc)
					{
						Send_Call(x, Bus_Open, open, 0, 0, 0);
						break;
					}
			}
		}
	}
	else
	{
		printf(" [ 错误信息! ]\n");
		res = 0;
	}
	return res;
}

//验证程序包(套接字,程序ID)
int Send_Iden(SOCKET client, int title)
{
	int res, r, can = 0,test = 0;
	char tip[5];
	char tip_get[] = "取号";
	char tip_call[] = "叫号";
	char tip_bus[] = "业务";
	memset(buf, 0, DEFAULT_BUFLEN);
	if (title == Get_Proc)
	{
		strcpy(tip, tip_get);
		if (Proc[0] == 0)
			can = 1;
		else
			can = 0;
	}
	else if (title == Call_Proc)
	{
		strcpy(tip, tip_call);
		if (Proc[1] == 0)
			can = 1;
		else
			can = 0;
	}
	else if (title == Business_Proc)
	{
		strcpy(tip, tip_bus);
		if (Proc[2] < 3)
		{
			if (Proc[2] == 0)
				test = '1';
			else
				test = '3' - Proc[2];
			can = 1;
		}
		else
			can = 0;
	}
	if (can == 1)
	{
		printf("[ %s程序进行验证... ]\n", tip);
		buf[0] = Get_Message;
		buf[1] = title;
		buf[2] = test;
		send(client, buf, 3, 0);
		memset(buf, 0, DEFAULT_BUFLEN);
		Sleep(100);
		r = recv(client, buf, 1, 0);
		if (r > 0)
		{
			if (buf[0] == Get_Message)
			{
				printf("[ %s程序验证成功! ]\n", tip);
				res = title;
				if (title == Business_Proc)
				{
					if (Proc[2] == 0)
					{
						printf("[ 业务程序为1号! ]\n");
						Proc[2]++;
						res = title + 1;
					}
					else
					{
						printf("[ 业务程序为%d号! ]\n", 3 - Proc[2]);
						res = title + 3 - Proc[2];
						Proc[2] = 3;		
					}
				}
				else
					Proc[title / 10 - 1] ++;
			}
			else
			{
				printf("[ 其他错误! ]\n");
				res = 0;
			}
		}
		else
		{
			printf("[ 接受错误! ]\n");
			res = 0;
		}
	}
	else
	{
		printf("[ %s程序连接已达到上限 ]\n", tip);
		memset(buf, 0, DEFAULT_BUFLEN);
		buf[0] = Get_Message;
		send(client, buf, 1, 0);
		res = 0;
	}
	return res;
}
//通知新开的业务机更新号码(新开业务机存的业务号,总数)
void Update_No(int open,int total)
{
	int cur = Find_No(3);
	//存在下一号码
	if (cur != -1)
	{
		char No_char[2] = { 0 };
		No_char[0] = (cur + 1) / 10 + 48;
		No_char[1] = (cur + 1) % 10 + 48;
		for (int x = 0; x < total; x++)
		{
			if (idArray[x] == Call_Proc)
			{
				//叫号机更新当前号码
				if (open == Business_Proc + 1)
					Send_Call(x, Call_Num, 0, 0, No_char[0], No_char[1]);
				if (open == Business_Proc + 2)
					Send_Call(x, Call_Num, 2, 0, No_char[0], No_char[1]);
			}
			else if (idArray[x] == open)
			{
				//业务机更新当前号码
				Send_Bus(x, Bus_Now, No_char[0], No_char[1]);
			}
		}
		No[cur] = open - Business_Proc;
		Max_Num--;
		Down_No();
		if (Max_Num >= 3)
		{
			cur = Find_No(3);
			No_char[0] = (cur + 1) / 10 + 48;
			No_char[1] = (cur + 1) % 10 + 48;
			for (int x = 0; x < total; x++)
			{
				if (idArray[x] == Call_Proc)
				{
					//叫号机更新下一号码
					if (Proc[2] == 1 || Proc[2] == 3)
						Send_Call(x, Call_Num, 0, 1, No_char[0], No_char[1]);
					if (Proc[2] == 2 || Proc[2] == 3)
						Send_Call(x, Call_Num, 2, 1, No_char[0], No_char[1]);
				}
				else if (idArray[x] == Business_Proc + 1 || idArray[x] == Business_Proc + 2)
				{
					//业务机更新下一号码
					Send_Bus(x, Bus_Next, No_char[0], No_char[1]);
				}
			}
		}
		else
		{
			for (int x = 0; x < total; x++)
			{
				if (idArray[x] == Call_Proc)
				{
					//叫号机取消下一叫号
					if (Proc[2] == 1 || Proc[2] == 3)
						Send_Call(x, Del_Num, 0, 1, -1, -1);
					if (Proc[2] == 2 || Proc[2] == 3)
						Send_Call(x, Del_Num, 2, 1, -1, -1);
				}
				else if (idArray[x] == Business_Proc + 1 || idArray[x] == Business_Proc + 2)
				{
					//业务机取消下一号码
					Send_Bus(x, Bus_Next, 0, 0);
				}
			}
		}
	}
}

//取号程序响应处理
void GetProc(int i,int total)
{
	int bus[2];
	char No_char[2] = { 0 };
	//判断其他程序是否处于打开状态
	if (Proc[1] == 0)
		Send_Get(i, Call_Close, 0, 0);
	else if (Proc[2] == 0)
		Send_Get(i, Bus_Close, 0, 0);
	else
	{
		bus[0] = Find_No(1);
		bus[1] = Find_No(2);
		//判断机器是否是空闲状态
		if ((bus[0] == -1 && (Proc[2] == 1 || Proc[2] == 3)) || (bus[1] == -1 && (Proc[2] == 2 || Proc[2] == 3)))
		{
			int use;
			if (bus[0] == -1)
				use = 1;
			else if (bus[1] == -1)
				use = 2;
			No_char[0] = (PWait_Num + 1) / 10 + 48;
			No_char[1] = (PWait_Num + 1) % 10 + 48;
			for (int x = 0; x < total; x++)
			{
				if (idArray[x] == Business_Proc + use)
					Send_Bus(x, Bus_Now, No_char[0], No_char[1]);
				else if (idArray[x] == Call_Proc)
				{
					if (use == 1)
						Send_Call(x, Call_Num, 0, 0, No_char[0], No_char[1]);
					else if (use == 2)
						Send_Call(x, Call_Num, 2, 0, No_char[0], No_char[1]);
				}
			}
			Send_Get(i, Get_Num_Succ, No_char[0], No_char[1]);
			printf("\n");
			No[PWait_Num] = use;
			Addnext();
		}
		//判断机器是否都已经存在号码
		else if (bus[0] != -1 && Proc[2] == 1 || bus[1] != -1 && Proc[2] == 2 || bus[0] != -1 && bus[1] != -1)
		{
			//是否存在下一号码
			int wait = Find_No(3);
			//不存在发送
			if (wait == -1)
			{
				No_char[0] = (PWait_Num + 1) / 10 + 48;
				No_char[1] = (PWait_Num + 1) % 10 + 48;
				for (int x = 0; x < total; x++)
				{
					if (idArray[x] == Business_Proc + 1 || idArray[x] == Business_Proc + 2)
						Send_Bus(x, Bus_Next, No_char[0], No_char[1]);
					else if (idArray[x] == Call_Proc)
					{
						if(Proc[2] == 1 || Proc[2] == 3)
							Send_Call(x, Call_Num, 0, 1, No_char[0], No_char[1]);
						if(Proc[2] == 2 || Proc[2] == 3)
							Send_Call(x, Call_Num, 2, 1, No_char[0], No_char[1]);
					}
				}
				Send_Get(i, Get_Num_Succ, No_char[0], No_char[1]);
				printf("\n");
				Max_Num++;
				No[PWait_Num] = Max_Num;
				Addnext();
			}
			else
			{
				int next = Find_No(0);
				if (next == -1)
				{
					//发回给取号程序取号失败
					Send_Get(i, Get_Num_Fail, 0, 0);
				}
				else
				{
					int cur = Find_No(0);
					//取号存在缓冲区
					No_char[0] = (cur + 1) / 10 + 48;
					No_char[1] = (cur + 1) % 10 + 48;
					Send_Get(i, Get_Num_Succ, No_char[0], No_char[1]);
					printf("\n");
					Max_Num++;
					No[cur] = Max_Num;
					PWait_Num = cur;
					Addnext();
				}		
			}
		}
	}
}

//业务程序响应处理
void BusProc(int i, int total)
{
	//if (buf[0] == Regist)
	//{

	//}
	//else if (buf[0] == Login)
	//{

	//}
	//else if (buf[0] == Put_Money)
	//{

	//}
	//else if (buf[0] == Get_Money)
	//{

	//}
	//else if (buf[0] == Bus_Other)
	//{

	//}
	if (buf[0] == Bus_End)
	{
		int bus = idArray[i] - Business_Proc;
		int no = Find_No(bus);
		char No_char[2];
		No_char[0] = (no + 1) / 10 + 48;
		No_char[1] = (no + 1) % 10 + 48;
		printf("[ 业务机%d:A0%c%c完成了当前业务 ] \n", bus, No_char[0], No_char[1]);
		No[no] = 0;
		//发送给叫号机更新当前叫号码
		for (int x = 0; x < total; x++)
		{
			//叫号机更新当前号码
			if (idArray[x] == Call_Proc)
			{
				if (bus == 1)
					Send_Call(x, Del_Num, 0, 0, No_char[0], No_char[1]);
				else if(bus == 2)
					Send_Call(x, Del_Num, 2, 0, No_char[0], No_char[1]);
				break;
			}
		}
		//存在等待号码
		if (Max_Num >= 3)
		{
			int cur = Find_No(3);
			No[cur] = bus;
			No_char[0] = (cur + 1) / 10 + 48;
			No_char[1] = (cur + 1) % 10 + 48;
			for (int x = 0; x < total; x++)
			{
				//叫号机更新当前号码
				if (idArray[x] == Call_Proc)
				{
					if(bus == 1)
						Send_Call(x, Call_Num, 0, 0, No_char[0], No_char[1]);
					else if (bus == 2)
						Send_Call(x, Call_Num, 2, 0, No_char[0], No_char[1]);
					break;
				}
			}
			Max_Num--;
			Down_No();
			//判断是否还有更多等待号码
			if (Max_Num >= 3)
			{
				int cur = Find_No(3);
				No_char[0] = (cur + 1) / 10 + 48;
				No_char[1] = (cur + 1) % 10 + 48;
				for (int x = 0; x < total; x++)
				{
					if (idArray[x] == Call_Proc)
					{
						//叫号机更新下一号码
						if(Proc[2] == 1 || Proc[2] == 3)
							Send_Call(x, Call_Num, 0, 1, No_char[0], No_char[1]);
						if (Proc[2] == 2 || Proc[2] == 3)
							Send_Call(x, Call_Num, 2, 1, No_char[0], No_char[1]);
					}
					else if (idArray[x] == Business_Proc + 1 || idArray[x] == Business_Proc + 2)
					{
						//业务机更新下一号码
						Send_Bus(x, Bus_Next, No_char[0], No_char[1]);
					}
				}
			}
			else
			{
				for (int x = 0; x < total; x++)
				{
					if (idArray[x] == Call_Proc)
					{
						//叫号机取消下一叫号
						if (Proc[2] == 1 || Proc[2] == 3)
							Send_Call(x, Del_Num, 0, 1, -1, -1);
						if (Proc[2] == 2 || Proc[2] == 3)
							Send_Call(x, Del_Num, 2, 1, -1, -1);
					}
					else if (idArray[x] == Business_Proc + 1 || idArray[x] == Business_Proc + 2)
					{
						//业务机取消下一号码
						Send_Bus(x, Bus_Next, 0, 0);
					}
				}
			}
		}
		printf("\n");
	}
}

//多线程选择关闭服务器和查看队列
DWORD WINAPI Close_Server(PVOID p)
{
	char input;
	while (true)
	{
		if (_kbhit())
		{
			input = _getch();
			if (input == 13)
			{
				for (int i = 0; i < WSA_MAXIMUM; i++)
					closesocket(sockArray[i]);
				WSACleanup();
				system("cls");
				printf("[ 服务器关闭! ]\n");
				system("pause");
				exit(0);
			}
			else if (input == 32)
			{
				int cur = 3;
				printf("\n[ 使用队列:");
				for (int i = 0; i < Max_No; i++)
				{
					if (No[i] == 1)
						printf(" 1:A0%d%d ", (i + 1) / 10, (i + 1) % 10);
					else if(No[i] == 2)
						printf(" 2:A0%d%d ", (i + 1) / 10, (i + 1) % 10);
				}
				printf("]\n");
				printf("[ 等待队列:");
				for (int i = 0; i < Max_No; i++)
					for (int j = 0; j < Max_No; j++)
						if(No[j] == cur)
						{
							printf(" A0%d%d ->", (j + 1) / 10, (j + 1) % 10);
							cur++;
							break;
						}
				printf("]\n");
			}
		}
	}
	return 0;
}

//服务器
int main(int argc, char** argv)
{
	struct sockaddr_in addr;
	struct sockaddr_in* paddr = &addr;
	InitSock();
	SOCKET s = CreateTCPSocket();
	addrinfo* result = My_GetAddr(s);
	My_Bind(s, result);
	My_Listen(s);
	printf("[ 银行排号系统服务器 ]（回车:关闭服务器 | 空格：查看号码队列）\n");
	CreateThread(0, 0, Close_Server, 0, 0, 0);
	Event(s);

	closesocket(s);
	WSACleanup();
	return 0;
}


/*
设计思路:
*/

//（1）系统有三个部分组成，分别包括：一个服务器，一个取号程序，一个叫号程序，两个业务程序。
//（2）服务器用指定的模型，其它部分可以采用任意模型
//（3）服务器功能
//（a）取号程序取号，要保证号码的唯一性。（设置单向循环队列（数组记录号码使用状态）0未被使用 1被使用 && 单向队列(数组记录下一号码))
//（b）业务程序取号
//（c）通知叫号程序叫号（即显示当前正在办业务号）
/*
服务器：
取号程序-->服务器：取号了。
服务器-->取号程序：取号结果。
服务器-->取号程序-->叫号程序：取了一个号，可否使用。
叫号程序-->服务器：可以，更新状态。不可以，等待更新。
叫号程序-->服务器-->业务程序：可以，执行业务。
叫号程序-->服务器-->业务程序：不可以，更新下一业务信息。
业务程序-->服务器：已经有下一业务信息，不更新。
业务程序-->服务器：业务结束，自动下一号码，取下下一个号码。
业务程序-->服务器-->叫号程序：下一号码工作了。
*/
/*
大致步骤：启动服务器，取号程序，叫号程序，业务程序。
①取号程序 取号 --(不用等待)-->叫号程序提示当前办理业务的号码 && 业务程序进行业务 || --(等待)-->业务机(下一号码) if 有下一号码则不发  
（设置一个BUF存工作/等待号码(上限取20个)）
②业务程序 办理业务 退出办理
*/