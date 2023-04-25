class HashTable:
    def __init__(self,size:int):
        self.size = size
        self.slots = [None] * self.size # 保存key
        self.data = [None] * self.size # 保存data
    # hash函数
    def hashfunction(self,key:any)->any:
        return key % self.size
    # 再hash函数
    def rehash(self,oldhash:int)->any:
        return (oldhash + 1) % self.size
    # 存key-val
    def put(self,key:any,data:any)->None:
        hashvalue = self.hashfunction(key)
        if self.slots[hashvalue] == None:
            self.slots[hashvalue] = key
            self.data[hashvalue] = data
        else:
            if self.slots[hashvalue] == key:
                self.data[hashvalue] = data
            else:
                nextslot = self.rehash(hashvalue)
                while self.slots[nextslot] != None and self.slots[nextslot] != key:
                    nextslot = self.rehash(nextslot)
                if self.slots[nextslot] == None:
                    self.slots[nextslot] = key
                    self.data[nextslot] = data
                else:
                    self.data[nextslot] = data
    # key获得data
    def get(self,key:any)->any:
        startslot = self.hashfunction(key)

        data = None
        stop = False
        found = False
        position = startslot

        while self.slots[position] != None and not found and not stop:
            if self.slots[position] == key:
                found = True
                data = self.data[position]
            else:
                position = self.rehash(position)
                if position == startslot:
                    stop = True
        return data
    # 索引查找
    def __getitem__(self, key):
        return self.get(key)
    # 索引设置
    def __setitem__(self, key, data):
        self.put(key,data)
if __name__ == '__main__':
    H = HashTable(11)
    H[54] = "cat"
    H[26] = "dog"
    H[93] = "lion"
    H[17] = "tiger"
    H[77] = "bird"
    H[31] = "cow"
    H[44] = "goat"
    H[55] = "pig"
    H[20] = "chicken"
    print(H.slots)
    print(H.data)
    print(H[20])
    print(H[17])
    H[20] = 'duck '
    print(H[20])
    print(H[99])