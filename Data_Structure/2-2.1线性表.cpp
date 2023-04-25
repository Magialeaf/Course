#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#define List_Size 100  //线性表初始值
#define List_Increment 10  //线性表增长值

//<初始化表 销毁表 表增长>

typedef struct  //线性表结构体
{
	int *elem;  //基地址
	int length;  //现在的长度
	int listsize;  //总长度
}SqList;

//初始化线性表 &L:表 <分配失败完善>
void InitList(SqList* L) 
{
	L->elem = (int*)malloc(List_Size * sizeof(int));  //分配地址
	if (!L->elem)  //分配失败
		exit(0);
	L->length = 0;  //给长度初值
	L->listsize = List_Size;
}

//摧毁表 &L:表  <存在问题>
void DestroyList(SqList* L) 
{
	if (L->length > 0)
		printf("存在元素,无法摧毁!\n");
	else if (L->elem)
		free(L->elem);
}

//清空表 &L:表 
void ClearList(SqList* L)
{
	L->elem = L->elem - L->length;  //指针回到起点
	L->length = 0;  //表长为0
}
//向表尾增加元素 &L:表 e:增加的元素 <待完善,过界判断>
void AddList(SqList* L,int e)
{
	*L->elem = e;
	L->elem++;
	L->length++;
}

//判断是否空表 return true是 false否
bool ListEmpty(SqList L)
{
	if (L.length == 0)
		return true;
	else
		return false;
}

//返回元素个数 L:表 return元素个数
int ListLength(SqList L)
{
	return L.length;
}

//返回第i个元素值 L:表 i:第i个元素 return第i个元素值,不存在返回0
int GetElem(SqList L,int i)
{
	if (i > 0 && i <= L.length)
		return *(L.elem - L.length + i -1);
	else
		{
		printf("超出该线性表范围!\n");
		return 0;
		}
}

//返回和e相等的第一个数的位置 L:表 e:数字 return位置 不存在返回0 
int LocateElem(SqList L,int e)
{
	for(int i = 0; i<L.length;i++)
		if(*(L.elem - L.length + i) == e)
			return (i+1);
	return 0;			//不存在返回0
}

//当前元素前驱 L:表 cur_e:当前元素 return元素值,不存在返回0
int PriorElem(SqList L, int cur_e)
{
	int e_index = LocateElem(L, cur_e) - 1;
	if (e_index == -1)
	{
		printf("不存在该元素\n");
		return 0;
	}
	if (e_index == 0)
	{
		printf("当前元素无前驱\n");
		return 0;
	}
	return *(L.elem - L.length + e_index - 1);
}

//当前元素后继 L:表 cur_e:当前元素 return元素值,不存在返回0
int NextElem(SqList L, int cur_e)
{
	int e_index = LocateElem(L, cur_e) - 1;
	if(e_index == -1)
	{
		printf("不存在该元素\n");
		return 0;
	}
	if (e_index == L.length - 1)
	{
		printf("当前元素无后继\n");
		return 0;
	}
	return *(L.elem - L.length + e_index + 1);
}

//在第i个数字插入e &L:表 i:第i个位置 e:数字
void ListInsert(SqList* L,int i,int e)
{
	L->length++;
	for (int j = 0; j < L->length - i + 1; j++)
		*(L->elem -j + 1) = *(L->elem - j);
	*(L->elem - L->length + i) = e;
	L->elem++;
}

//删除第i个数字 &L:表 i:第i个位置 return删除的数字
int ListDelete(SqList* L, int i)
{
	int e = *(L->elem - L->length + i - 1);
	for (int j = 0; j < L->length - i + 1; j++)
		*(L->elem - L->length + i + j - 1) = *(L->elem - L->length + i + j);
	L->length--;
	L->elem--;
	return e;
}

//赋值:第i个元素值等于e &L:表 i:第i个元素 e:修改值
void PutElem(SqList* L,int i,int e)
{
	if(i>=0 && i< L->length)
	*(L->elem - L->length + i) = e;
}

//遍历表 L:表
void ListTraverse(SqList L)  
{
	for (int i = 0; i < L.length; i++)
		printf("Location:%d Elem:%d\n", i+1, *(L.elem - L.length + i));
}

/*int main()
{
	SqList x;
	InitList(&x);
	for (int i = 0; i < 10; i++)
	{
		ListInsert(&x, 1, i * 2);
	}
	ListInsert(&x, 5, 100);
	ListTraverse(x);
	ListInsert(&x, 7, 100);
	ListTraverse(x);
	return 0;
}*/