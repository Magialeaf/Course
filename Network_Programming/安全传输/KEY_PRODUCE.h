#pragma once
#include <math.h>
#include <stdlib.h>
#include <time.h>

int JudgePrime(int prime);// �ж�prime�Ƿ�Ϊ����
int gcd(int a, int b);// ��a��b�����Լ��
int niyuan(int e, int mod);// ��e��ģn�µ���Ԫ
void GetKey(int& PublicKey, int& PrivateKey, int& Model);
void EnCode(unsigned int* in, unsigned int* out, int len, int PublicKey, int Model);
void DeCode(unsigned int* in, unsigned int* out, int len, int PrivateKey, int Model);
