import hashlib      # 导入哈希函数模块
import random       # 导入随机数生成函数模块

def hash(block_str):
    """
    哈希函数，用于计算区块哈希值
    """
    return hashlib.sha256(block_str.encode()).hexdigest()

def generate_block(prev_block, transactions, validator_list, difficulty):
    """
    生成区块函数
    prev_block : str 前一个区块的哈希值
    transactions : list 区块中包含的所有交易信息列表
    validator_list : list 节点列表
    difficulty : int 当前网络的难度系数
    """
    nonce = 0   # 初始化随机数
    while True:
        validator = random.choice(validator_list)  # 随机选取一个验证节点
        block_str = (str(prev_block) + str(transactions) + str(nonce) + str(validator)) # 构造区块字符串
        block_hash = hash(block_str)   # 计算区块哈希值
        if block_hash.startswith('0' * difficulty):   # 判断是否满足难度要求
            return {'prev_block' : prev_block,
                    'transactions' : transactions,
                    'nonce' : nonce,
                    'validator': validator,
                    'hash' : block_hash}  # 返回生成的区块信息
        else:
            nonce += 1  # 如果不满足要求，随机数加1，开始新一轮计算

if __name__ == '__main__':
    prev_block = '00000000000000000000000000000000'   # 初始化前一个区块的哈希值
    transactions = ['A给B转账100元', 'B给C转账50元']   # 初始化交易信息
    validator_list = ['V1', 'V2', 'V3', 'V4', 'V5']   # 初始化验证节点列表
    difficulty = 4  # 初始化网络难度系数
    block = generate_block(prev_block, transactions, validator_list, difficulty)  # 生成区块
    print(block)    # 输出生成的区块信息