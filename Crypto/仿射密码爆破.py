def gcd(a:int)->int:          #判断a与26互质(传入参数为a(int类型),返回值为int类型)
    n = a
    while True:               #输入不为质数将一直循环
        m = 26                #m=26表示a与26互质
        while m:
            t = n % m
            n = m
            m = t
        if n != 1:            #不为质数重新输入
            n = input('a与26不互质!重新输入:')
            n = int(n)
            a = n
        else:                 #为质数返回a值
            return a


def re(a:int)->int:            #求a的逆元(传入参数为a(int类型),返回值为int类型)
    a_re = 0                   #初始化逆元
    while True:                #依照a * a^-1 mod 26 == 1算出逆元a^-1，为逆元直接返回a_re,不为则让a_re+1再判断
        if a_re * int(a) % 26 == 1:
            break
        else:
            a_re += 1
    return a_re

def encode(en:str,a:int,b:int)->str:    #加密函数(传入参数为en(str类型),a(int类型),b(int类型),返回值为str类型)
    de = ''                             #初始化de字符串用于接受密文
    for i in en:                        #for循环,以此获得en中的元素进行比较
        if i >= 'A' and i <= 'Z':       #利用𝐸(𝑥)=(𝑎𝑥+𝑏)𝑚𝑜𝑑 𝑚加密字母 ord()是将字符转成ASCII码,chr()是将ASCII码变回字符
            de = de + chr((a * (ord(i) - 65) + b) % 26 + 65)
        elif i >= 'a' and i <= 'z':
            de = de + chr((a * (ord(i) - 97) + b) % 26 + 97)
        else:
            de = de + i
    print('加密结果为:%s' % de)
    return de


def decode(de:str,a_re:int,b:int)->str:      #解密函数(传入参数为de(str类型),a(int类型),b(int类型),无返回值)
    res = ''
    for i in de:                        #for循环,以此获得de中的元素进行比较
        if i >= 'A' and i <= 'Z':       #利用D(𝑥)=(𝑎(𝑥-𝑏))𝑚𝑜𝑑 𝑚解密字母 ord()是将字符转成ASCII码,chr()是将ASCII码变回字符
            res = res + chr((a_re * ((ord(i) - 65) - b)) % 26 + 65)
        elif i >= 'a' and i <= 'z':
            res = res + chr((a_re * ((ord(i) - 97) - b)) % 26 + 97)
        else:
            res = res + i
    return res

def ende():
    # 主函数
    en = input('输入明文:')  # 获得明文,input中的内容是提示语,获得的值作为字符串存入en中
    a = input('密钥a(Key1):')  # 获得密钥a
    a = gcd(int(a))  # 判断a是否与26互质，先用int将a转化成整形
    b = input('密钥b(Key2):')  # 获得密钥b
    b = int(b)  # 把密钥b从字符串变成整形
    print("解密结果为:%s"%decode(encode(en, a, b), re(a), b))
    # 先调用了encode函数求密文并输出密文，之后将密文作为参数再传入decode函数中，并且将a的逆元也传入decode函数中

'''输入明文:Mountains multiply, streams double back — I doubt there's even a road
密钥a(Key1):31
密钥b(Key2):7
加密结果为:Pzduyhvut pdkyvekx, tyobhpt wzdmkb mhrf — V wzdmy yqbob't bibu h ozhw
解密结果为:Mountains multiply, streams double back — I doubt there's even a road
'''
def baopo():
    de = input('输入密文:')
    en = input('输入部分明文:')
    res = ''
    key_a = [1,3,5,7,9,11,15,17,19,21,23,25]
    for i in range(0,len(de) - len(en) + 1):
        for a in key_a:
            for b in range(0,26):
                res = decode(de[i:len(en) + i],re(a),b)
                if(res == en):
                    print("密钥a为:%d密钥b为:%d"%(a,b))
                    res = decode(de, re(a), b)
                    print('解密结果为:%s' % res)

if __name__ == '__main__':
    i = input("选择仿射密码加解密or爆破\n1.加解密     2.爆破\ninput:")
    if i == '1':
        ende()
    elif i == '2':
        baopo()
