import hashlib
import time

# 根据难度目标值 target 挖矿，返回挖到的区块 Hash 值和挖矿所花费的时间
def mine_block(difficulty, block_data):
    start_time = time.time()  # 记录挖矿开始时间
    nonce = 0  # 初始 nonce 值为 0
    target = '0' * difficulty  # 难度目标值，例如 difficulty 为 2 时，target 为 '00'

    while True:
        # 创建区块数据和当前 nonce 的拼接字符串
        data_str = str(block_data) + str(nonce)
        # 计算 SHA256 Hash
        block_hash = hashlib.sha256(data_str.encode('utf-8')).hexdigest()

        # 如果挖到符合目标难度的 Hash，则挖矿成功，返回结果
        if block_hash[:difficulty] == target:
            end_time = time.time()
            mining_time = end_time - start_time
            return block_hash, mining_time
        nonce += 1  # 如果未挖到符合条件的 Hash，则 nonce 加 1 继续挖矿

if __name__ == '__main__':
    # 挖矿难度目标值为 2，需要写入新区块的数据为 "Hello, World!"
    block_hash, mining_time = mine_block(2, "Hello, World!")
    print("Block Hash:", block_hash)
    print("Mining Time:",mining_time)