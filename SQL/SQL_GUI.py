from tkinter import *
import pymssql
class GUI:
    #定义桌面
    def __init__(self):
        self.win = Tk()
        self.width = 500
        self.heigth = 360
        self.win.title("服装信息管理系统")
        self.win.geometry("%dx%d"%(self.width,self.heigth))
        self.ID_GUI()  #进入登入界面
    # 登入界面
    def ID_GUI(self):
        self.frm_ID = Frame(self.win,width=self.width,height=self.heigth)
        self.frm_ID.pack()

        admin = Button(self.frm_ID, text="admin登入",command=self.Is_ID)
        admin.place(x=self.width // 6, y=0, height=30, width=self.width // 6)
        # waiter = Button(self.frm_ID, text="waiter登入",command=self.Waiter_Fram)
        # waiter.place(x=self.width // 6 * 2, y=0, height=30, width=self.width // 6)

        self.win.mainloop()
    #判断登入成功
    def Is_ID(self):
        self.frm_ID.destroy()
        self.con = pymssql.connect(host='127.0.0.1',port='1433',user='sa',password='SQLadmin',database='Clothing_Management',charset='utf8')
        self.Main_Fram()
    # 回到登入界面
    def Back_ID_GUI(self):
        self.frm_color.destroy()
        self.frm_switch.destroy()
        self.ID_GUI()
        self.con.close()
    #数据库管理员框架
    def Main_Fram(self):
        self.frm_color = Frame(self.win,width=self.width, height=self.heigth // 6)
        self.frm_color.config(background='#666E7A')
        self.frm_switch = Frame(self.win,width=self.width,height=self.heigth)
        self.frm_switch.config(background='#FFFFFF')
        self.frm_color.pack()
        self.frm_switch.pack()

        select = Button(self.frm_color, text="查询数据",command=self.Select_Data)
        select.place(x=0,y=0,height=self.heigth // 12,width=self.width // 6)
        update = Button(self.frm_color,text="修改数据",command=self.Update_Data)
        update.place(x=self.width // 6, y=0, height=self.heigth // 12, width=self.width // 6)
        delete = Button(self.frm_color, text="删除数据",command=self.Delete_Data)
        delete.place(x=self.width // 6 * 2,y=0,height=self.heigth // 12,width=self.width // 6)
        exit = Button(self.frm_color, text="退出登入",command=self.Back_ID_GUI)
        exit.place(x=self.width // 6 * 3,y=0,height=self.heigth // 12,width=self.width // 6)

        self.win.mainloop()
        self.con.close()
    # 查询数据库
    def Select_Data(self):
        self.frm_switch.pack_forget()
        self.frm_switch = Frame(self.win,width=self.width,height=self.heigth)
        self.frm_switch.config(background='#FFFFFF')
        self.frm_switch.pack()
        select_C = Button(self.frm_switch, text="查询服装信息表",command=self.Select_C)
        select_C.place(x=0,y=0,height=self.heigth // 8,width=self.width // 3)
        select_S = Button(self.frm_switch, text="查询销售信息表",command=self.Select_S)
        select_S.place(x=0, y=self.heigth // 8, height=self.heigth // 8, width=self.width // 3)
        select_St = Button(self.frm_switch, text="查询库存表",command=self.Select_St)
        select_St.place(x=0, y=self.heigth // 8 * 2, height=self.heigth // 8, width=self.width // 3)
        select_Month = Button(self.frm_switch, text="查询每月销售额",command=self.Select_Month)
        select_Month.place(x=self.width // 3,y=0, height=self.heigth // 8, width=self.width // 3)
        select_Hot = Button(self.frm_switch, text="查询每月热销商品（件）",command=self.Select_Hot)
        select_Hot.place(x=self.width // 3, y=self.heigth // 8, height=self.heigth // 8, width=self.width // 3)
        select_Hots = Button(self.frm_switch, text="查询每月热销商品（总价）",command=self.Select_Hots)
        select_Hots.place(x=self.width // 3, y=self.heigth // 8 * 2, height=self.heigth // 8, width=self.width // 3)
        select_C_in = Button(self.frm_switch, text="查询商品信息（参数）",command=self.Select_C_in)
        select_C_in.place(x=self.width // 3 * 2, y=0, height=self.heigth // 8, width=self.width // 3)
        select_S_in = Button(self.frm_switch, text="查询销售信息（参数）",command=self.Select_S_in)
        select_S_in.place(x=self.width // 3 *2, y=self.heigth // 8, height=self.heigth // 8, width=self.width // 3)
    # 修改数据库
    def Update_Data(self):
        self.frm_switch.pack_forget()
        self.frm_switch = Frame(self.win,width=self.width,height=self.heigth)
        self.frm_switch.config(background='#FFFFFF')
        self.frm_switch.pack()
        update_C = Button(self.frm_switch, text="修改服装信息",command=self.Update_C)
        update_C.place(x=0,y=0,height=self.heigth // 8,width=self.width // 3)
        update_S = Button(self.frm_switch, text="修改销售信息",command=self.Update_S)
        update_S.place(x=0, y=self.heigth // 8, height=self.heigth // 8, width=self.width // 3)
        update_St = Button(self.frm_switch, text="修改库存量",command=self.Update_St)
        update_St.place(x=0, y=self.heigth // 8 * 2, height=self.heigth // 8, width=self.width // 3)
        insert_C = Button(self.frm_switch, text="插入服装信息",command=self.Insert_C)
        insert_C.place(x=self.width // 3,y=0,height=self.heigth // 8,width=self.width // 3)
        insert_S = Button(self.frm_switch, text="插入销售信息",command=self.Insert_S)
        insert_S.place(x=self.width // 3, y=self.heigth // 8 , height=self.heigth // 8, width=self.width // 3)
    # 删除数据库
    def Delete_Data(self):
        self.frm_switch.pack_forget()
        self.frm_switch = Frame(self.win,width=self.width, height=self.heigth)
        self.frm_switch.config(background='#FFFFFF')
        self.frm_switch.pack()
        delete_C = Button(self.frm_switch, text="删除服装信息",command=self.Delete_C)
        delete_C.place(x=0,y=0,height=self.heigth // 8,width=self.width // 3)
        delete_S = Button(self.frm_switch, text="删除销售信息",command=self.Delete_S)
        delete_S.place(x=0, y=self.heigth // 8, height=self.heigth // 8, width=self.width // 3)
    # # 服装销售人员查询框架
    # def Waiter_Fram(self):
    #     self.frm_ID.destroy()
    #     self.frm_color = Frame(self.win,width=self.width, height=self.heigth // 6)
    #     self.frm_color.config(background='#666E7A')
    #     self.frm_switch = Frame(self.win,width=self.width,height=self.heigth)
    #     self.frm_switch.config(background='#FFFFFF')
    #     self.frm_color.pack()
    #     self.frm_switch.pack()
    #
    #     select = Button(self.frm_color, text="查询数据",command=self.Select_Some)
    #     select.place(x=0,y=0,height=self.heigth // 12,width=self.width // 6)
    #     update = Button(self.frm_color,text="修改数据",command=self.No_Power)
    #     update.place(x=self.width // 6, y=0, height=self.heigth // 12, width=self.width // 6)
    #     delete = Button(self.frm_color, text="删除数据",command=self.No_Power)
    #     delete.place(x=self.width // 6 * 2,y=0,height=self.heigth // 12,width=self.width // 6)
    #     exit = Button(self.frm_color, text="退出登入",command=self.Back_ID_GUI)
    #     exit.place(x=self.width // 6 * 3,y=0,height=self.heigth // 12,width=self.width // 6)
    #
    #     self.win.mainloop()
    # 更新数据库
    def Select_Some(self):
        self.frm_switch.pack_forget()
        self.frm_switch = Frame(self.win,width=self.width,height=self.heigth)
        self.frm_switch.config(background='#FFFFFF')
        self.frm_switch.pack()
        select = Button(self.frm_switch, text="查询")
        select.place(x=0,y=0,height=self.heigth // 8,width=self.width // 3)
    # # 无权限
    # def No_Power(self):
    #     new_win = Tk()
    #     new_win.title("Error！")
    #     new_win.geometry("200x60")
    #     frm_Power = Frame(new_win)
    #     frm_Power.pack()
    #     tip = Label(frm_Power,text="无权限")
    #     tip.pack()
    #     new_win.mainloop()
    # 查询服装信息表
    def Select_C(self):
        new_win = Tk()
        new_win.title("查询服装信息表")
        new_win.geometry("%dx%d"%(self.width * 2,self.heigth * 1.5))
        i = 1
        cursor = self.con.cursor()
        cursor.execute("exec all_view 'C'")
        data = cursor.fetchall()
        put = Label(new_win, text="商品号 商品名 商品类型 商品描述 商品单价")
        put.place(x=0, y=0)
        for (t) in data:
            put = Label(new_win,text=t)
            put.place(x=0,y=self.heigth // 20 * i)
            i += 1
        new_win.mainloop()
    # 查询销售信息表
    def Select_S(self):
        new_win = Tk()
        new_win.title("查询服装信息表")
        new_win.geometry("%dx%d"%(self.width * 2,self.heigth * 1.5))
        i = 1
        cursor = self.con.cursor()
        cursor.execute("exec all_view 'S'")
        data = cursor.fetchall()
        put = Label(new_win, text="订单号 商品号 销售日期 销售件数(件) 销售总金额(元)")
        put.place(x=0, y=0)
        for (t) in data:
            put = Label(new_win,text=t)
            put.place(x=0,y=self.heigth // 20 * i)
            i += 1
        new_win.mainloop()
    # 查询库存表
    def Select_St(self):
        new_win = Tk()
        new_win.title("查询服装信息表")
        new_win.geometry("%dx%d"%(self.width * 2,self.heigth * 1.5))
        i = 1
        cursor = self.con.cursor()
        cursor.execute("exec all_view 'St'")
        data = cursor.fetchall()
        put = Label(new_win, text="商品号 商品名称 库存量(件)")
        put.place(x=0, y=0)
        for (t) in data:
            put = Label(new_win,text=t)
            put.place(x=0,y=self.heigth // 20 * i)
            i += 1
        new_win.mainloop()
    # 查询每月收入
    def Select_Month(self):
        new_win = Tk()
        new_win.title("查询服装信息表")
        new_win.geometry("%dx%d"%(self.width * 2,self.heigth * 1.5))
        i = 1
        cursor = self.con.cursor()
        cursor.execute("exec all_view 'Mon_Money'")
        data = cursor.fetchall()
        put = Label(new_win, text="销售年月 商品名称 销售件数 销售总价")
        put.place(x=0, y=0)
        for (t) in data:
            put = Label(new_win,text=t)
            put.place(x=0,y=self.heigth // 20 * i)
            i += 1
        new_win.mainloop()
    # 查询每月热销
    def Select_Hot(self):
        new_win = Tk()
        new_win.title("查询服装信息表")
        new_win.geometry("%dx%d" % (self.width * 2, self.heigth * 1.5))
        i = 1
        cursor = self.con.cursor()
        cursor.execute("exec all_view 'Hot_N'")
        data = cursor.fetchall()
        put = Label(new_win, text="商品号 销售年月 销售总件数(件) 销售总价(元)")
        put.place(x=0, y=0)
        for (t) in data:
            put = Label(new_win, text=t)
            put.place(x=0, y=self.heigth // 20 * i)
            i += 1
        new_win.mainloop()
    # 查询每月热销
    def Select_Hots(self):
        new_win = Tk()
        new_win.title("查询服装信息表")
        new_win.geometry("%dx%d" % (self.width * 2, self.heigth * 1.5))
        i = 1
        cursor = self.con.cursor()
        cursor.execute("exec all_view 'Hot_M'")
        data = cursor.fetchall()
        put = Label(new_win, text="商品号 销售年月 销售总件数(件) 销售总价(元)")
        put.place(x=0, y=0)
        for (t) in data:
            put = Label(new_win, text=t)
            put.place(x=0, y=self.heigth // 20 * i)
            i += 1
        new_win.mainloop()
    # 查询商品信息（参数）
    def Select_C_in(self):
        self.new_win = Tk()
        self.new_win.title("查询服装信息表")
        self.new_win.geometry("%dx%d" % (self.width * 2, self.heigth * 1.5))
        self.new_fram = Frame(self.new_win,width=self.width * 2,height=self.heigth * 1.5)
        self.new_fram.pack()
        self.n1 = None
        self.n2 = None
        self.n3 = None
        # self.n4 = None
        # self.n5 = None
        # self.n6 = None
        self.n1 = StringVar()
        self.b1 = Label(self.new_fram,text='商品号:')
        self.b1.place(x=0, y=0, width=self.width // 10,height=self.heigth // 10)
        self.e1 = Entry(self.new_fram, textvariable=self.n1)
        self.e1.place(x=self.width // 10,y=0,width=self.width // 10,height=self.heigth // 10)
        go = Button(self.new_fram, text="确定",command=self.Select_C_ins)
        go.place(x=0,y=self.heigth // 10 * 4,width=self.width // 10,height=self.heigth // 10)
        self.new_win.mainloop()
    def Select_C_ins(self):
        i = 1
        self.new_fram.destroy()
        print(self.n1.get())
        cursor = self.con.cursor()
        cursor.execute("exec Select_C '%s',null,null"%(self.n1.get()))
        data = cursor.fetchall()
        put = Label(self.new_win, text="商品号 商品名 商品类型 商品描述 商品单价")
        put.place(x=0, y=0)
        for (t) in data:
            put = Label(self.new_win, text=t)
            put.place(x=0, y=self.heigth // 20 * i)
            i += 1
    # 查询销售信息（参数）
    def Select_S_in(self):return
    # 修改服装信息
    def Update_C(self):return
    # 修改销售信息
    def Update_S(self):return
    # 修改库存量
    def Update_St(self):return
    # 插入服装信息
    def Insert_C(self):return
    # 插入销售信息
    def Insert_S(self):return
    # 删除服装信息
    def Delete_C(self):return
    # 删除销售信息
    def Delete_S(self):return

if __name__ == "__main__":
    s = GUI()