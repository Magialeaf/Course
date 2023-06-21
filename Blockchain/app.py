# 1
# from flask import Flask
#
# app = Flask(__name__)
#
# @app.route('/')
# def hello_world():
#     return 'Hello, World!'
#
# if __name__ == '__main__':
#     app.run()


# 2.1
# import hashlib
# m = hashlib.md5()
# m.update("Hello World".encode("utf-8"))
# print(m.hexdigest())

# 2.2
# import hashlib
# sha1 = hashlib.sha256()
# data = '2333333'
# sha1.update(data.encode('utf-8'))
# sha1_data = sha1.hexdigest()
# print(sha1_data)

# 2.3
# import hmac
# import hashlib
# # 第一个参数是密钥key，第二个参数是待加密的字符串，第三个参数是hash函数
# mac = hmac.new(b'key',b'hello',hashlib.md5)
# print(mac.digest()) # 字符串的ascii格式
# print(mac.hexdigest()) # 加密后字符串的十六进制格式


# 3
# 定义填充函数
import math
import struct

# 定义逻辑函数
def F(x, y, z):
    return (x & y) | (~x & z)

def G(x, y, z):
    return (x & z) | (y & ~z)

def H(x, y, z):
    return x ^ y ^ z

def I(x, y, z):
    return y ^ (x | ~z)

# 定义移位函数
def shift_left(x, n):
    return (x << n) | (x >> (32 - n))

def padding(msg):
    # 获取消息的长度
    length = len(msg) * 8

    # 计算填充的位数
    padding_length = (448 - length - 1) % 512

    # 填充消息
    padded_msg = msg + b'\x80' + b'\x00' * (padding_length // 8)

    # 将消息的长度添加到填充后的消息末尾
    padded_msg += struct.pack('<Q', length)

    return padded_msg

# 定义 MD5 函数，返回 128 位长度的 hash 值
def md5(msg):
    # 初始化 MD 缓冲区
    A = 0x67452301
    B = 0xEFCDAB89
    C = 0x98BADCFE
    D = 0x10325476

    # 定义 T 表
    T = [int((2 ** 32) * abs(math.sin(i + 1))) for i in range(64)]

    msg = padding(msg)

    # 处理每一个 512 位的消息块
    for i in range(0, len(msg), 64):
        # 将每个 512 位的消息块划分为 16 个 32 位的字
        M = [struct.unpack('<I', msg[i + j:i + j + 4])[0] for j in range(0, 64, 4)]

        # 保存 MD 缓冲区的值
        AA, BB, CC, DD = A, B, C, D

        # 迭代压缩函数，共迭代 64 次
        for j in range(64):
            if j < 16:
                A = B + shift_left((A + F(B, C, D) + M[j] + T[j]), 7)
                A, B, C, D = D, A, B, C
            elif j < 32:
                A = B + shift_left((A + G(B, C, D) + M[(5 * j + 1) % 16] + T[j]), 12)
                A, B, C, D = D, A, B, C
            elif j < 48:
                A = B + shift_left((A + H(B, C, D) + M[(3 * j + 5) % 16] + T[j]), 17)
                A, B, C, D = D, A, B, C
            else:
                A = B + shift_left((A + I(B, C, D) + M[7 * j % 16] + T[j]), 22)
                A, B, C, D = D, A, B, C

        # 更新 MD 缓冲区的值
        A = (A + AA) % (2 ** 32)
        B = (B + BB) % (2 ** 32)
        C = (C + CC) % (2 ** 32)
        D = (D + DD) % (2 ** 32)

    # 将 4 个缓冲区值连接成 128 位长度的 hash 值
    return '{:08x}{:08x}{:08x}{:08x}'.format(A, B, C, D)

msg = b'hello world'
print("Message before MD5 hashing:", msg)
md5_hash = md5(msg)
print("MD5 hash value:", md5_hash)
