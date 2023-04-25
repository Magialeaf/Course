import socketserver
import socket
import struct
import os
import re
import json
from optparse import OptionParser

def tip()->str:
    parser = OptionParser("Usage:%prog -p <port> --ip [true]")
    parser.add_option('-p',type='string',dest='port',help='specify target port')
    parser.add_option('--ip',type='string',dest='ip',help='get ip address')
    options,args = parser.parse_args()
    if options.ip == 'true':
        print("[+]IP address:", socket.gethostbyname(socket.gethostname()))
        exit(-1)
    else:
        try:
            return options.port
        except TypeError:
            print("Please add -h for usage")
            exit(-1)


# 多线程的发送文件
class MyServer(socketserver.BaseRequestHandler):
    buf_len = 1024
    def handle(self) -> None:
        print("[+]Client ip:",self.client_address[0],'\n')
        while True:
            filename = input("[Filename(exit)]>>>").strip()
            if(filename == 'exit'):
                print("[-]Connection Closed!")
                self.request.send(struct.pack('i',0))
                break
            head_info_len,head_info = self.operafile(filename)
            self.sendFile(self.request,head_info,head_info_len,filename)
        self.request.close()
    # 解析文件相关信息
    def operafile(self,filename:str)->tuple:
        filesize_bytes = os.path.getsize(filename)
        # pattern = re.compile(r'([^<>/\\\|:""\*\?]+\.\w+$') # 提取文件名
        # data = pattern.findall(filename)
        data = filename.split("\\")[-1]
        head_dir={
            'filename':data,
            'filesize_bytes':filesize_bytes
        }
        head_info = json.dumps(head_dir)
        head_info_len = struct.pack('i',len(head_info))
        return head_info_len,head_info
    # 发送文件
    def sendFile(self,conn:socket.socket,head_info:str,head_info_len:int,filename:str)->None:
        try:
            conn.send(head_info_len)
            conn.send(head_info.encode('utf-8'))
            with open(filename,'rb') as f:
                conn.sendall(f.read())
            print('[+]Send success! :: '+filename)
        except:
            print('[+]Send fail! :: ' + filename)

def main():
    port = int(tip())
    ip = socket.gethostbyname(socket.gethostname())
    addr = (ip,port)
    print("[+]listening at",addr)
    s = socketserver.ThreadingTCPServer(addr,MyServer)
    s.serve_forever()

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        print("interrupted by user, killing all threads...")