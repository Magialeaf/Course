import hashlib
class merkletree:
    def __init__(self,datalist):
        self.datalist = datalist
        self._mt = {}
        self.hashlist = self.init(datalist)
        (self.hashlist,self.layerlength) = self.grow(self.hashlist)
        self.merkle_root = self.hashlist["layer_{}".format(len(self.hashlist)-1)]

    def init(self,datalist):
        hash1 = []
        for x in datalist:
            msg = hashlib.sha256()
            msg.update(str(x).encode('utf-8'))
            hash1.append(msg.hexdigest())
        return hash1

    def combinehash(self,data1,data2):
        m = hashlib.sha256()
        m.update(data1.encode('utf-8'))
        m.update(data2.encode('utf-8'))
        return m.hexdigest()


    def grow(self,datalist):
        isOdd = True
        if len(datalist) % 2 == 0:
            isOdd = False
        pairnum = int(len(datalist) / 2)
        mt = {}
        layercount = 0
        prevlayerlist = []
        layerlength = []
        while len(prevlayerlist) >= 1 or layercount == 0:
            if layercount == 0:
                prevlayerlist = datalist
            mt["layer_{}".format(layercount)] = prevlayerlist
            layerlength.append(len(prevlayerlist))
            layercount += 1
            if len(prevlayerlist) == 1:
                break
            layerlist = []
            for i in range(pairnum):
                layerlist.append(self.combinehash(prevlayerlist[2*i],prevlayerlist[2*i+1]))
            if isOdd == True:
                layerlist.append(prevlayerlist[-1])
            prevlayerlist = layerlist
            isOdd = True
            if len(layerlist) % 2 == 0:
                isOdd = False
            pairnum = int(len(layerlist) / 2)
        return mt,layerlength

if __name__ == '__main__':
    data=list()
    for i in range(9):
        fileName = str(i+1)+".txt"
        with open("./files/" + fileName,'w') as f:
            f.writelines(str(i)*9)

    for i in range(9):
        fileName = str(i+1)+".txt"
        with open("./files/" + fileName,'r') as f:
            data.append(f.read())

    tree = merkletree(data)
    for key in list(tree.hashlist.keys())[::-1]:
        print(key,tree.hashlist[key])