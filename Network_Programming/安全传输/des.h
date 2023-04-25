#pragma once
#ifndef _DES_H_
#define _DES_H_
typedef unsigned char quint8;



// º”√‹
void encrypt8(const quint8 input[8], const quint8 key[8], quint8 output[8]);
void encrypt64(const quint8 input[64], const quint8 key[64], quint8 output[64]);
// Ω‚√‹
void decrypt8(const quint8 input[8], const quint8 key[8], quint8 output[8]);
void decrypt64(const quint8 input[64], const quint8 key[64], quint8 output[64]);



#endif // _DES_H_