import hashlib
import json
from time import time

class Blockchain(object):
    def __init__(self):
        self.chain = []
        self.current_transactions = []

        # 创建创世块
        self.new_block(previous_hash=1, proof=100)

    def new_block(self, proof, previous_hash=None):
        """
        生成新块
        :param proof: <int> 工作量证明提供者给出的证明
        :param previous_hash: (Optional) <str> 前一个块的hash值
        :return: <dict> 新块
        """

        block = {
            'index': len(self.chain) + 1,
            'timestamp': time(),
            'transactions': self.current_transactions,
            'proof': proof,
            'previous_hash': previous_hash or self.hash(self.chain[-1])
        }

        # 重置当前交易列表
        self.current_transactions = []

        self.chain.append(block)
        return block

    def new_transaction(self, sender, recipient, amount):
        """
        在下一个待挖的块中添加一个新交易
        :param sender: <str> 发送方地址
        :param recipient: <str> 接收方地址
        :param amount: <int> 数量
        :return: <int> 包含该交易的块的索引
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
        生成块的 SHA-256 hash值
        :param block: <dict> Block
        :return: <str>
        """

        # 确保字典是排好序的，否则会导致不一致的哈希值
        block_string = json.dumps(block, sort_keys=True).encode()
        return hashlib.sha256(block_string).hexdigest()

    @property
    def last_block(self):
        # 返回最后一个块
        return self.chain[-1]

b = Blockchain()
print(b.last_block())
b.new_block(123)
print(b.last_block())