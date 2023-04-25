#include "../服务器/Protocol.h"

char num[4][5] = { {'A','0',-1,-1},{'A','0',-1,-1} ,{'A','0',-1,-1},{'A','0',-1,-1} };

/****************************************声明函数********************************************/
//向服务器发出验证
void login(SOCKET s);
//显示号码功能
void show();
//状态改变函数
int Change_State(SOCKET s);
/****************************************定义函数********************************************/

//向服务器发出验证
void login(SOCKET s)
{
	int r;
	buf[0] = Call_Proc;
	send(s, buf, 1, 0);
	r = recv(s, buf, 3, 0);
	if (r > 0 && buf[0] == Get_Message && buf[1] == Call_Proc)
	{
		memset(buf, 0, DEFAULT_BUFLEN);
		buf[0] = Get_Message;
		send(s, buf, 1, 0);
		printf("[ 银行叫号程序启动成功! ]\n");
		Sleep(1500);
	}
	else
	{
		printf("[ 银行叫号程序启动失败! ]\n");
		closesocket(s);
		WSACleanup();
		system("pause");
		exit(0);
	}
}

//显示号码功能
void show()
{
	char res[4][5] = {};
	char free[] = "空闲";
	char close[] = "关闭";
	for (int i = 0; i < 4; i++)
	{
		if (num[i][2] == -1 && num[i][3] == -1)
			strcpy(res[i], close);
		else if (num[i][2] == 0 && num[i][3] == 0)
			strcpy(res[i], free);
		else
			strcpy(res[i], num[i]);
	}
	system("cls");
	printf("[ 银行叫号程序 ]\n");
	printf("------------------------------\n");
	printf("|                            |\n");
	printf("|   1号机当前号码:%s       |\n", res[0]);
	printf("|                            |\n");
	printf("|   1号机下一号码:%s       |\n", res[1]);
	printf("|                            |\n");
	printf("|   2号机当前号码:%s       |\n", res[2]);
	printf("|                            |\n");
	printf("|   2号机下一号码:%s       |\n", res[3]);
	printf("|                            |\n");
	printf("------------------------------\n");
}

//状态改变函数
int Change_State(SOCKET s)
{
	int r,res = 0;
	memset(buf, 0, DEFAULT_BUFLEN);
	r = recv(s, buf, 5, 0);
	if (r > 0)
	{
		if (buf[0] == Bus_Open)
		{
			if (buf[1] == 1)
			{
				for (int i = 0; i < 2; i++)
				{
					num[i][2] = 0;
					num[i][3] = 0;
				}
			}
			else if (buf[1] == 2)
			{
				for (int i = 2; i < 4; i++)
				{
					num[i][2] = 0;
					num[i][3] = 0;
				}
			}
			else if(buf[1] == 3)
			{
				for (int i = 0; i < 4; i++)
				{
					num[i][2] = 0;
					num[i][3] = 0;
				}
			}
		}
		else if (buf[0] == Bus_Close)
		{
			if (buf[1] == 1)
			{
				for (int i = 0; i < 2; i++)
				{
					num[i][2] = -1;
					num[i][3] = -1;
				}
			}
			else if (buf[1] == 2)
			{
				for (int i = 2; i < 4; i++)
				{
					num[i][2] = -1;
					num[i][3] = -1;
				}
			}
			else if (buf[1] == 3)
			{
				for (int i = 0; i < 4; i++)
				{
					num[i][2] = -1;
					num[i][3] = -1;
				}
			}
		}
		else if (buf[0] == Call_Num)
		{
			num[buf[1] + buf[2]][2] = buf[3];
			num[buf[1] + buf[2]][3] = buf[4];

		}
		else if (buf[0] == Del_Num)
		{
			num[buf[1] + buf[2]][2] = 0;
			num[buf[1] + buf[2]][3] = 0;
		}
		res = 0;
	}
	else if (r == 0)
	{
		system("cls");
		printf("[ 服务器已关闭! ]\n");
		res = -1;
	}
	else
	{
		system("cls");
		if (WSAGetLastError() == 10054)
			printf(" [ 服务器意外关闭! ]\n");
		else
			printf("[ 接收错误! ]: %d\n", WSAGetLastError());
		res = -1;
	}
	return res;
}

//叫号程序
int main(int argc, char** argv)
{
	struct addrinfo* result = NULL;
	InitSock();
	SOCKET s = CreateTCPSocket();
	result = My_GetAddr(s);
	My_Connect(s, result);

	login(s);

	int r = 0;
	do
	{
		show();
		r = Change_State(s);
	} while (r == 0);

	closesocket(s);
	WSACleanup();
	system("pause");
	return 0;
}

/*
设计思路:
*/

//（6）叫号程序：显示当前正在办业务号
/*
服务器-->取号程序-->叫号程序：取了一个号，可否使用。
叫号程序-->服务器：可以，更新状态。不可以，等待更新。
叫号程序-->服务器-->业务程序：可以，执行业务。
叫号程序-->服务器-->业务程序：不可以，更新下一业务信息。
*/