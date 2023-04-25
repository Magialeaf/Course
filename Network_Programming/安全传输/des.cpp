#include "des.h"

static quint8 s_ip_table[64] =
{
    58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17,  9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7
};

// 终止置换表
static quint8 s_ipr_table[64] =
{
    40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41,  9, 49, 17, 57, 25
};

// 密钥第一次置换表
static quint8 s_pc1_table[56] =
{
    57, 49, 41, 33, 25, 17,  9,  1, 58, 50, 42, 34, 26, 18,
    10,  2, 59, 51, 43, 35, 27, 19, 11,  3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,  7, 62, 54, 46, 38, 30, 22,
    14,  6, 61, 53, 45, 37, 29, 21, 13,  5, 28, 20, 12,  4
};

// 密钥第二次置换表
static quint8 s_pc2_table[48] =
{
    14, 17, 11, 24,  1,  5,  3, 28, 15,  6, 21, 10,
    23, 19, 12,  4, 26,  8, 16,  7, 27, 20, 13,  2,
    41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
};

// P盒置换表
static quint8 s_p_table[32] =
{
    16, 7, 20, 21, 29, 12, 28, 17, 1,  15, 23, 26, 5,  18, 31, 10,
    2,  8, 24, 14, 32, 27, 3,  9,  19, 13, 30, 6,  22, 11, 4,  25
};

// 左移位数表
const quint8 s_shift_bits[16] =
{
    1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

// 扩展表
static quint8 s_extend_table[48] =
{
    32,  1,  2,  3,  4,  5,  4,  5,  6,  7,  8,  9,
    8,  9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32,  1
};

// 8个S盒
static quint8 s_sbox[8][4][16] =
{
    // S1 
    {
        { 14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7 },
        { 0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8 },
        { 4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0 },
        { 15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13 }
    },
    // S2
    {
        { 15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10 },
        { 3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5 },
        { 0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15 },
        { 13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9 }
    },
    // S3
    {
        { 10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8 },
        { 13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1 },
        { 13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7 },
        { 1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12 }
    },
    // S4
    {
        { 7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15 },
        { 13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9 },
        { 10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4 },
        { 3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14 }
    },
    // S5
    {
        { 2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9 },
        { 14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6 },
        { 4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14 },
        { 11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3 }
    },
    // S6
    {
        { 12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11 },
        { 10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8 },
        { 9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6 },
        { 4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13 }
    },
    // S7
    {
        { 4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1 },
        { 13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6 },
        { 1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2 },
        { 6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12 }
    },
    // S8
    {
        { 13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7 },
        { 1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2 },
        { 7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8 },
        { 2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11 }
    }
};

// ip置换
static void ip_change(const quint8 input[64], quint8 output[64])
{
    for (int i = 0; i < 64; i++)
    {
        int j = s_ip_table[i] - 1;
        output[i] = input[j];
    }
}

// ipr置换
static void ipr_change(const quint8 left[32], const quint8 right[32], quint8 output[64])
{
    for (int i = 0; i < 64; i++)
    {
        int j = s_ipr_table[i] - 1;
        if (j < 32)
        {
            output[i] = left[j];
        }
        else
        {
            output[i] = right[j - 32];
        }
    }
}

// pc1置换
static void pc1_change(const quint8 input[64], quint8 output[56])
{
    for (int i = 0; i < 56; i++)
    {
        int j = s_pc1_table[i] - 1;
        output[i] = input[j];
    }
}

// pc2置换
static void pc2_change(const quint8 left[28], const quint8 right[28], quint8 output[48])
{
    for (int i = 0; i < 48; i++)
    {
        int j = s_pc2_table[i] - 1;
        if (j < 28)
        {
            output[i] = left[j];
        }
        else
        {
            output[i] = right[j - 28];
        }
    }
}

// p置换
static void p_change(const quint8 input[32], quint8 output[32])
{
    for (int i = 0; i < 32; i++)
    {
        int j = s_p_table[i] - 1;
        output[i] = input[j];
    }
}

// 左移1位
static void left_shift(quint8 input[28])
{
    quint8 bit0 = input[0];
    for (int i = 1; i < 28; i++)
    {
        input[i - 1] = input[i];
    }
    input[27] = bit0;
}

// 子密钥生成
static void subkey_fun(const quint8 input[64], quint8 output[16][48])
{
    // pc1置换（64位->56位）
    quint8 pc1[56] = { 0 };
    pc1_change(input, pc1);

    // 拆分为两个28位
    quint8 left[28] = { 0 };
    quint8 right[28] = { 0 };
    for (int i = 0; i < 28; i++)
    {
        left[i] = pc1[i];
        right[i] = pc1[i + 28];
    }

    // 生成16组新密钥
    quint8 pc2[16][48] = { 0 };
    for (int i = 0; i < 16; i++)
    {
        // 左移相应位数
        for (int j = 0; j < s_shift_bits[i]; j++)
        {
            left_shift(left);
        }
        for (int j = 0; j < s_shift_bits[i]; j++)
        {
            left_shift(right);
        }

        // pc2置换
        pc2_change(left, right, output[i]);
    }
}

// E扩展置换（32位明文扩展置换至48位）
static void eextend(const quint8 input[32], quint8 output[48])
{
    for (int i = 0; i < 48; i++)
    {
        quint8 j = s_extend_table[i] - 1;
        output[i] = input[j];
    }
}

// 异或
static void x_or(const quint8* inputa, const quint8* inputb, quint8* output, int len)
{
    for (int i = 0; i < len; i++)
    {
        *(output + i) = *(inputa + i) ^ *(inputb + i);
    }
}

// S盒压缩处理（从48位压缩至32位）
static void sbox(const quint8 input[48], quint8 output[32])
{
    for (int i = 0; i < 8; i++)
    {
        int j = i * 6;

        // bit0和bit5，组合得到行号
        int row = ((input[j] << 1) | input[j + 5]);
        // bit1-bit4，组合得到列号
        int col = ((input[j + 1] << 3) | (input[j + 2] << 2) | (input[j + 3] << 1) | input[j + 4]);

        // 4个bit输出
        quint8 value = s_sbox[i][row][col];

        j = i * 4;
        output[j] = ((value >> 3) & 0x1);
        output[j + 1] = ((value >> 2) & 0x1);
        output[j + 2] = ((value >> 1) & 0x1);
        output[j + 3] = (value & 0x1);
    }
}

// f函数（一个32位的数据区块和一个48位的秘钥区块，输出一个32位的区块）
static void f_func(const quint8 input[32], const quint8 subkey[48], quint8 output[32])
{
    int len = 48;

    // E扩展置换（32位明文扩展置换至48位）
    quint8 ext[48] = { 0 };
    eextend(input, ext);

    // 48位明文和48位密钥进行异或处理
    quint8 xr[48] = { 0 };
    x_or(ext, subkey, xr, 48);

    // S盒压缩处理（从48位压缩至32位）
    quint8 sbx[32] = { 0 };
    sbox(xr, sbx);

    // p置换
    p_change(sbx, output);
}

// 进制转换
static void to_binary(const quint8 input[8], quint8 output[64])
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            output[i * 8 + j] = ((input[i] >> (7 - j)) & 0x01);
        }
    }
}

