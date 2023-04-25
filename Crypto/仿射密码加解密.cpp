#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
int gcd(int x, int y)
{
    int t;
    while (y)
    {
        t = x % y;
        x = y;
        y = t;
    }
    return x;
}
void code(char* p, int k1, int k2, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        if (p[i] >= 'A' && p[i] <= 'Z')
            p[i] = (k1 * (p[i] - 'A') + k2) % 26 + 'A';
        else if (p[i] >= 'a' && p[i] <= 'z')
            p[i] = (k1 * (p[i] - 'a') + k2) % 26 + 'a';
        else
            p[i] = p[i];
    }
    p[i] = '\0';
    printf("加密后的密文是：%s\n", p);
}
void decode(char* p, int k1, int k2, int n)
{
    int i;
    int j = 0;
    int s;
    do
    {
        j++;
        s = (j * k1) % 26;
    } while (s != 1);
    for (i = 0; i < n; i++)
    {
        if (p[i] >= 'A' && p[i] <= 'Z')
            p[i] = (j * (p[i] - 'A' - k2 + 26)) % 26 + 'A';
        else if (p[i] >= 'a' && p[i] <= 'z')
            p[i] = (j * (p[i] - 'a' - k2 + 26)) % 26 + 'a';
        else
            p[i] = p[i];
    }
    p[i] = '\0';
    printf("解密后的明文是：%s\n", p);
}
int main()
{
    char res[100];
    int k1, k2, n;
    printf("请输入明文：");
    gets_s(res);
    do
    {
        printf("请输入密钥k1（与26互素）：");
        scanf("%d", &k1);
        if (gcd(k1, 26) != 1)
            printf("密钥输入非法,");
        else
            break;
    } while (true);
    printf("请输入密钥k2：");
    scanf("%d", &k2);
    n = strlen(res);
    code(res, k1, k2, n);
    decode(res, k1, k2, n);
    return 0;
}
