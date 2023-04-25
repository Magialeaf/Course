#pragma once
#include <math.h>
#include <stdlib.h>
#include <time.h>

int JudgePrime(int prime);// 判断prime是否为素数
int gcd(int a, int b);// 求a，b的最大公约数
int niyuan(int e, int mod);// 求e在模n下的逆元
void GetKey(int& PublicKey, int& PrivateKey, int& Model);
void EnCode(unsigned int* in, unsigned int* out, int len, int PublicKey, int Model);
void DeCode(unsigned int* in, unsigned int* out, int len, int PrivateKey, int Model);
