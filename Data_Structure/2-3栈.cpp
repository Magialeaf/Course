#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<stdlib.h>

#define Stack_size 100				//初始大小
#define Stack_increase 10			//增长大小

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
	free(S);
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
//若栈不空用e返回栈顶元素 否则返回0
int GetTop(SqStack* S)
{
	if (S->top == S->base)
		return 0;
	else
		return *(S->top - 1);
}
//插入e为新栈顶元素
void Push(SqStack* S, int e)
{
	if (S->top - S->base >= S->Stacksize)
	{
		S->base = (int*)realloc(S->base,(Stack_size + Stack_increase)* sizeof(int));
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
	if (n != 1)
		n = 0;
	if (n == 0)
		while(S.base != S.top)
			printf("%d\n", *S.base++);
	if (n == 1)
		while (S.base != S.top)
			printf("%d\n", *--S.top);
}

//十进制数转化成别的进制数
void conversion()
{
	int n, m, e;
	SqStack x;
	InitStack(&x);
	printf("输入十进制数:");
	scanf("%d", &m);
	printf("输入要转化的进制(十以内):");
	scanf("%d", &n);
	while (m)
	{
		Push(&x,m % n);
		m /= n;
	}
	while (StackLength(&x) > 0)
	{
		e = Pop(&x);
		printf("%d",e);
	}
}

//括号匹配
void bracket()
{
	char input[100];
	SqStack x;
	InitStack(&x);
	printf("输入一组括号（小中大）（不多于100个）:\n");
	scanf("%s", &input);
	for (int i = 0; i < strlen(input); i++)
	{
		if (input[i] == '(' || input[i] == '[' || input[i] == '{')
			Push(&x, input[i]);
		else if ((input[i] == ')' && Pop(&x) == '('))
			continue;
		else if ((input[i] == ']' && Pop(&x) == '['))
			continue;
		else if ((input[i] == '}' && Pop(&x) == '{'))
			continue;
		else
		{
			printf("bad input!\n");
			ClearStack(&x);
			return;
		}
	}
	printf("yes match!\n");
}


void main()
{
	conversion();
}
