import block as Block
import json
import rsa

# pubkey_str = input("pubkey:")
# prikey_str = input("prikey:")
pubkey_str = 'MEgCQQDSRmOhJ4VL2oSyGj2JrF1UAjAQc6n211OqyCg4GE45ef03erRdGp2YvjRW\nbP5oli1FP+KgaBDHf8ASWQomqpmRAgMBAAE='
prikey_str = 'MIIBPgIBAAJBANJGY6EnhUvahLIaPYmsXVQCMBBzqfbXU6rIKDgYTjl5/Td6tF0a\nnZi+NFZs/miWLUU/4qBoEMd/wBJZCiaqmZECAwEAAQJBAMJwFkZ5nfejS5rJSugY\ne4BoOXutCJhZSmOcjBw61K64aIPppIxkum01hDPhtguem7F/0qA5ZEhiYSDeOpJ0\ngxUCIwDT7DXcLIpJFK2jcdhVskZC/HHTEEr5HPhcQ722a1pAUjQbAh8A/gJx96fS\ny0QBwIUI+CUqC2F2mnbwka90gz+a60vDAiMAyaplUqD78zF/TyL+CAa+/+2073H4\ntniQC1TXSql71+uh8wIfAPrM5USXgw3KT0SM1qdU4qesCxRG3g+qwwsYkVlESwIi\nX31sv3uDwQeER6q63YHu10xQ/xPwa7yByWy8sQfXxbVDKg=='
prikey_str = eval(repr(prikey_str).replace("\\\\", "\\"))
pubkey_str = eval(repr(pubkey_str).replace("\\\\", "\\"))

prikey_str = "-----BEGIN RSA PRIVATE KEY-----\n" + prikey_str + "\n-----END RSA PRIVATE KEY-----\n"
pubkey_str = "-----BEGIN RSA PUBLIC KEY-----\n" + pubkey_str + "\n-----END RSA PUBLIC KEY-----\n"

prikey = rsa.PrivateKey.load_pkcs1(prikey_str)
pubkey = rsa.PublicKey.load_pkcs1(pubkey_str)

b = Block.Blockchain()
sender = Block.Blockchain.hash(str(pubkey))

tran = {"sender" :1 ,"receiver" :1 ,"amount" :1 ,"information" :1}
sign = rsa.sign(json.dumps(tran).encode(), prikey, 'SHA-256')
if b.signProof(tran, sign, pubkey):
    b.newTrans(tran['sender'], tran['receiver'], tran['amount'], tran['information'])
    print("提交成功！你提交的交易为:" + str(b.trans[-1]) + "请等待确认")
else:
    print("提交失败！")