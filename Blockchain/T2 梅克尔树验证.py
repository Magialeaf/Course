import hashlib

# 定义 MerkleTree 类
class MerkleTree:
    def __init__(self, data_list):
        # 初始化 MerkleTree 类，将构建好的 Merkle Tree 保存在 self.tree 中
        self.tree = []
        self.build_tree(data_list)

    def build_tree(self, data_list):
        """
        构建梅克尔树
        """
        leaf_nodes = [] # 叶子节点列表
        for data in data_list:
            # 计算每一个数据块的哈希值
            hash_value = self.calculate_hash(data)
            # 将哈希值与数据存成元组，作为叶子节点
            leaf_node = (hash_value, data)
            leaf_nodes.append(leaf_node)
        # 如果叶子节点为奇数个，则复制最后一个节点使得叶子节点数为偶数
        if len(leaf_nodes) % 2 == 1:
            leaf_nodes.append(leaf_nodes[-1])

        # 递归构建树，直到根节点
        while len(leaf_nodes) > 1:
            parent_nodes = []
            for i in range(0, len(leaf_nodes), 2):
                # 将相邻两个叶子节点的哈希值拼接成父节点的哈希值
                # 判断最后一个节点防止超出范围导致报错
                if i + 1 < len(leaf_nodes):
                    hash_value = self.calculate_hash(leaf_nodes[i][0] + leaf_nodes[i+1][0])
                    # 将哈希值与左右子节点组成元组
                    parent_node = (hash_value, leaf_nodes[i], leaf_nodes[i+1])
                    parent_nodes.append(parent_node)
                else:
                    hash_value = self.calculate_hash(leaf_nodes[i][0] + leaf_nodes[i][0])
                    # 将哈希值与左右子节点组成元组
                    parent_node = (hash_value, leaf_nodes[i], leaf_nodes[i])
                    parent_nodes.append(parent_node)
            leaf_nodes = parent_nodes
        self.tree = leaf_nodes[0]

    def get_root_hash(self):
        """
        获取根节点哈希值
        """
        return self.tree[0]
    # 嵌套元祖判断
    def is_value_in_nested_tuple(self,value, nested_tuple):
        for item in nested_tuple:
            if isinstance(item, tuple):  # 如果当前元素是嵌套元组，则递归调用本函数
                if self.is_value_in_nested_tuple(value, item):
                    return True
            elif item == value:  # 如果当前元素与指定值相等，则返回True
                return True
        return False  # 遍历完所有元素都没有找到指定值，则返回False

    def verify_datapoint(self, datapoint):
        """
        验证数据是否在 Merkle Tree 中，并且验证数据完整性
        """
        current_node = self.tree
        # 遍历 Merkle Tree，检查每个节点的哈希值
        while len(current_node) == 3:
            left_child_node = current_node[1]
            right_child_node = current_node[2]
            # 如果数据在左子树，则验证左子树的哈希值
            if self.is_value_in_nested_tuple(datapoint,left_child_node):
                if not self.verify_hash(current_node[0], left_child_node[0] + right_child_node[0]):
                    return False
                else:
                    current_node = left_child_node
            # 如果数据在右子树，则验证右子树的哈希值
            elif self.is_value_in_nested_tuple(datapoint,right_child_node):
                if not self.verify_hash(current_node[0], left_child_node[0] + right_child_node[0]):
                    return False
                else:
                    current_node = right_child_node
            else:
                return False
        return self.verify_hash(current_node[0], datapoint)

    def verify_hash(self, hash_value, data):
        """
        验证哈希值是否正确
        """
        return self.calculate_hash(data) == hash_value

    def calculate_hash(self, data):
        """
        计算数据的哈希值
        """
        return hashlib.sha256(data.encode()).hexdigest()


if __name__ == "__main__":
    # 原始数据
    data_list = ["block1", "block2", "block3", "block4","block5"]
    # 测试数据
    test_list = ["block1", "block2", "block3000", "block4", "block5"]

    # 创建 MerkleTree 实例
    merkle_tree = MerkleTree(data_list)

    # 获取根节点哈希值
    root_hash = merkle_tree.get_root_hash()
    print("Root hash: ", root_hash)

    # 验证数据完整性
    for data in test_list:
        result = merkle_tree.verify_datapoint(data)
        print(f"Verify {data}: {result}")