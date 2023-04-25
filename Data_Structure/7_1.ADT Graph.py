class Vertex:
    def __init__(self, key: any):
        self.id = key
        self.connectedTo = {}
    def addNeighbor(self, nbr:object, weight=0) -> None:
        self.connectedTo[nbr] = weight
    def __str__(self)->any:
        return '[{0}]'.format(str(self.id)) + '<->' + str([x.id for x in self.connectedTo])
    def getConnections(self)->any:
        return self.connectedTo.keys()
    def getId(self)->any:
        return self.id
    def getWeight(self,nbr:any)->any:
        return self.connectedTo[nbr]

class Graph:
    def __init__(self):
        self.vertList = {}
        self.numVertices = 0
    def addVertex(self,key:any)->Vertex:
        self.numVertices += 1
        newVertex = Vertex(key)
        self.vertList[key] = newVertex
        return newVertex
    def getVertex(self,vKey:any)->any:
        if vKey in self.vertList:
            return self.vertList[vKey]
        else:
            return None
    def __contains__(self, vKey:any)->bool:
        return vKey in self.vertList

    def addEdge(self,fromVert:any,toVert:any,weight:any)->None:
        if fromVert not in self.vertList:
            newVertex = self.addVertex(fromVert)
        if toVert not in self.vertList:
            newVertex = self.addVertex(toVert)
        self.vertList[fromVert].addNeighbor(self.vertList[toVert],weight)
    def Vertices(self)->list:
        return list(self.vertList.keys())
    def __iter__(self)->iter:
        return iter(self.vertList.values())

g = Graph()
for i in range(6):
    g.addVertex(i*2)
for i in g:
    print(i)


