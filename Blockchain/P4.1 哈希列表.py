import hashlib
import time

class HashList:
    def __init__(self):
        self.end = 10
        self.new_file()
        self.calculate_hash()

    def new_file(self):
        # 文件名列表，表示分片后文件名
        fileNameList = [str(i) + ".txt" for i in range(1,self.end)]

        # 根据文件名列表创建多个文件分片
        for fileName in fileNameList:
            # 为了保证每个文件分片的内容不一样，将当前时间字符串的sha256运算结果写入文件
            string = str(time.time())
            stringEncode = string.encode()
            stringHash = hashlib.sha256(stringEncode).hexdigest()
            with open("./files/" + fileName,'w') as f:
                for i in range(32):
                    f.write(stringHash)
            time.sleep(1)

    def calculate_hash(self):
        def hash_list(data):
            hashList = []
            hashLstring = ""
            for d in data:
                dencode = d.encode()
                dhash = hashlib.sha256(dencode).hexdigest()
                hashList.append(dhash)
                hashLstring = hashLstring + dhash
            hashLencode = hashLstring.encode()
            return hashList,hashlib.sha256(hashLencode).hexdigest()
        data = []
        # 读取分片文件
        for i in range(1,self.end):
            fileName = str(i) + ".txt"
            with open("./files/"+fileName,'r') as f:
                data.append(f.read())
        (hashList,hashValue) = hash_list(data)
        print(hashValue)
        print(hashList)

s = HashList()