class DES:     #定义DES类
    def __init__(self):
        #IP置换表
        self.IP_Table = [
        58,50,42,34,26,18,10,2,60,52,44,36,28,20,12,4,
        62,54,46,38,30,22,14,6,64,56,48,40,32,24,16,8,
        57,49,41,33,25,17, 9,1,59,51,43,35,27,19,11,3,
        61,53,45,37,29,21,13,5,63,55,47,39,31,23,15,7]
        #IP-1置换表
        self.IPR_Table = [
        40,8,48,16,56,24,64,32,39,7,47,15,55,23,63,31,
        38,6,46,14,54,22,62,30,37,5,45,13,53,21,61,29,
        36,4,44,12,52,20,60,28,35,3,43,11,51,19,59,27,
        34,2,42,10,50,18,58,26,33,1,41, 9,49,17,57,25]
        #E扩展表
        self.E_Table = [
        32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9,
         8, 9,10,11,12,13,12,13,14,15,16,17,
        16,17,18,19,20,21,20,21,22,23,24,25,
        24,25,26,27,28,29,28,29,30,31,32, 1]
        #PC1置换表
        self.PC1_Table = [
        57,49,41,33,25,17, 9, 1,58,50,42,34,26,18,
        10, 2,59,51,43,35,27,19,11, 3,60,52,44,36,
        63,55,47,39,31,23,15, 7,62,54,46,38,30,22,
        14, 6,61,53,45,37,29,21,13, 5,28,20,12, 4]
        #pc2表
        self.PC2_Table = [
        14,17,11,24, 1, 5, 3,28,15, 6,21,10,
        23,19,12, 4,26, 8,16, 7,27,20,13, 2,
        41,52,31,37,47,55,30,40,51,34,33,48,
        44,49,39,56,34,53,46,42,50,36,29,32]
        #移位表
        self.Move_Table = [1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1]
        #S盒
        self.S_Box = [
        #S1
        [[14, 4,13, 1, 2,15,11, 8, 3,10, 6,12, 5, 9, 0, 7],
        [  0,15, 7, 4,14, 2,13, 1,10, 6,12,11, 9, 5, 3, 8],
        [  4, 1,14, 8,13, 6, 2,11,15,12, 9, 7, 3,10, 5, 0],
        [ 15,12, 8, 2, 4, 9, 1, 7, 5,11, 3,14,10, 0, 6,13]],
        #S2
        [[15, 1, 8,14, 6,11, 3, 4, 9, 7, 2,13,12, 0, 5,10],
        [  3,13, 4, 7,15, 2, 8,14,12, 0, 1,10, 6, 9,11, 5],
        [  0,14, 7,11,10, 4,13, 1, 5, 8,12, 6, 9, 3, 2,15],
        [ 13, 8,10, 1, 3,15, 4, 2,11, 6, 7,12, 0, 5,14, 9]],
        #S3
        [[10, 0, 9,14, 6, 3,15, 5, 1,13,12, 7,11, 4, 2, 8],
        [ 13, 7, 0, 9, 3, 4, 6,10, 2, 8, 5,14,12,11,15, 1],
        [ 13, 6, 4, 9, 8,15, 3, 0,11, 1, 2,12, 5,10,14, 7],
        [  1,10,13, 0, 6, 9, 8, 7, 4,15,14, 3,11, 5, 2,12]],
        #S4
        [[7,13,14, 3, 0, 6, 9,10, 1, 2, 8, 5,11,12, 4,15],
        [13, 8,11, 5, 6,15, 0, 3, 4, 7, 2,12, 1,10,14, 9],
        [10, 6, 9, 0,12,11, 7,13,15, 1, 3,14, 5, 2, 8, 4],
        [ 3,15, 0, 6,10, 1,13, 8, 9, 4, 5,11,12, 7, 2,14]],
        #S5
        [[2,12, 4, 1, 7,10,11, 6, 8, 5, 3,15,13, 0,14, 9],
        [14,11, 2,12, 4, 7,13, 1, 5, 0,15,10, 3, 9, 8, 6],
        [ 4, 2, 1,11,10,13, 7, 8,15, 9,12, 5, 6, 3, 0,14],
        [11, 8,12, 7, 1,14, 2,13, 6,15, 0, 9,10, 4, 5, 3]],
        #S6
        [[12, 1,10,15, 9, 2, 6, 8, 0,13, 3, 4,14, 7, 5,11],
        [ 10,15, 4, 2, 7,12, 0, 5, 6, 1,13,14, 0,11, 3, 8],
        [  9,14,15, 5, 2, 8,12, 3, 7, 0, 4,10, 1,13,11, 6],
        [  4, 3, 2,12, 9, 5,15,10,11,14, 1, 7, 6, 0, 8,13]],
        #S7
        [[4,11, 2,14,15, 0, 8,13, 3,12, 9, 7, 5,10, 6, 1],
        [13, 0,11, 7, 4, 0, 1,10,14, 3, 5,12, 2,15, 8, 6],
        [ 1, 4,11,13,12, 3, 7,14,10,15, 6, 8, 0, 5, 9, 2],
        [ 6,11,13, 8, 1, 4,10, 7, 9, 5, 0,15,14, 2, 3,12]],
        #S8
        [[13, 2, 8, 4, 6,15,11, 1,10, 9, 3,14, 5, 0,12, 7],
        [  1,15,13, 8,10, 3, 7, 4,12, 5, 6,11, 0,14, 9, 2],
        [  7,11, 4, 1, 9,12,14, 2, 0, 6,10,13,15, 3, 5, 8],
        [  2, 1,14, 7, 4,10, 8,13,15,12, 9, 0, 3, 5, 6,11]]
        ]
        #P置换表
        self.P_Table = [
        16, 7,20,21,29,12,28,17, 1,15,23,26, 5,18,31,10,
         2, 8,24,14,32,27, 3, 9,19,13,30, 6,22,11, 4,25]
        #2->16进制表（字典）
        self.btoh = btoh = {"0000": '0', "0001": '1', "0010": '2', "0011": '3',
                            "0100": '4', "0101": '5', "0110": '6', "0111": '7',
                            "1000": '8', "1001": '9', "1010": 'a', "1011": 'b',
                            "1100": 'c', "1101": 'd', "1110": 'e', "1111": 'f'}
        #16->2进制表（字典）
        self.htob = {'0': [0, 0, 0, 0], '1': [0, 0, 0, 1], '2': [0, 0, 1, 0], '3': [0, 0, 1, 1],
                     '4': [0, 1, 0, 0], '5': [0, 1, 0, 1], '6': [0, 1, 1, 0], '7': [0, 1, 1, 1],
                     '8': [1, 0, 0, 0], '9': [1, 0, 0, 1], 'a': [1, 0, 1, 0], 'b': [1, 0, 1, 1],
                     'c': [1, 1, 0, 0], 'd': [1, 1, 0, 1], 'e': [1, 1, 1, 0], 'f': [1, 1, 1, 1]}
        #十六轮子密钥（先进行一次初始化）
        self.SubKey = [0 for i in range(16) for j in range(48)]
    #//字节转换成位；
    def ByteToBit(self,Data_in:str)->list:# void ByteToBit(bool* Data_out, char* Data_in, int Num);
        Data_out = []   #初始化Data_out
        for s in Data_in:   #每次从Data_in获得一个字符
            temp = []
            for i in bin(ord(s) + 256):     #把字符转化成ASCII码
                if i != 'b':
                    temp.append(int(i))
            temp = temp[-8:]#re
            temp = temp[::-1]
            Data_out = Data_out + temp    #把转化后的结果加入都Data_out列表中去
        return Data_out
    # //字节转换成位（整形）；
    def ByteToBitint(self,get:int)->list:
        Data_out = []  # 初始化Data_out
        if get < 10:
            get = str(get)
        else:
            get = chr(get + 87)
        Data_out = self.htob[get]
        Data_out = Data_out[::-1]#re
        return Data_out
    # //位转换成字节；
    def BitToByte(self,Data_in:list)->str:# void BitToByte(char My_message[9], bool* Message_in, int Num);
        Data_out = ""   #初始化Data_out
        for i in range(len(Data_in) // 8):
            temp = Data_in[i * 8:(i + 1) * 8]   #每次从Data_in获得八位
            temp = temp[::-1]#re
            temp = map(str,temp)
            temp = "".join(temp)
            Data_out = Data_out + chr(int(temp,base=2))  #转化成字符
        return Data_out
    # // 十六进制转二进制
    def HexToBit(self,Data_in:str)->list:# void HexToBit(bool* Data_out, char* Data_in, int Num);
        Data_out = []
        for i in Data_in:   #每次获得一个字符匹配十六进制转二进制字典，将转化结果加入到Data_out里
            temp = self.htob[i]
            temp = temp[::-1]#re
            Data_out = Data_out + temp
        return Data_out
    # //二进制转换成十六进制；
    def BitToHex(self,Data_in:list)->str:# void BitToHex(char* Data_out, bool* Data_in, int Num);
        Data_out = ""
        for i in range(0,len(Data_in) // 4):   #每次获得一个字符匹配十六进制转二进制字典，将转化结果加入到Data_out里
            temp = Data_in[i*4:(i+1)*4]
            temp = temp[::-1]#re
            temp = map(str,temp)
            temp = "".join(temp)
            Data_out = Data_out + self.btoh[temp]
        return Data_out
    # //各种表的置换算法；
    def TableReplace(self,Data_in:list,table:list,num:int)->list:# void TableReplace(bool* Data_out, bool* Data_in, const char* Table, int Num);
        Data_out = []
        for i in range(num):
            Data_out.append(Data_in[table[i] - 1])
        return Data_out         # void Bitcopy(bool* Data_out, bool* Data_in, int Num);
    #  //左移位；
    def Loop_bit(self,Data_out:list,movestep:int,lens:int)->list:# void Loop_bit(bool* Data_out, int movstep, int len);
        Data_out = Data_out[movestep:lens] + Data_out[0:movestep]     #切片拼接完成左移位
        return Data_out
    # //执行异或
    def Xor(self,Data_out:list,Data_in:list)->list:# void Xor(bool* Message_out, bool* Message_in, int Num);
        for i in range(len(Data_out)):
            Data_out[i] = Data_out[i] ^ Data_in[i]
        return Data_out
    #  // S盒变换；
    def S_change(self,Data_in:list)->list:# void S_change(bool* Data_out, bool* Data_in);
        Data_out = []
        for i in range(8):
            r = Data_in[6*i+0] * 2 + Data_in[6*i+5] * 1
            c = Data_in[6*i+1] * 8 + Data_in[6*i+2] * 4 +Data_in[6*i+3] * 2 + Data_in[6*i+4] * 1
            temp = self.ByteToBitint(self.S_Box[i][r][c])       #ByteToBit(Data_out, & S_Box[i][r][c], 4);
            Data_out = Data_out + temp
        return Data_out
    # // f函数;F_change(Message_bit_R, SubKey[i]);
    def F_change(self,Data_out:list,Data_in:list)->list:#  void F_change(bool Data_out[32], bool Data_in[48])
        message_E = []
        message_E = self.TableReplace(Data_out,self.E_Table,48)
        message_E = self.Xor(message_E,Data_in)
        Data_out = self.S_change(message_E)
        Data_out = self.TableReplace(Data_out,self.P_Table,32)
        return Data_out
    # //生成16轮的子密钥；
    def SetKey(self,key:str):# void SetKey(char My_key[9]);
        key_bit = self.ByteToBit(key)
        key_bit = self.TableReplace(key_bit,self.PC1_Table,56)
        key_bit_L = key_bit[0:28]       #    Key_bit_L = &Key_bit[0]; //key的左边28位；
        key_bit_R = key_bit[28:56]      #同理获得右28位
        for i in range(16):
            key_bit_L = self.Loop_bit(key_bit_L, self.Move_Table[i], 28)
            key_bit_R = self.Loop_bit(key_bit_R, self.Move_Table[i], 28)
            key_bit = key_bit_L + key_bit_R
            self.SubKey[i] = self.TableReplace(key_bit,self.PC2_Table,48)
    # //des的轮加密算法
    def Run_Des(self,message:str)->str:  # void Run_Des(char My_message[9], char HexMssage[16]);
        message_bit = self.ByteToBit(message)
        message_bit = self.TableReplace(message_bit,self.IP_Table,64)
        message_bit_L = message_bit[0:32]
        message_bit_R = message_bit[32:64]
        for i in range(16):
            temp = message_bit_R
            message_bit_R = self.F_change(message_bit_R,self.SubKey[i])
            message_bit_R = self.Xor(message_bit_R,message_bit_L)
            message_bit_L = temp
        message_bit = message_bit_L + message_bit_R
        message_bit = self.TableReplace(message_bit,self.IPR_Table,64)
        message_hex = self.BitToHex(message_bit)
        return message_hex
    # // DES轮解密算法；
    def Run_desDes(self,message_hex:str)->str:# void Run_desDes(char My_message[9], char HexMessage[16]);
        message_bit = self.HexToBit(message_hex)
        message_bit = self.TableReplace(message_bit,self.IP_Table,64)
        message_bit_L = message_bit[0:32]
        message_bit_R = message_bit[32:64]
        for i in range(15,-1,-1):
            temp = message_bit_L
            message_bit_L= self.F_change(message_bit_L, self.SubKey[i])
            message_bit_L = self.Xor(message_bit_L, message_bit_R)
            message_bit_R = temp
        message_bit = message_bit_L + message_bit_R
        message_bit = self.TableReplace(message_bit,self.IPR_Table,64)
        message = self.BitToByte(message_bit)
        return message
    # 加密主函数
    def encode(self):
        message = input("输入明文message:")  # 获得明文message
        key = input("输入加密密钥key（8bytes）:")  # 获得密钥key
        while (len(key) != 8):
            key = input("输入加密密钥key（8bytes）:")
        des.SetKey(key)  # 生成16轮的加密子密钥
        length = len(message)
        print("密文为:")
        for i in range(0,length,8):
            if length - i >= 8:
                message_cut = message[i:i+8]
                message_hex = des.Run_Des(message_cut)  # 加密
                print(str.upper(message_hex),end="")  # 转化成大写字母输出
            else:
                message_cut = message[i:length] + ' ' * (8 - (length - i))
                message_hex = des.Run_Des(message_cut)  # 加密
                print(str.upper(message_hex),end="")  # 转化成大写字母输出
        print()

    # 解密主函数
    def decode(self):
        message_hex = input("输入密文message_hex:")  # 获得明文message
        message_hex = str.lower(message_hex)
        key = input("输入加密密钥key（8bytes）:")  # 获得密钥key
        while (len(key) != 8):
            key = input("输入加密密钥key（8bytes）:")
        length = len(message_hex)
        print("明文为:")
        for i in range(0,length,16):
            message_cut = message_hex[i:i+16]
            des.SetKey(key)  # 生成16轮的加密子密钥
            message = des.Run_desDes(message_cut)  # 解密
            print(message,end="")
        print()


if __name__ == '__main__':  # 主函数
    des = DES()  # 设置对象des实例化类DES获得类DES中的所有属性和函数
    n = 0
    while n != '0':
        n = input("——————————————————\n选择加密或者解密\n1.加密\t2.解密\t0.退出\ninput:")  # 选择
        if n == '1':  # 加密
            des.encode()
        elif n == '2':  # 解密
            des.decode()
        else:
            pass