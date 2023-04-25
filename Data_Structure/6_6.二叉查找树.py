class TreeNode:
    def __init__(self, key: any = 0, val: any = 0, parent: object = None, left: object = None, right: object = None):
        self.key = key
        self.payload = val
        self.leftChild = left
        self.rightChild = right
        self.parent = parent
    def hasLeftChild(self)->object:
        return self.leftChild
    def hasRightChild(self)->object:
        return self.rightChild
    def isLeftChild(self)->bool:
        return self.parent and self.parent.leftChild == self
    def isRightChild(self)->bool:
        return self.parent and self.parent.rightChild == self
    def isRoot(self)->bool:
        return not self.parent
    def isLeaf(self)->bool:
        return not (self.rightChild or self.leftChild)
    def hasAnyChildren(self)->bool:
        return self.rightChild or self.leftChild
    def hasBothChildren(self)->bool:
        return self.rightChild and self.leftChild
    # 重新构造树
    def replaceNodeData(self,key:any,val:any,lc:object,rc:object)->None:
        self.key = key
        self.payload = val
        self.leftChild = lc
        self.rightChild = rc
        if self.hasLeftChild():
            self.leftChild.parent = self
        if self.hasRightChild():
            self.rightChild.parent = self
    def __iter__(self)->any:
        if self:
            if self.hasLeftChild():
                for elem in self.leftChild:
                    yield elem
            yield self.key
            if self.hasRightChild():
                for elem in self.rightChild:
                    yield elem
    # remove方法,寻找后继结点
    def findSuccessor(self)->object:
        succ = None
        if self.hasRightChild():
            succ = self.rightChild.findMin()
        else: # remove中不会遇到
            if self.parent:
                if self.isLeftChild():
                    succ = self.parent
                else:
                    self.parent.rightChild = None
                    succ = self.parent.findSuccessor()
                    self.parent.rightChild = self
        return succ
    def findMin(self):
        current = self
        while current.hasLeftChild():
            current = current.leftChild
        return current
    def spliceOut(self):
        if self.isLeaf():
            if self.isLeftChild():
                self.parent.leftChild = None
            else:
                self.parent.rightChild = None
        elif self.hasAnyChildren():
            if self.hasLeftChild():# remove不会遇到
                if self.isLeftChild():
                    self.parent.leftChild = self.leftChild
                else:
                    self.parent.rightChild = self.leftChild
                self.leftChild.parent = self.parent
            else:
                if self.isLeftChild():
                    self.parent.leftChild = self.rightChild
                else:
                    self.parent.rightChild = self.rightChild
                self.rightChild.parent = self.parent

class BinarySearchTree:
    def __init__(self):
        self.root = None
        self.size = 0
    def length(self):
        return self.size
    def __len__(self):
        return self.size
    def __iter__(self):
        return self.root.__iter__()
    # 插入值
    def put(self,key:any,val:any)->None:
        if self.root:    # 树非空插入
            self._put(key,val,self.root)
        else:
            self.root = TreeNode(key,val)
        self.size += 1
    def _put(self,key:any,val:any,currrentNode:TreeNode)->None:
        if key < currrentNode.key:
            if currrentNode.hasLeftChild():
                self._put(key,val,currrentNode.leftChild)
            else:
                currrentNode.leftChild = TreeNode(key,val,parent=currrentNode)
        else:
            if currrentNode.hasRightChild():
                self._put(key,val,currrentNode.rightChild)
            else:
                currrentNode.rightChild = TreeNode(key,val,parent=currrentNode)
    def __setitem__(self, key:any, value:any):
        self.put(key,value)
    # 获得值
    def get(self,key:any)->any:
        if self.root:
            res = self._get(key,self.root)
            if res:
                return res.payload
            else:
                return None
        else:
            return None
    def _get(self,key:any,currentNode:TreeNode)->TreeNode:
        if not currentNode:
            return None
        elif currentNode.key == key:
            return currentNode
        elif key < currentNode.key:
            return self._get(key,currentNode.leftChild)
        else:
            return self._get(key,currentNode.rightChild)
    def __getitem__(self, item:any):
        return self.get(item)
    def __contains__(self, item:any):  # tip:payload == 0
        if self._get(item,self.root):
            return True
        else:
            return False
    # 删除值
    def delete(self,key:any)->None:
        if self.size > 1:
            nodeToRemove = self._get(key,self.root)
            if nodeToRemove:
                self.remove(nodeToRemove)
                self.size -= 1
            else:
                raise KeyError('Error, key not in tree')
        elif self.size == 1 and self.root.key == key:
            self.root = None
            self.size -= 1
        else:
            raise KeyError('Error, key not in tree')
    def remove(self,currentNode:TreeNode):
        if currentNode.isLeaf():
            if currentNode == currentNode.parent.leftChild:
                currentNode.parent.leftChild = None
            else:
                currentNode.parent.rightChild = None
        elif currentNode.hasBothChildren():
            succ = currentNode.findSuccessor()
            succ.spliceOut()
            currentNode.key = succ.key
            currentNode.payload = succ.payload
        else:
            if currentNode.hasLeftChild():
                if currentNode.isLeftChild():
                    currentNode.leftChild.parent = currentNode.parent
                    currentNode.parent.leftChild = currentNode.leftChild
                elif currentNode.isRightChild():
                    currentNode.leftChild.parent = currentNode.parent
                    currentNode.parent.rightChild = currentNode.leftChild
                else:
                    currentNode.replaceNodeData(currentNode.leftChild,currentNode.leftChild.payload,currentNode.leftChild.leftChild,currentNode.rightChild.rightChild)
            else:
                if currentNode.isLeftChild():
                    currentNode.rightChild.parent = currentNode.parent
                    currentNode.parent.leftChild = currentNode.rightChild
                elif currentNode.isRightChild():
                    currentNode.rightChild.parent = currentNode.parent
                    currentNode.parent.rightChild = currentNode.rightChild
                else:
                    currentNode.replaceNodeData(currentNode.rightChild,currentNode.rightChild.payload,currentNode.rightChild.leftChild,currentNode.rightChild.rightChild)
    def __delitem__(self, key:any):
        self.delete(key)


s = BinarySearchTree()
for i in range(1,16):
    s[i] = i * 100
del s[9]