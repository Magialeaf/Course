class RSA:
    # 加密（执行a的b次方mod c 操作）
    def candp(self,a:int,b:int,c:int)->int:
        r = 1
        for i in range(b):
            r *= a
            r %= c
        return r

    # 判断两数互质(传入参数为int类型,返回值为bool类型,True表示互质)
    def gcd(self,a:int,b:int)->bool:
        if a < b:
            return self.gcd(b, a)
        while a % b != 0:
            temp = b
            b = a % b
            a = temp
        return b    #互质返回1(True)，不互质返回大于1

    # 求a的逆元(传入要求逆元的参数a(int类型),范围为m(int类型),返回值为int类型)
    def re(self,a:int,m:int) -> int:
        a_re = 0  # 初始化逆元
        while True:  # 依照a * a^-1 mod m == 1算出a在m范围内的逆元a^-1，为逆元直接返回a_re,不为则让a_re+1再判断
            if a_re * a % m== 1:
                break
            else:
                a_re += 1
        return a_re

    # 主函数
    def main(self):
        while True:
            p = eval(input("input p:"))    #获得p
            q = eval(input("input q:"))    #获得q
            if self.gcd(p, q) == True:     #判断p,q互质
                print("p:%d,q:%d" % (p, q))
                break
            else:
                print("p与q不互质!")
        n = p * q  # 两数乘积
        fn = (p - 1) * (q - 1)  # 求n的欧拉函数
        print("n:%d,φ(n):%d" % (n, fn))
        while True:
            e = eval(input("input e(e与φ(n)互质):"))    #获得e
            if self.gcd(e, fn) == True:     #判断e与φ(n)互质
                print("e:%d" %e)
                break
            else:
                print("e与φ(n)不互质!")
        d = self.re(e,fn)     #求e的逆元d
        print("d:%d"%d)
        while True:
            print("————————————————————")
            print("p:%d,q:%d,n:%d,φ(n):%d,e:%d,d:%d"%(p,q,n,fn,e,d))
            i = eval(input("1.encode  2.decode  0.exit\ninput:"))
            if i == 1:
                plaintext = eval(input("input plaintext:"))
                r = self.candp(plaintext, e, n)
                print("the cipertext is %d"%r)
            elif i == 2:
                cipertext = eval(input("input cipertext:"))
                r = self.candp(cipertext, d, n)
                print("the plaintext is %d"%r)
            elif i == 0:
                break

if __name__ == '__main__':  # 主函数
    rsa = RSA()   #初始化类
    rsa.main()    #调用RSA类


