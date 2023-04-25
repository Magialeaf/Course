#include "../服务器/Protocol.h"

char No;
char num[2][5] = { {'A','0',0,0},{'A','0',0,0} };
char state = 0;

/****************************************声明函数********************************************/
//向服务器发出验证
void login(SOCKET s);
//多线程监听服务器信息
DWORD WINAPI Next_Num(PVOID p);
//显示界面
void show();
//展示界面和处理业务
void Homework(SOCKET s);
/****************************************定义函数********************************************/

//向服务器发出验证
void login(SOCKET s)
{
	int r;
	buf[0] = Business_Proc;   //验证自己是业务程序
	send(s, buf, 1, 0);
	r = recv(s, buf, 3, 0);
	No = buf[2];
	if (r > 0 && buf[0] == Get_Message && buf[1] == Business_Proc)
	{
		memset(buf, 0, DEFAULT_BUFLEN);
		buf[0] = Get_Message;
		send(s, buf, 1, 0);
		printf("[ 银行业务程序启动成功!本机为业务机%c ]\n",No);
		Sleep(1500);
	}
	else
	{
		printf("[ 银行业务程序启动失败! ]\n");
		closesocket(s);
		WSACleanup();
		system("pause");
		exit(0);
	}
}

//多线程监听服务器信息
DWORD WINAPI Next_Num(PVOID p)
{
	SOCKET c = (SOCKET)p;
	int r;
	while (true)
	{
		memset(buf, 0, DEFAULT_BUFLEN);
		r = recv(c, buf, 5, 0);
		if (r > 0)
		{
			if (buf[0] == Get_Message);
			else if (buf[0] == Bus_Now)
			{
				num[0][2] = buf[1];
				num[0][3] = buf[2];
			}
			else if (buf[0] == Bus_Next)
			{
				num[1][2] = buf[1];
				num[1][3] = buf[2];
				show();
			}
		}
		else if (r == 0)
		{
			system("cls");
			printf("[ 服务器已关闭! ]\n");
			closesocket(c);
			WSACleanup();
			system("pause");
			exit(0);
		}
		else
		{
			system("cls");
			if (WSAGetLastError() == 10054)
				printf(" [ 服务器意外关闭! ]\n");
			else
				printf("[ 接收错误! ]: %d\n", WSAGetLastError());
			closesocket(c);
			WSACleanup();
			system("pause");
			exit(0);
		}
	}
	return 0;
}

//显示界面
void show()
{
	char free[] = "空闲";
	char res[5];
	if (num[1][2] == 0 && num[1][3] == 0)
		strcpy(res, free);
	else
		strcpy(res, num[1]);
	if (state == 0)
	{
		system("cls");
		printf("[ 银行业务程序:%c号 ]\n", No);
		printf("-----------------------\n");
		printf("|                     |\n");
		printf("| 当前机器状态:空闲   |\n");
		printf("|                     |\n");
		printf("-----------------------\n");
	}
	else if (state == 10)
	{
		system("cls");
		printf("[ 银行业务程序:%c号 ]\n", No);
		printf("[ 当前用户号码:%s  |  下一用户号码:%s ]\n", num[0], res);
		printf("-----------------------------\n");
		printf("|  请选择注册或登入账户     |\n");
		printf("|  1.注册                   |\n");
		printf("|  2.登入                   |\n");
		printf("|  0.退出                   |\n");
		printf("-----------------------------\n");
		printf("输入:");
	}
	else if (state == 11)
	{
		system("cls");
		printf("[ 银行业务程序:%c号 ]\n", No);
		printf("[ 当前用户号码:%s  |  下一用户号码:%s ]\n", num[0], res);
		printf("------------------------------------\n");
		printf("|  请输入注册相关信息             |\n");
		printf("------------------------------------\n");
		printf("输入身份证号:");
	}
	else if (state == 12)
	{
		system("cls");
		printf("[ 银行业务程序:%c号 ]\n", No);
		printf("[ 当前用户号码:%s  |  下一用户号码:%s ]\n", num[0], res);
		printf("------------------------------------\n");
		printf("|  请输入卡号和密码               |\n");
		printf("------------------------------------\n");
		printf("输入卡号:");
	}
	else if (state == 13)
	{
		system("cls");
		printf("[ 银行业务程序:%c号 ]\n", No);
		printf("[ 当前用户号码:%s  |  下一用户号码:%s ]\n", num[0], res);
		printf("------------------------------------\n");
		printf("|           账户操作                |\n");
		printf("|           1.存钱                  |\n");
		printf("|           2.取钱                  |\n");
		printf("|           3.其他业务           |\n");
		printf("|           0.退出                  |\n");
		printf("------------------------------------\n");
		printf("输入:");
	}
}

//展示界面和处理业务
void Homework(SOCKET s)
{
	char user[20];
	char passwd[7];
	char co;
	state = 10;
	show();
	while(true)
	{
		state = 10;
		if (_kbhit())
		{
			co = _getch();
			if (co == '1')
			{
				/*state = 11;
				show();*/
				printf("\n[ 暂无该业务! ]\n");
				Sleep(1000);
			}
			else if (co == '2')
			{
				/*state = 12;
				show();*/
				printf("\n[ 暂无该业务! ]\n");

			}
			else if (co == '0')
			{
				if (num[1][2] != 0 && num[1][3] != 0)
				{
					num[0][2] = num[1][2];
					num[0][3] = num[1][3];
					num[1][2] = 0;
					num[1][3] = 0;
					memset(buf, 0, DEFAULT_BUFLEN);
					buf[0] = Bus_End;
					send(s, buf, DEFAULT_BUFLEN, 0);	
				}
				else
				{
					num[0][2] = 0;
					num[0][3] = 0;
					state = 0;
					show();
					memset(buf, 0, DEFAULT_BUFLEN);
					buf[0] = Bus_End;
					send(s, buf, DEFAULT_BUFLEN, 0);
				}
			}
			else
			{
				printf("[ 输入的选项有误! ]\n");
				Sleep(1000);
			}
			break;
		}
	}
}

//业务程序
int main(int argc, char** argv)
{
	struct addrinfo* result = NULL;
	InitSock();
	SOCKET s = CreateTCPSocket();
	result = My_GetAddr(s);
	My_Connect(s, result);
	
	login(s);
	CreateThread(0, 0, Next_Num, (LPVOID)s, 0, 0);
	show();

	while (true)
	{
		if(num[0][2] != 0 || num[0][3] != 0)
			Homework(s);
	}
	return 0;
}

/*
设计思路:
*/

//（5）业务程序功能：通过服务器获取下一个可以办业务号码，并显示。两个业务程序应同时运行，交叉测试。

/*
叫号程序-->服务器-->业务程序：可以，执行业务。
叫号程序-->服务器-->业务程序：不可以，更新下一业务信息。
业务程序-->服务器：已经有下一业务信息，不更新。
业务程序-->服务器：业务结束，自动下一号码，取下下一个号码。
业务程序-->服务器-->叫号程序：下一号码工作了。
*/