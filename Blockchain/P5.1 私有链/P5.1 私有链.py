import block as Block
import hashlib
import json
from textwrap import dedent
from time import time
from uuid import uuid4
from flask import Flask,request,render_template,jsonify
import requests
import rsa

if __name__ == '__main__':
    p = int(input("port:"))

    app = Flask(__name__)

    node_identifier = str(uuid4()).replace('-','.')

    b = Block.Blockchain()

    @app.route('/',methods=['GET'])
    def home():
        return render_template('index.html'
                               , address1="http://127.0.0.1:" + str(p) + "/chain"
                               , address2="http://127.0.0.1:" + str(p) + "/newtrans"
                               , address3="http://127.0.0.1:" + str(p) + "/newblock"
                               )

    @app.route('/chain',methods=['GET'])
    def allChain():
        response = {
            'chain':b.chain,
            'length':len(b.chain)
        }
        return jsonify(response)

    @app.route('/newtrans',methods=['GET'])
    def newtrans2():
        return render_template('newtrans.html')

    @app.route('/newtrans',methods=['POST'])
    def newtrans1():
        try:
            receiver = request.form['receiver']
            amount = request.form['amount']
            information = request.form['information']
            prikey_str  = request.form['prikey']
            prikey_str  = eval(repr(prikey_str).replace("\\\\","\\"))
            pubkey_str  = request.form['pubkey']
            pubkey_str  = eval(repr(pubkey_str).replace("\\\\","\\"))

            prikey_str = "-----BEGIN RSA PRIVATE KEY-----\n" + prikey_str + "\n-----END RSA PRIVATE KEY-----\n"
            pubkey_str = "-----BEGIN RSA PUBLIC KEY-----\n" + pubkey_str + "\n-----END RSA PUBLIC KEY-----\n"

            prikey = rsa.PrivateKey.load_pkcs1(prikey_str)
            pubkey = rsa.PublicKey.load_pkcs1(pubkey_str)

            if receiver == None:
                return "提交失败，接受者不能为空！"
            if amount == None:
                return "提交失败，数量不能为空！"
            if pubkey == None:
                return "提交失败，数字签名不能为空！"
            if prikey == None:
                return "提交失败，公钥不能为空！"

            sender = Block.Blockchain.hash(str(pubkey))
            tran = {"sender":sender,"receiver":receiver,"amount":amount,"information":information}
            # print(type(rsa.PrivateKey.load_pkcs1(prikey.encode())))
            # 修改：json.dumps(tran).encode()
            sign = rsa.sign(json.dumps(tran).encode(), prikey, 'SHA-256')
            if b.signProof(tran, sign, pubkey):
                return "提交成功！你提交的交易为:" + str(b.trans[-1]) + "请等待确认"
            else:
                return "提交失败！"
        except Exception as e:
            return e

    @app.route('/newblock',methods=['GET'])
    def newblock():
        b.workProof()
        return "新区块生成成功，新区块为: " + str(b.lastBlock())

    app.run(host='127.0.0.1',port=p)