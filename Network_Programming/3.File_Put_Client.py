import socket
import os
import sys
import json
import struct
from optparse import OptionParser

def tip()->tuple:
    parser = OptionParser("python %prog -i <ip> -p <port>")
    parser.add_option('-i',type='string',dest='ip',help='specify target ip')
    parser.add_option('-p',type='string',dest='port',help='specify target port')
    options,args = parser.parse_args()
    try:
        ip = options.ip
        port = int(options.port)
    except TypeError:
        print("Please add -h for usage")
        exit(-1)
    addr = (ip,port)
    return addr
# 接收文件
def recv_file(head_dir:dict,c:socket.socket)->None:
    filename = head_dir['filename']
    filesize = head_dir['filesize_bytes']
    print("[+]filename is "+ filename)
    print("[+]filesize is "+ str(filesize))
    recv_len = 0
    with open(filename , 'wb') as f:
        while recv_len < filesize:
            if filesize > 1024:
                recv_mesg = c.recv(1024)
                recv_len += len(recv_mesg)
                f.write(recv_mesg)
            else:
                recv_mesg = c.recv(filesize)
                recv_len += len(recv_mesg)
                f.write(recv_mesg)
    print("[+]File receiving completed！",'\n')

def main():
    addr = tip()
    c = socket.socket(socket.AF_INET,socket.SOCK_STREAM,socket.IPPROTO_TCP)
    c.connect(addr)
    print("[+]Wait for the server to send data...")

    while True:
        struct_len = c.recv(4) # 报头长
        struct_info_len = struct.unpack('i',struct_len)[0]
        if struct_info_len == 0:
            print("[-]Connection Closed!")
            break
        else:
            print("[+]Recv message len:"+str(struct_info_len))
            head_info = c.recv(struct_info_len)
            head_dir = json.loads(head_info.decode('utf-8'))
            print ("[+]Message is "+str(head_dir))
            recv_file(head_dir,c)


if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        print("interrupted by user, killing all threads...")