void from_binary(const quint8 input[64], quint8 output[8])
{
    for (int i = 0; i < 8; i++)
    {
        quint8 v = 0;
        for (int j = 0; j < 8; j++)
        {
            v = ((v << 1) | input[i * 8 + j]);
        }
        output[i] = v;
    }
}

// 加密
void encrypt8(const quint8 input[8], const quint8 key[8], quint8 output[8])
{
    // 进制转换
    quint8 key64[64] = { 0 };
    to_binary(key, key64);

    quint8 input64[64] = { 0 };
    to_binary(input, input64);

    // 加密
    quint8 output64[64] = { 0 };
    encrypt64(input64, key64, output64);

    // 进制转换
    from_binary(output64, output);
}

void encrypt64(const quint8 input[64], const quint8 key[64], quint8 output[64])
{
    // 16个子密钥
    quint8 subkeys[16][48] = { 0 };
    subkey_fun(key, subkeys);

    // ip置换（64位->64位）
    quint8 ip[64] = { 0 };
    ip_change(input, ip);

    // 拆分为两个32位
    quint8 left[32] = { 0 };
    quint8 right[32] = { 0 };
    for (int i = 0; i < 32; i++)
    {
        left[i] = ip[i];
        right[i] = ip[i + 32];
    }

    // 16个迭代
    for (int i = 0; i < 16; i++)
    {
        // 一个32位的数据区块和一个48位的秘钥区块，输出一个32位的区块
        quint8 fc[32] = { 0 };
        f_func(right, subkeys[i], fc);

        // 经过P-盒置换的结果与最初64位分组的左半部分异或
        quint8 xr[32] = { 0 };
        x_or(left, fc, xr, 32);

        // 左右两部分交换，开始下一轮迭代
        for (int j = 0; j < 32; j++)
        {
            left[j] = right[j];
            right[j] = xr[j];
        }
    }

    // ipr置换
    ipr_change(right, left, output);
}

// 解密
void decrypt8(const quint8 input[8], const quint8 key[8], quint8 output[8])
{
    // 进制转换
    quint8 key64[64] = { 0 };
    to_binary(key, key64);

    quint8 input64[64] = { 0 };
    to_binary(input, input64);

    // 加密
    quint8 output64[64] = { 0 };
    decrypt64(input64, key64, output64);

    // 进制转换
    from_binary(output64, output);
}

void decrypt64(const quint8 input[64], const quint8 key[64], quint8 output[64])
{
    // 16个子密钥
    quint8 subkeys[16][48] = { 0 };
    subkey_fun(key, subkeys);

    // ip置换（64位->64位）
    quint8 ip[64] = { 0 };
    ip_change(input, ip);

    // 拆分为两个32位
    quint8 left[32] = { 0 };
    quint8 right[32] = { 0 };
    for (int i = 0; i < 32; i++)
    {
        left[i] = ip[i];
        right[i] = ip[i + 32];
    }

    // 16个迭代
    for (int i = 15; i >= 0; i--)
    {
        // 一个32位的数据区块和一个48位的秘钥区块，输出一个32位的区块
        quint8 fc[32] = { 0 };
        f_func(right, subkeys[i], fc);

        // 经过P-盒置换的结果与最初64位分组的左半部分异或
        quint8 xr[32] = { 0 };
        x_or(left, fc, xr, 32);

        // 左右两部分交换，开始下一轮迭代
        for (int j = 0; j < 32; j++)
        {
            left[j] = right[j];
            right[j] = xr[j];
        }
    }

    // ipr置换
    ipr_change(right, left, output);
}