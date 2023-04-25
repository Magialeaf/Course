class BinaryHeap:
    def __init__(self):
        self.heapList = [0]
        self.currentSize = 0
    # 上浮
    def percUp(self, i:int)->None:
        while i // 2 > 0:
            if self.heapList[i] < self.heapList[i // 2]:
                self.heapList[i],self.heapList[i // 2] = self.heapList[i // 2],self.heapList[i]
            i //= 2
    # 下沉
    def percDown(self,i:int)->None:
        while i * 2 <= self.currentSize:
            mc = self.minChild(i)
            if self.heapList[i] > self.heapList[mc]:
                self.heapList[i],self.heapList[mc] = self.heapList[mc],self.heapList[i]
            i = mc
    # 最小子孩子
    def minChild(self,i:int)->int:
        if i * 2 + 1 > self.currentSize:
            return i * 2
        else:
            if self.heapList[i * 2] < self.heapList[i * 2 + 1]:
                return i * 2
            else:
                return i * 2 + 1
    # 插入后上浮保持有序
    def insert(self,key:any)->None:
        self.heapList.append(key)
        self.currentSize += 1
        self.percUp(self.currentSize)
    def findMin(self)->any:
        if self.size() > 0:
            return self.heapList[1]
        else:
            return None
    # 插入后尾节点替换主根，然后下沉保持有序
    def delMin(self)->any:
        retval = self.heapList[1]
        self.heapList[1] = self.heapList[self.currentSize]
        self.currentSize -= 1
        self.heapList.pop()
        self.percDown(1)
        return retval
    def isEmpty(self)->bool:
        return self.currentSize > 0
    def size(self)->int:
        return self.currentSize
    # 下沉法控制时间是O(n)
    def buildHeap(self,lst:list):
        i = len(lst) // 2
        self.currentSize = len(lst)
        self.heapList = [0] + lst[:]
        print(len(self.heapList),i)
        while i > 0:
            print(self.heapList,i)
            self.percDown(i)
            i -= 1
        print(self.heapList,i)

hp = BinaryHeap()

hp.insert(5)
hp.insert(17)
hp.insert(22)
hp.insert(15)
hp.insert(11)
hp.insert(9)
hp.insert(37)
hp.insert(25)
hp.insert(16)
hp.insert(7)

print(hp.heapList)

