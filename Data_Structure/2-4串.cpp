#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<stdlib.h>

typedef struct string  //串
{
	char* ch = NULL;	
	int length = 0;
}HString;


void StrAssign(HString* T,char* chars) //生成一个值为chars的字符串T
{
	int i = 0;
	char* c = chars;
	if (T->length > 0)
		free(T->ch);
	while (*(c + i) != NULL)
		i++;
	if (!i)
	{
		T->ch = NULL;
		T->length = 0;
	}
	else
	{
		T->ch = (char*)malloc(i * sizeof(char));
		if (!T->ch)
			exit(0);
		for(int j =0;j<i;j++)
		*(T->ch+j) = *(chars+j);
		T->length = i;
	}
}

void StrVisit(HString T)  //输出字符串
{
	int i = 0;
	if (T.length > 0)
	{
		for (int i = 0; i < T.length; i++)
			printf("%c", *(T.ch + i));
		printf("\n");
	}
}

void StrCopy(HString* T,HString S) //由串S复制得到T
{
	StrAssign(T,S.ch);
	T->length = S.length;
}

bool StrEmpty(HString S) //判空，True为空
{
	if (S.length == 0)
		return true;
	return false;
}

int StrCompare(HString S, HString T) //比较字符串,若S>T返回>0
{
	int min_length;
	if (S.length < T.length)
		min_length = S.length;
	else
		min_length = T.length;
	for (int i = 0; i < min_length; i++)
	{
		if (*(S.ch + i) == *(T.ch + i))
				continue;
		else if (*(S.ch + i) > *(T.ch + i))
			return 1;
		else if (*(S.ch + i) < *(T.ch + i))
			return -1;
	}
	if (S.length == T.length)
		return 0;
	else if (S.length > T.length)
		return 1;
	else if (S.length < T.length)
		return -1;
}
int StrLength(HString S) //求长度
{
	return S.length;
}
void ClearString(HString* S) //清空串
{
	S->length = 0;
}
void Concat(HString* T, HString S1, HString S2) //S1 + S2 = T
{
	int length;
	int i;
	length = S1.length + S2.length;
	char* a = (char*)malloc(length * sizeof(char));
	for (i = 0; i < S1.length; i++)
		*(a + i) = *(S1.ch + i);
	for (i = S1.length; i < length; i++)
		*(a + i) = *(S2.ch + (i - S1.length));
	StrAssign(T, a);
	T->length = length;
	free(a);
}
void SubString(HString* Sub, HString S, int pos, int len) //用Sub返回串S的第pos个字符起长度为len的子串
{
	char* a = (char*)malloc(len * sizeof(char));
	for (int i = 0; i < len; i++)
		*(a + i) = *(S.ch + (pos - 1) + i);
	StrAssign(Sub, a);
	Sub->length = len;
	free(a);
}
int Index(HString S, HString T) //用pos返回串T出现在主串S中的第一处位置，否则返回0
{
	int count;
	for (int i = 0; i < S.length - T.length; i++)
	{
		count = 0;
		for (int j = 0; j < T.length; j++)
		{
			if (*(S.ch + i + j) != *(T.ch + j))
				break;
			else
				count++;
		}
		if (count == T.length)
			return (i + 1);
	}
	return 0;
}

void StrInsert(HString* S,int pos, HString T) //在串S的第pos个字符之前插入串T
{
	int length;
	int i,j;
	pos--;
	length = S->length + T.length;
	char* a = (char*)malloc(length * sizeof(char));
	for (i = 0; i < pos; i++)
		*(a + i) = *(S->ch + i);
	for (j = 0; j < T.length; j++)
		*(a + i + j) = *(T.ch + j);
	for (i = pos; i < S->length; i++)
		*(a + i + j) = *(S->ch + i);
	StrAssign(S, a);
	S->length = length;
	free(a);
}

void StrDelete(HString* S,int pos, int len) //从串S中删除第pos个字符起长度为len的子串
{
	int length;
	int i;
	pos--;
	length = S->length - len;
	char* a = (char*)malloc(length * sizeof(char));
	for (i = 0; i < pos; i++)
		*(a + i) = *(S->ch + i);
	for (i = pos + len; i < S->length; i++)
		*(a + (i - len)) = *(S->ch + i);
	StrAssign(S, a);
	S->length = length;
	free(a);
}

void DestroyString(HString* S) //销毁串S
{
	free(S->ch);
	S->length = 0;
}


void Replace(HString* S, HString T, HString V) //用V替换主串中出现所有与T相等的不重叠的子串
{
	HString temp;
	char* a = (char*)malloc(sizeof(char) * T.length);
	for (int i = 0; i < S->length - T.length + 1; i++)
	{
		for (int j = 0; j < T.length; j++)
			*(a + j) = *(S->ch + i + j);
		StrAssign(&temp, a);
		temp.length = T.length;
		if (StrCompare(temp, T) == 0)
		{
			StrDelete(S, i + 1, T.length);
			StrInsert(S, i + 1, V);
			i = i + V.length - 1;
		}
	}
	free(a);
}
void main()
{
	HString x;
	HString y;
	HString z;
	char a[] = "I will do it, but you willn't do it";
	char b[] = "will";
	char c[] = "would";
	StrAssign(&x, a);
	StrAssign(&y, b);
	StrAssign(&z, c);
	Replace(&x, y, z);
	StrVisit(x);
}