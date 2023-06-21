import hashlib
import json
from time import time

class Blockchain:
    def __init__(self):
        self.chain = [] # 区块链列表
        self.current_transactions = [] # 当前交易列表
        # 创建创世区块
        self.new_block(previous_hash=1, proof=100)

    def new_block(self, proof, previous_hash=None):
        """
        创建新区块
        :param proof: <int> 工作量证明
        :param previous_hash: (Optional) <str> 前一个区块的哈希值
        :return: <dict> 新区块
        """

        block = {
            'index': len(self.chain) + 1,
            'timestamp': time(),
            'transactions': self.current_transactions,
            'proof': proof,
            'previous_hash': previous_hash or self.hash(self.chain[-1]),
        }
        # 清空当前交易列表
        self.current_transactions = []
        # 添加新区块到区块链上
        self.chain.append(block)

        return block

    def new_transaction(self, sender, recipient, amount):
        """
        添加新交易到当前交易列表
        :param sender: <str> 发送者地址
        :param recipient: <str> 接收者地址
        :param amount: <int> 金额
        :return: <int> 包含交易的区块的索引
        """
        self.current_transactions.append({
            'sender': sender,
            'recipient': recipient,
            'amount': amount,
        })

        return self.last_block['index'] + 1

    @staticmethod
    def hash(block):
        """
        计算 SHA-256 哈希值
        :param block: <dict> 区块
        :return: <str>
        """

        # 注意要对区块进行排序，保证哈希一致性
        block_string = json.dumps(block, sort_keys=True).encode()
        return hashlib.sha256(block_string).hexdigest()

    @property
    def last_block(self):
        # 返回区块链中最后一个区块
        return self.chain[-1]

blockchain = Blockchain()

# 添加新交易并创建新区块
blockchain.new_transaction("Tom", "Bob", 15)
blockchain.new_block(12345)

# 输出区块链
for chain in blockchain.chain:
    print(chain)