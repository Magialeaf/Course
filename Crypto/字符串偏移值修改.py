put = "e6Z9i~]8R~U~QHE{RnY{QXg~QnQ{^XVlRXlp^XI5Q6Q6SKY8jUAA"  #输入字符
res = []   #结果
k = 4      #设置偏移值
for i in put:
    res.append(chr(ord(i) - k))

res = "".join(res)

print(res)