#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<stdlib.h>

//三维数组的输出
void threeArr()
{
	int a[3][3][4];
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			for (int k = 0; k < 4; k++)
			{
				a[i][j][k] = i * 100 + j * 10 + k;
			}
	int* p = &a[0][0][0];
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				printf("%d\t", *(p + i * (sizeof(a[0]) / sizeof(int)) + j * (sizeof(a[0][0]) / sizeof(int)) + k));
			}
			printf("\n");
		}
		printf("\n");
	}
}

//void InitArray(&A, int n, int bound1, int bound2, int bound3, int bound4);

void main()
{
	threeArr();
}
