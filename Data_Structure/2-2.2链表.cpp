#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>

typedef struct LNode//线性表结构体
{
	int data;  //数据
	struct LNode *next;  //下一结点
}LNode, *LinkList;

//逆向初始化链表 &L:表 n:链表元素个数 <判断 p == NULL>
LinkList InitList_L(int n)
{
	LinkList L = (LinkList)malloc(sizeof(LNode));
	L->next = NULL;
	for (int i = 0; i < n; i++)
	{
		LinkList p = (LinkList)malloc(sizeof(LNode));
		if (p == NULL) // 存疑
			printf("malloc fair!\n");
		printf("Input data:");
		scanf("%d", &p->data);
		p->next = L->next;
		L->next = p;
	}
	return L;
}

//摧毁链表 &L:表  <存在问题>
void DestroyList_L(LinkList L)
{
	free(L);
}

//返回第i个元素值 L:表 i:第i个元素 return第i个元素值,不存在返回0
int GetElem_L(LinkList L, int i)
{
	int j = 1;
	LinkList p = (LinkList)malloc(sizeof(LNode));
	p = L->next;
	while (p && j < i)
	{
		p = p->next;
		j++;
	}
	if (!p)
	{
		free(p);
		return NULL;
	}
	else
	{
		j = p->data;
		free(p);
		return j;
	}
}

//在第i个数字插入e &L:表 i:第i个位置 e:数字
void ListInsert_L(LinkList L, int i, int e)
{
	int j = 0;
	LinkList p = (LinkList)malloc(sizeof(LNode));
	p = L;
	while (p && j < i - 1)
	{
		p = p->next;
		j++;
	}
	if (!p)
	{
		printf("插入失败!\n");
		return;
	}
	LinkList s = (LinkList)malloc(sizeof(LNode));
	s->next = p->next;
	s->data = e;
	p->next = s;
}

//删除第i个数字 &L:表 i:第i个位置 return删除的数字,不存在返回0
int ListDeleteL(LinkList L, int i)
{
	int j = 0;
	LinkList p = (LinkList)malloc(sizeof(LNode));
	p = L;
	while (p && j < i - 1)
	{
		p = p->next;
		j++;
	}
	if (!p)
	{
		printf("删除失败!\n");
		return 0;
	}
	LinkList q = (LinkList)malloc(sizeof(LNode));
	q = p->next;
	p->next = q->next;
	j = q->data;
	free(q);
	return j;
}

//遍历链表 &L:表
void ListTraverse_L(LinkList L)
{
	LinkList p = (LinkList)malloc(sizeof(LNode));
	p = L->next;
	int i = 1;
	while (p)
	{
		printf("Location:%d Data:%d\n", i++, p->data);
		p = p->next;
	}
	free(p);
}

LinkList MergeList_L(LinkList La, LinkList Lb)
{
	LinkList pa = (LinkList)malloc(sizeof(LNode));
	LinkList pb = (LinkList)malloc(sizeof(LNode));
	LinkList pc = (LinkList)malloc(sizeof(LNode));
	LinkList Lc = (LinkList)malloc(sizeof(LNode));
	pa = La->next; pb = Lb->next;
	Lc = pc = La;
	while(pa && pb)
		if(pa->data <= pb->data)
		{
			pc->next = pa; 
			pc = pa;
			pa = pa->next;
		}
		else
		{
			pc->next = pb;
			pc = pb;
			pb = pb->next;
		}
	pc->next = pa ? pa : pb;
	return Lc;
}

/*int main()
{
	LinkList x;
	LinkList y;
	LinkList z;
	x = InitList_L(3);
	ListTraverse_L(x);
	y = InitList_L(3);
	ListTraverse_L(y);
	z = MergeList_L(x,y);
	ListTraverse_L(z);

	return 0;
}*/
