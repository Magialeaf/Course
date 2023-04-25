def new_S(key:str)->list:         #定义初始化S的函数(传入字符串key，返回列表)
    S = []                        #按照初始化S的步骤依次初始化
    T = []
    length = len(key)
    for i in range(256):          #range(0,len)表示从0到255进行迭代，每次+1)
        S.append(i)               #把i加到S列表的最后一个位置，就是S[i] = i
        T.append(key[i % length])          #同上把key[i % length]加到T列表的最后一个位置
    j = 0
    for i in range(256):
        j = (j + S[i] + ord(T[i])) % 256
        S[i],S[j] = S[j],S[i]           #交换S[i]和S[j]的值
    return S

def PRG(S:list)->list:         #定义密钥流生成函数(传入列表S，返回列表)
    K = []                     #按照密钥流生成函数步骤依次初始化
    i,j = 0,0
    num = 0
    while(num < 256):
        i = (i + 1) % 256
        j = (j + S[i]) % 256
        S[i],S[j] = S[j],S[i]           #交换S[i]和S[j]的值
        t = (S[i] + S[j]) % 256
        K.append(S[t])           #把S[t]加到K列表的最后一个位置，就是K[num] = S[t]
        num += 1
    return K


if __name__ == '__main__':      #主函数
    key = input("input key:")   #获得密钥存入key中
    K = PRG(new_S(key))         #先把key传入new_S()中初始化S，再把初始化后的S作为参数传入PRG()中完成密钥流的生成，返回密钥存入K中
    cipertext = []              #定义最后接收密钥的列表
    plaintext = input("plaintext:")        #获得明文存入plaintext中
    for i in range(0,len(plaintext)):      #循环(len()求长度,ord()转化成ASCII码，range(0,len)表示从0到len-1进行迭代，每次+1)
        cipertext.append(ord(plaintext[i]) ^ K[i])          #计算每一个明文加密后的结果加到cipertext列表的最后一个的位置
        print("plaintext[%d]:%c"%(i,plaintext[i]),"ciphertext[%d]:%x"%(i,cipertext[i]))        #输出明文和对应密文

    print("密文流为:",end='')             #进行一次密文流的总输出
    for i in range(0,len(plaintext)):
        print("%x"%cipertext[i],end='')

    print("\n解密结果为:",end='')             #依照异或规则在原密文基础上再进行一次异或完成解密
    for i in range(0,len(plaintext)):
        print("%c"%(cipertext[i] ^ K[i]),end='')