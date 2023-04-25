#include "../服务器/Protocol.h"

/****************************************声明函数********************************************/
//设置非阻塞
void My_io(SOCKET s);
//向服务器发出验证
void login(SOCKET s);
//取号提示界面
void Tip();
//取号结果界面
void Get();
//取号主程序
void Num(SOCKET s);
/****************************************定义函数********************************************/

//设置非阻塞
void My_io(SOCKET s)
{
	int iMode = 1;
	int iResult = ioctlsocket(s, FIONBIO, (u_long*)&iMode);
	if (iResult == SOCKET_ERROR)
	{
		printf("ioctlsocket failed with error: %ld\n", WSAGetLastError());
		closesocket(s);
		WSACleanup();
		exit(0);
	}
}

//向服务器发出验证
void login(SOCKET s)
{
	int r;
	buf[0] = Get_Proc;
	send(s, buf, 1, 0);
	r = recv(s, buf, 3, 0);
	if (r > 0 && buf[0] == Get_Message && buf[1] == Get_Proc)
	{
		memset(buf, 0, DEFAULT_BUFLEN);
		buf[0] = Get_Message;
		send(s, buf, 1, 0);
		printf("[ 银行取号程序启动成功! ]\n");
		Sleep(1500);
	}
	else
	{
		printf("[ 银行取号程序启动失败! ]\n");
		closesocket(s);
		WSACleanup();
		system("pause");
		exit(0);
	}
}

//取号提示界面
void Tip()
{
	system("cls");
	printf("--------------------\n");
	printf("|  银行取号程序    |\n");
	printf("|  取号(回车)      |\n");
	printf("--------------------\n");
	printf("[Enter]:");
}

//取号结果界面
void Get()
{
	printf("\n----------------------------------\n");
	printf("|                                |\n");
	if (buf[0] == Get_Num_Succ)
		printf("|   取号成功,你的号码是:A0%c%c     |\n", buf[1], buf[2]);
	else if(buf[0] == Get_Num_Fail)
		printf("|   取号失败,当前排队人数过多!   |\n");
	else if (buf[0] == Call_Close)
		printf("|  叫号机未开启,无法取号!        |\n");
	else if(buf[0] == Bus_Close)
		printf("|  业务机未开启,无法取号!        |\n");
	printf("|                                |\n");
	printf("----------------------------------\n");
	Sleep(3000);
}

//取号主程序
void Num(SOCKET s)
{
	char botton;
	int r;
	while (true)
	{
		if (_kbhit())
		{
			botton = _getch();
			if (botton == 13)
			{
				memset(buf, 0, DEFAULT_BUFLEN);
				buf[0] = Get_Num;
				send(s, buf, 1, 0);
			}
		}
		r = recv(s, buf, 3, 0);
		if (r > 0)
		{
			Get();
			Tip();
		}
		else if (r == 0)
		{
			system("cls");
			printf("[ 服务器已关闭! ]\n");
			break;
		}
		else
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK);
			else
			{
				system("cls");
				if (WSAGetLastError() == 10054)
					printf(" [ 服务器意外关闭! ]\n");
				else
					printf("[ 接收错误! ]: %d\n", WSAGetLastError());
				break;
			}
		}
	}
}

//取号程序
int main(int argc,char** argv)
{
	struct addrinfo* result = NULL;
	InitSock();
	SOCKET s = CreateTCPSocket();
	result = My_GetAddr(s);
	My_Connect(s, result);
	
	login(s);
	My_io(s);
	Tip();

	Num(s);

	closesocket(s);
	WSACleanup();
	system("pause");
	return 0;
}

/*
设计思路:
*/

//（4）取号程序功能：通过服务器获取号码，并显示

/*
取号程序-->服务器：取号了。
服务器-->取号程序：取号结果。
*/