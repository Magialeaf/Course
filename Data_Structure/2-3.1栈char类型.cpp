#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<stdlib.h>

#define Stack_size 100				//初始大小
#define Stack_increase 10			//增长大小

typedef struct Stack  //栈
{
	char* top;			//栈顶指针
	char* base;			//栈底指针
	int Stacksize;	    //栈容量
}SqStack;

//构造一个空栈S
void InitStack(SqStack* S)
{
	S->base = (char*)malloc(Stack_size * sizeof(char));
	if (!S->base)
		exit(0);
	S->top = S->base;
	S->Stacksize = Stack_size;
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
void Push(SqStack* S, char e)
{
	if (S->top - S->base >= S->Stacksize)
	{
		S->base = (char*)realloc(S->base, (Stack_size + Stack_increase) * sizeof(char));
		printf("栈容量增加！\n");
		if (!S->base)
			exit(0);
		S->top = S->base + S->Stacksize;
		S->Stacksize += Stack_increase;
	}
	*S->top++ = e;
}
//若栈不空 删除栈顶元素 返回e 否则返回0
char Pop(SqStack* S)
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
		while (S.base != S.top)
			printf("%c", *S.base++);
	if (n == 1)
		while (S.base != S.top)
			printf("%c", *--S.top);
}

//文本编辑器
void text()
{
	char input[100];
	SqStack x;
	InitStack(&x);
	do
	{
		printf("输入文本(80字以内)(#退格@清空):\n");
		gets_s(input);
	} while (strlen(input) > 80);
	for (int i = 0; i < strlen(input); i++)
	{
		if (input[i] == '#')
			Pop(&x);
		else if (input[i] == '@')
			ClearStack(&x);
		else
			Push(&x, input[i]);
	}
	printf("你输入的文本如下：\n");
	StackTravserse(x, 0);
}

void main()
{
	text();
}
