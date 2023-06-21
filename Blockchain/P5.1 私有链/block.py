import hashlib
import json
from time import time
from urllib.parse import urlparse
import requests
import rsa

# 全部变量，用于动态调整难度
hard = 4

class Blockchain(object):
    # 初始化
    def __init__(self):
        self.chain = []
        self.trans = []
        self.chain.append(
            {
             'index':len(self.chain) + 1,
             'time':time(),
             'trans':[],
             'proof':961113,
             'hash':self.hash(961113)
            }
        )

    # 产生新区块并将未确认的数据写入区块
    def newBlock(self,block=None):
        self.chain.append(block)
        self.trans = []
        return block

    # 接收交易（上传的数据）
    def newTrans(self,sender=None,receiver=None,amount=None,information=None):
        tran={
            'sender':sender,
            'receiver':receiver,
            'amount':amount,
            'information':information
            }
        self.trans.append(tran)
        return self.lastBlock()['index'] + 1

    # 计算区块的哈希值
    @staticmethod
    def hash(block):
        blockString = json.dumps(block,sort_keys=True)
        blockEncode = blockString.encode()
        blockHash = hashlib.sha256(blockEncode).hexdigest()
        return blockHash

    # 进行工作量证明
    def workProof(self):
        proof = 0
        while True:
            block={
            'index':len(self.chain) + 1,
            'time':time(),
            'trans':self.trans,
            'proof':proof,
            'hash':self.hash(self.lastBlock())
            }
            block_hash = Blockchain.hash(block)
            i = 0
            for j in range(len(block_hash)):
                if block_hash[j] != '0':
                    break
                else:
                    i += 1
            if i >= hard:
                self.newBlock(block)
                return block
            proof += 1

    # 验证数字签名
    def signProof(self,tran,sign,pubkey):
        try:
            # 修改：json.dumps(tran).encode()
            rsa.verify(json.dumps(tran).encode(),sign,pubkey)
            sender = tran['sender']
            receiver = tran['receiver']
            amount = tran['amount']
            information = tran['information']
            self.newTrans(sender,receiver,amount,information)
            return True
        except rsa.VerificationError as e:
            return False
        except KeyError as e:
            return False
        
    # 查看最新的区块
    def lastBlock(self):
        return self.chain[-1]

