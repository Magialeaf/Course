import os

# 定义函数实现将文件编码转换为UTF-8
def convert_file_encoding(file_path):
    with open(file_path, 'r', encoding='gbk') as f:
        content = f.read()
    with open(file_path, 'w', encoding='utf-8') as f:
        f.write(content)

# 遍历指定目录下所有.cpp文件，将编码从GBK转换为UTF-8
dir_path = './'  # 指定目录路径
for root, dirs, files in os.walk(dir_path):
    for file in files:
        if file.endswith('.c'):
            file_path = os.path.join(root, file)
            convert_file_encoding(file_path)
