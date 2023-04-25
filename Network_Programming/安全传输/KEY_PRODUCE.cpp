#include "KEY_PRODUCE.h"

// 判断prime是否为素数
int JudgePrime(int prime)
{
	int i;
	int limit = (unsigned int)sqrt((double)prime);

	for (i = 2; i <= limit; i++)
	{
		if (prime % i == 0) return 0;
	}
	return 1;
}

// 求a，b的最大公约数
int gcd(int a, int b)
{
	if (a <= 0 || b <= 0) return 0;
	while (b)
	{
		unsigned int r = a % b;
		a = b; b = r;
	}
	return a;
}

// 求e在模n下的逆元
int niyuan(int e, int mod)
{
	int x1, x2, x3, y1, y2, y3, t1, t2, t3, temp;
	x1 = 1; x2 = 0; x3 = mod;
	y1 = 0; y2 = 1; y3 = e;

	while (1)
	{
		if (y3 == 0)
		{
			return 0;
			break;
		}
		else if (y3 == 1)
		{
			if (y2 < 0)  return (y2 + mod);
			else return y2;
			break;
		}
		temp = x3 / y3;
		t1 = x1 - temp * y1; t2 = x2 - temp * y2; t3 = x3 - temp * y3;
		x1 = y1; x2 = y2; x3 = y3;
		y1 = t1; y2 = t2; y3 = t3;
	}
}
void GetKey(int& PublicKey, int& PrivateKey, int& Model)
{
	int p, q, oula;

	srand(time(0));
	while (1)
	{
		p = (int)rand();
		if (50 < p && p < 150 && JudgePrime(p)) break;
	}
	while (1)
	{
		q = (unsigned int)rand();
		if (50 < q && q < 150 && JudgePrime(q)) break;
	}
	Model = p * q;
	oula = (p - 1) * (q - 1);
	while (1)
	{
		PublicKey = (int)rand() % (oula - 2) + 2;
		if (gcd(PublicKey, oula) == 1) break;
	}
	PrivateKey = niyuan(PublicKey, oula);
}
void EnCode(unsigned int* in, unsigned int* out, int len, int PublicKey, int Model)
{

	for (int i = 0; i < len; i++)
	{

		int j = in[i];
		int temp = 1;
		for (int k = 0; k < PublicKey; k++)
		{
			temp *= j;
			if (temp >= Model)
				temp %= Model;
			if (!temp)
				out[i] = temp;
		}
		out[i] = temp % Model;
	}
}
void DeCode(unsigned int* in, unsigned int* out, int len, int PrivateKey, int Model)
{
	int codenum;
	codenum = len;
	for (int i = 0; i < codenum; i++)
	{
		int j = 0;
		int m = in[i];
		int temp = 1;
		for (int k = 0; k < PrivateKey; k++)
		{
			temp *= m;
			if (temp >= Model)
				temp %= Model;
			if (!temp)
				m = temp;
		}
		out[i] = temp % Model;
	}
}
