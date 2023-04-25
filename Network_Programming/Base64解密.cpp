#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include <stdlib.h>



typedef struct
{
	union {
		struct {
			unsigned char s1, s2, s3, s4;
		}res_char;
		unsigned int s;
	}res_int;
}res;

void Encodebase64(const char str1[], char str2[])
{

}

void Decodebase64(const char str1[], char str2[])
{
	int i = 0, j;
	int len;
	int n;
	int temp;
	res res;
	len = strlen(str2);
	while (i != len)
	{
		res.res_int.s = 0;
		for (n = 3; n >= 0; n--)
		{
			if (str2[i] == '=')
			{
				temp = 0;
				temp = temp << (6 * n);
				res.res_int.s += temp;
				i++;
				continue;
			}
			for (j = 0; j < 64; j++)
			{
				if (str2[i] == str1[j])
				{
					temp = j;
					temp = temp << (6 * n);
					res.res_int.s += temp;
					i++;
					break;
				}
			}
		}
		printf("%c", res.res_int.res_char.s3);
		printf("%c", res.res_int.res_char.s2);
		printf("%c", res.res_int.res_char.s1);
	}
	printf("\n");
}

void main()
{
	int i = -1;
	char input[100];
	char base64char[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	while (i != 0)
	{
		system("cls");
		printf("选择加密或者解密：1.加密 2.解密 0.退出\ninput:");
		scanf("%d", &i);
		switch (i)
		{
		case 1:printf("请输入要加密的字符串:");
			scanf("%s", &input);
			Encodebase64(base64char, input);break;
		case 2:printf("请输入要解密的字符串:");
			scanf("%s", &input);
			Decodebase64(base64char, input);break;
		case 0:break;
		default:printf("error!\n");
		}
	}
}
