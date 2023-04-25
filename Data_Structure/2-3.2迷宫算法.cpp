#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<stdlib.h>
#include <time.h>

#define Stack_size 100				//初始大小
#define Stack_increase 10			//增长大小

void input(char a[][10]);

char show[10][10];

struct go 
{
	bool go[10][10];
}go;

typedef struct Stack  //栈
{
	int* top;			//栈顶指针
	int* base;			//栈底指针
	int Stacksize;	    //栈容量
}SqStack;

//构造一个空栈S
void InitStack(SqStack* S)
{
	S->base = (int*)malloc(Stack_size * sizeof(int));
	if (!S->base)
		exit(0);
	S->top = S->base;
	S->Stacksize = Stack_size;
}

//销毁栈S，S不再存在
void DestroyStack(SqStack* S)
{
	free(S->base);
	S = NULL;
}
//把S置为空栈
void ClearStack(SqStack* S)
{
	S->top = S->base;
}
//若S为空 返回true 否则返回false
bool StackEmpty(SqStack* S)
{
	if (S->top == S->base)
		return true;
	else
		return false;
}
//返回栈的元素个数 return 元素个数
int StackLength(SqStack* S)
{
	return (S->top - S->base);
}

//插入e为新栈顶元素
void Push(SqStack* S, int e)
{
	if (S->top - S->base >= S->Stacksize)
	{
		S->base = (int*)realloc(S->base, (Stack_size + Stack_increase) * sizeof(int));
		printf("栈容量增加！\n");
		if (!S->base)
			exit(0);
		S->top = S->base + S->Stacksize;
		S->Stacksize += Stack_increase;
	}
	*S->top++ = e;
}
//若栈不空 删除栈顶元素 返回e 否则返回0
int Pop(SqStack* S)
{
	if (S->top == S->base)
		return 0;
	return (*--S->top);
}

// n=0从栈底到栈顶遍历 n=1从栈顶访问到栈底
void StackTravserse(SqStack S, int n)
{
	int count = 0;
	if (n != 1)
		n = 0;
	if (n == 0)
		while (S.base != S.top)
		{
			printf("行%d,列:%d\t", *S.base / 100 + 1, *S.base % 100 + 1);
			show[*S.base / 100][*S.base % 100] = '1';
			S.base++;
			count++;
			if (count == 5)
			{
				printf("\n");
				count = 0;
			}
		}
	if (n == 1)
		while (S.base != S.top)
		{
			--S.top;
			printf("行%d,列:%d\t", *S.top / 100 + 1 , *S.top % 100 + 1);
			show[*S.top / 100][*S.top % 100] = '1';
			count++;
			if (count == 5)
			{
				printf("\n");
				count = 0;
			}
		}
	printf("\n");
	input(show);
}

void input(char a[][10])
{
	printf("迷宫如下:\n");
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
			printf("%c  ",a[i][j]);
		printf("\n");
	}
}
void seed(int n)
{
	srand(time(NULL));
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
			go.go[i][j] = false;
	char ma[10][10] = {'0','0','0','0','0','0','0','0','0','0',
					   '.','.','.','.','0','0','.','.','.','0',
					   '0','.','0','0','.','.','.','0','.','0',
					   '0','.','.','.','0','0','0','.','.','0',
					   '0','.','.','.','0','.','.','.','0','0',
					   '0','0','.','0','.','.','0','0','.','0',
					   '0','.','.','.','.','0','.','.','.','0',
					   '0','.','0','0','0','0','.','0','.','0',
					   '0','.','.','.','.','.','.','0','.','C',
					   '0','0','0','0','0','0','0','0','0','0' };

	char ran[10][10] = {'0','0','0','0','0','0','0','0','0','0',
						'.','.','.','.','.','.','.','.','.','0',
						'0','.','.','.','.','.','.','.','.','0',
						'0','.','.','.','.','.','.','.','.','0',
						'0','.','.','.','.','.','.','.','.','0',
						'0','.','.','.','.','.','.','.','.','0',
						'0','.','.','.','.','.','.','.','.','0',
						'0','.','.','.','.','.','.','.','.','0',
						'0','.','.','.','.','.','.','.','.','C',
						'0','0','0','0','0','0','0','0','0','0' };
	if (n == 1)
	{
		for (int i = 0; i < 10; i++)
			for (int j = 0; j < 10; j++)
				if (ma[i][j] == '0')
					go.go[i][j] = true;
		for (int i = 0; i < 10; i++)
			for (int j = 0; j < 10; j++)
				show[i][j] = ma[i][j];
		input(ma);
	}
	else
	{
		for (int i = 0; i < 20; i++)
			ran[rand() % 8 + 1][rand() % 8 + 1] = '0';
		for (int i = 0; i < 10; i++)
			for (int j = 0; j < 10; j++)
				if (ran[i][j] == '0')
					go.go[i][j] = true;
		for (int i = 0; i < 10; i++)
			for (int j = 0; j < 10; j++)
				show[i][j] = ran[i][j];
		input(ran);
	}	
}

//迷宫算法
void maze()
{
	int n = 0;
	int row = 1, col = 0;
	do {
		printf("选择地图:\n");
		printf("1.默认\n");
		printf("2.随机\n");
		scanf("%d", &n);
	} while (n != 1 && n != 2);
	seed(n);
	SqStack x;
	InitStack(&x);
	Push(&x, 100); //行列
	do
	{
		if (go.go[row][col + 1] == false)  //向右
		{
			go.go[row][col] = true;
			Push(&x, row * 100 + col);
			col++;
		}
		else if (go.go[row + 1][col] == false)  //向下
		{
			go.go[row][col] = true;
			Push(&x, row * 100 + col);
			row++;
		}
		else if (go.go[row][col - 1] == false)  //向左
		{
			go.go[row][col] = true;
			Push(&x, row * 100 + col);
			col--;
		}
		else if (go.go[row - 1][col] == false)  //向上
		{
			go.go[row][col] = true;
			Push(&x, row * 100 + col);
			row--;
		}
		else
		{
		n = Pop(&x);
		go.go[row][col] = true;
		row = n / 100;
		col = n % 100;
		}
		if (row == 8 && col == 9)
		{
			printf("此迷宫有解!如下:\n");
			StackTravserse(x, 0);
			ClearStack(&x);
			DestroyStack(&x);
			return;
		}

	} while (StackLength(&x) > 0);
	printf("此迷宫无解！\n");
	ClearStack(&x);
	DestroyStack(&x);
}

void main()
{
	int n = 1;
	do
	{
		system("cls");
		maze();
		printf("其他键继续\n");
		printf("0.退出\n");
		scanf("%d", &n);
	} while (n != 0);
	
}
