--创建数据库
-------------------------------------------------------------------------------------------
create database Clothing_Management
on
(
	name = Clothing,
	filename = 'D:\E\SQL_file\DB\Clothing.mdf',
	size = 10,
	maxsize = 500,
	filegrowth = 10
)
log on
(
	name = Clothing_log,
	filename = 'D:\E\SQL_file\DB\Clothing_log.ldf',
	size = 10,
	maxsize = 500,
	filegrowth = 5
)
--创建数据表
-------------------------------------------------------------------------------------------
--服装信息（商品号(主键)、商品名、商品类型、商品描述、商品图片、销售价格(单)）
create table C
(
	C_No varchar(10) constraint C_No_Prim primary key,                      --商品号(主键)
	C_Name nvarchar(20) constraint C_Name_Not not null,                     --商品名（非空）
	C_Type nvarchar(20) constraint C_Type_Def default '无类型',             --商品类型（默认无类型）
	C_Discribe nvarchar(60) constraint C_Discribe_Def default '无描述',     --商品描述（默认无描述）
	C_Photo image,                                                          --商品图片
	C_Price money constraint C_Price_Not not null,                           --销售价格(单)（非空，大于0）
	constraint C_Price_More check(C_Price > 0) 
) 
--销售信息（订单号(主键)、商品号（C表外部键）、销售日期、销售数量、销售金额(总)）
create table S
(
	S_Order varchar(10) constraint S_Order_Prim primary key,               --订单号（S表主键）
	S_No varchar(10) constraint S_No_Fore foreign key references C(C_No),  --商品号（C表外部键）
	S_Date date constraint S_Date_Not not null,                                --销售日期（非空）
	S_Num int  constraint S_Price_More check(S_Num > 0),                   --销售数量（大于0）
	S_Price money                                                          --销售金额(总)
)
--存储信息(商品号（C表外部键）、库存量)
create table St
(
	St_No varchar(10) constraint St_No_Fore foreign key references C(C_No), --商品号（C表外部键）
	St_Num int constraint St_Num_Def default 0,                             --库存量（默认0）
	constraint St_No_uni unique(St_No),                                     --商品号唯一
	constraint St_Num_More check(St_Num >= 0)                               --库存量（大于等于0）
)

--进货表（商品号（主键）、进货日期、进货件数）
create table I
(
	I_Order varchar(10) constraint I_Order_Prim primary key,              --订单号（主键）
	I_No varchar(10) constraint I_No_Not not null,    --商品号
	I_Date date constraint I_Date_Not not null,                           --进货日期（非空）
	I_Num int constraint I_Num_More check(I_Num >= 0)                     --进货件数大于0
)

-----------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------

--插入默认数据
-------------------------------------------------------------------------------------------
--C表服装信息数据（商品号(主键)、商品名、商品类型、商品描述、商品图片、销售价格）
insert into C(C_No,C_Name,C_Type,C_Discribe,C_Photo,C_Price)
values
('010001','polo衫男短袖','男生夏衣','纯棉夏季polo衫男短袖上衣翻领t恤全棉简约宽松透气带领T商务休闲','Photo\010001.png',69),
('010002','短袖T恤','男生夏衣','花花公子短袖T恤男士夏季2022新款纯棉宽松潮牌男装休闲套装夏装','Photo\010002.png',139),
('010003','男装连帽夹克','男生秋衣','杉杉男装连帽夹克男加绒2022秋冬季新款保暖男士外套','Photo\010003.png',299),
('010004','加厚棉衣棒球服','男生秋衣','墨麦客男装春秋季加厚棉衣棒球服夹克男士休闲飞行员棉服外套男潮','Photo\010004.png',206),
('010005','森马棉服','男生冬衣','森马棉服男2022年冬季新款外套宽松抽绳户外机能冬装休闲潮','Photo\010005.png',324),
('010006','南极人冬季羽绒','男生冬衣','南极人冬季羽绒服男士2022年新款潮牌加厚帅气衣服短款冬装外套男','Photo\010006.png',213),
('010007','锥形裤男夏季','男生夏裤','乱步冰丝薄款锥形裤男夏季韩版潮流宽松九分裤弹力速干休闲长裤子','Photo\010007.png',22),
('010008','秋裤男士','男生秋裤','秋天韩版休闲裤男长裤小脚裤修身潮青少年男生裤子男士小西裤秋季','Photo\010008.png',29),
('010009','束脚工装裤','男生冬裤','秋季束脚工装裤男黑色休闲运动裤子宽松大码美式长裤秋冬加绒加厚','Photo\010009.png',110),
('010010','白衬衫t恤女','女生夏衣','wan能搭配~春秋棉拼接白衬衫t恤女韩版长袖宽松设计感小众打底衫','Photo\010010.png',69),
('010011','卫衣女','女生秋衣','灰色连帽开衫卫衣女2022秋季新款宽松百搭上衣设计感小众休闲外套','Photo\010011.png',100),
('010012','长款毛呢外套女','女生冬衣','orange desire中长款毛呢外套女2022冬新款双面呢子大衣宽松韩版','Photo\010012.png',700),
('010013','阔腿裤女','女生夏裤','果绿色阔腿裤女夏季薄款小个子直筒百搭高腰显瘦西装裤垂感高级感','Photo\010013.png',79),
('010014','法式气质连衣裙','女生长裙','秋冬法式气质长袖茶歇内搭长裙高级感加长高个子女生长款连衣裙','Photo\010014.png',168),
('010015','双面羊毛西裤','女生冬裤','小个子毛呢裤女裤子爆款2022新款秋冬加绒直筒双面羊毛西裤烟管裤','Photo\010015.png',168)
-------------------------------------------------------------------------------------------
--S表销售信息数据（订单号(主键)、商品号（C表外部键）、销售日期、销售数量、销售金额(总)）
insert into S(S_Order,S_No,S_Date,S_Num)
values
('A00001','010009','2022-9-1',50),
('A00002','010007','2022-9-2',30),
('A00003','010012','2022-9-2',70),
('A00004','010006','2022-9-7',40),
('A00005','010006','2022-9-15',50),
('A00006','010007','2022-10-7',100),
('A00007','010013','2022-10-11',15),
('A00008','010005','2022-10-14',50),
('A00009','010007','2022-10-22',70),
('A00010','010002','2022-10-27',25),
('A00011','010002','2022-11-2',40),
('A00012','010001','2022-11-5',50)
-------------------------------------------------------------------------------------------
--I表进货表信息数据(商品号、进货日期、库存量)
insert into I(I_Order,I_No,I_Date,I_Num)
values
('B00001','010001','2022-8-1',100),
('B00002','010002','2022-8-1',200),
('B00003','010003','2022-8-1',150),
('B00004','010004','2022-8-1',200),
('B00005','010005','2022-8-1',340),
('B00006','010006','2022-8-1',270),
('B00007','010007','2022-8-1',300),
('B00008','010008','2022-8-1',110),
('B00009','010009','2022-8-1',230),
('B00010','010010','2022-8-1',300),
('B00011','010011','2022-8-1',190),
('B00012','010012','2022-8-1',240),
('B00013','010013','2022-8-1',310),
('B00014','010014','2022-8-1',340),
('B00015','010015','2022-8-1',220)
exec Select_C null,null,null
exec Select_S null,'010001',null
exec Select_S null,null,null
exec Select_I null,null,null
exec Select_St null,null
exec Insert_S 'A00013','010001','2022-7-1',10
exec Delete_S 'A00013'
-------------------------------------------------------------------------------------------
select S_No 
from I,S where I_No = S.S_No and S.S_Date > I_Date 
group by S_No 
having SUM(I_Num) < SUM(S.S_NUM)

-----------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------

--创建视图
-------------------------------------------------------------------------------------------
--服装信息表视图C表（商品号、商品名、商品类型、商品描述、商品图片、销售价格）
create view C_main
as
select C_No 商品号,C_Name 商品名,C_Type 商品类型,C_Discribe 商品描述,C_Photo 商品图片,C_Price 销售价格_元
from C
go
-------------------------------------------------------------------------------------------
--销售信息表视图S表（订单号、商品号、销售日期、销售数量、销售(总)金额）
create view S_main
as
select S_Order 订单号,S_No 商品号,S_Date 销售日期,S_Num 销售数量_件,S_Price 销售总金额_元
from S
go
-------------------------------------------------------------------------------------------
--查询商品库存量St表（商品号、商品名、库存量)
create view St_main
as
select C_No 商品号,C_Name 商品名,St_Num 库存量_件
from C,St
where C_No = St.St_No
go
-------------------------------------------------------------------------------------------
--查询商品进货表I表（商品号、商品名、进货日期、进货量)
create view I_main
as
select I_Order 订单号, I_No 商品号,C_Name 商品名, I_Date 进货日期,I_Num 进货量_件
from I,C
where I.I_No = C.C_No
go
-------------------------------------------------------------------------------------------
--依照类型的月销售总量和销售金额报表(日期(年-月),月销售总量，月销售总金额)
create view Per_Month_Money
as
select convert(varchar(7),S.S_Date,120) 月份,C.C_Type 商品类型,SUM(S_Num) 月销售总量_件,SUM(S_Price) 月销售总金额_元
from S,C
where S.S_No = C.C_No
group by C.C_Type,convert(varchar(7),S.S_Date,120)
go

-------------------------------------------------------------------------------------------
--月热销榜(按件数降序)
create view Num_Hot
as
select top 100 S_No 商品号,convert(varchar(7),S.S_Date,120) 月份,SUM(S_Num) 销售数量_件,SUM(S_Price) 销售总金额_元
from S
group by S_No,convert(varchar(7),S.S_Date,120)
order by convert(varchar(7),S.S_Date,120),销售数量_件 desc
go
-------------------------------------------------------------------------------------------
--盈利榜(按钱数降序)
create view Money_Hot
as
select top 100 S_No 商品号,SUM(S_Num) 销售数量_件,SUM(S_Price) 销售总金额_元
from S
group by S_No
order by 销售总金额_元 desc
go

-----------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------

--创建存储过程
-------------------------------------------------------------------------------------------
--查询商品信息（依部分商品类型、完全商品号、部分商品名查询商品信息）
create procedure Select_C
(
	@no varchar(10),      --完全商品号
	@name nvarchar(20),   --部分商品名
	@type nvarchar(20)    --部分商品类型
)
as
begin
	declare @sql nvarchar(MAX) = '',@para nvarchar(MAX) = ''
	set @sql = 'select * from C_main where 1 = 1'
	set @para = '@no varchar(10),@name nvarchar(20),@type nvarchar(20)'

	if @no is not null
	begin
		set @sql = @sql + ' and 商品号 = @no'
	end
	if @name is not null
	begin
		set @sql = @sql + ' and 商品名 like ''%''+@name+''%'''
	end
	if @type is not null
	begin
		set @sql = @sql + ' and 商品类型 like ''%''+@type+''%''' 
	end
	exec sp_executesql @sql,@para,@no=@no,@name=@name,@type=@type
end
go
-------------------------------------------------------------------------------------------
--添加商品信息(商品号、商品名、商品类型(可填null)、商品描述(可填null)、商品图片、销售价格(单))(一条)
create procedure Insert_C
(
	@no varchar(10),
	@name nvarchar(20),
	@type nvarchar(20),
	@discribe nvarchar(60),
	@photo image,
	@price money
)
as
begin
	if @no in (select C_No from C)
	begin
		print '该商品已存在！'
	end
	else
	begin
		if @type is not null and @discribe is not null
		begin
			insert into C(C_No,C_Name,C_Type,C_Discribe,C_Photo,C_Price)
			values(@no,@name,@type,@discribe,@photo,@price)
		end
		if @type is not null and @discribe is null
		begin
			insert into C(C_No,C_Name,C_Discribe,C_Photo,C_Price)
			values(@no,@name,@discribe,@photo,@price)
		end
		if @type is null and @discribe is not null
		begin
			insert into C(C_No,C_Name,C_Type,C_Photo,C_Price)
			values(@no,@name,@type,@photo,@price)
		end
		if @type is null and @discribe is null
		begin
			insert into C(C_No,C_Name,C_Photo,C_Price)
			values(@no,@name,@photo,@price)
		end
		select * from C_main where 商品号 = @no
		select * from St_main where 商品号 = @no
	end
end
go
-------------------------------------------------------------------------------------------
--修改商品信息（依商品号，商品名、商品类型、商品描述、商品图片、销售价格(单)（显示S表总金额变化））(不能改商品号) 
create procedure Update_C
(
	@no varchar(10),
	@name nvarchar(20),
	@type nvarchar(20),
	@discribe nvarchar(60),
	@photo image,
	@price money
)
as
begin
	if @no not in (select C_No from C)
	begin
		print '没有此商品'
	end
	else
	begin
		if @name is not null
		begin
			update C
			set C_name = @name
			where C_No = @no
		end
		if @type is not null
		begin
			update C
			set C_Type = @type
			where C_No = @no
		end
		if @discribe is not null
		begin
			update C
			set C_Discribe = @discribe
			where C_No = @no
		end
		if @photo is not null
		begin
			update C
			set C_Photo = @photo
			where C_No = @no
		end
		if @price is not null
		begin
			select * from S_main where 商品号 = @no
			update C
			set C_Price = @price
			where C_No = @no
			select * from S_main where 商品号 = @no
		end
		select * from C_main where 商品号 = @no
	end
end
go
-------------------------------------------------------------------------------------------
--删除商品信息（依商品号）(实则删除商品库存量触动触发器)
create procedure Delete_C
(
	@no varchar(10)
)
as
begin
	if @no not in (select C_No from C)
	begin
		print '没有此商品'
	end
	else
	begin
		exec Select_No @no
		delete
		from St
		where St_No = @no
		exec Select_No @no
	end
end
go
-------------------------------------------------------------------------------------------
--查询销售信息（依完全订单号，完全商品号，日期的月份）  --(提示无结果查询)
create procedure Select_S
(
	@order varchar(10),
	@no varchar(10),       
	@date char(2)           
)
as
begin
	declare @sql nvarchar(MAX) = '',@para nvarchar(MAX) = ''
	set @sql = 'select * from S_main where 1 = 1'
	set @para = '@order varchar(10),@no varchar(10),@date char(2)'   
	if @order is not null
	begin
		set @sql = @sql + ' and 订单号 = @order'
	end
	if @no is not null
	begin
		set @sql = @sql + ' and 商品号 = @no'
	end
	if @date is not null
	begin
		set @sql = @sql + ' and convert(char(2),销售日期,101) = @date' 
	end
	exec sp_executesql @sql,@para,@order = @order,@no = @no,@date =@date
end
go
-------------------------------------------------------------------------------------------
--添加销售信息（订单号、商品号、销售日期、销售数量）  --（提示添加的商品号必须在C表中存在）
create procedure Insert_S
(
	@order varchar(10),               
	@no varchar(10),                  
	@date date,
	@num int
)
as
begin
	if @no not in (select C_No from C)
	begin
		print '添加的商品不存在!' 
	end
	else if @order in (select S_Order from S)
	begin
		print '订单号已存在!'
	end
	else
	begin
		select * from St_main where 商品号 = @no
		insert into S(S_Order,S_No,S_Date,S_Num)
		values(@order,@no,@date,@num)
		select * from S_main where 订单号 = @order
		select * from St_main where 商品号 = @no
	end
end
go

-------------------------------------------------------------------------------------------
--修改销售信息（依订单号，更新商品号、商品号销售日期、销售数量）(不能改订单号)（商品号必须存在）  --（提示添加的商品号必须在C表中存在）
create procedure Update_S
(
	@order varchar(10),               
	@no varchar(10),                  
	@date date,
	@num int
)
as
begin
	if @order not in (select S_Order from S)
	begin
		print '没有此订单号'
	end
	else
	begin
		if @no is not null
		begin
			if @no not in (select C_No from C)
			begin
				print '没有此商品号'
			end
			else
			begin
				select * from St_main where 商品号 = @no or 商品号 = (select S_No from S where S_Order = @order)
				update S
				set S_No = @no
				where S_Order = @order
				select * from St_main where 商品号 = @no or 商品号 = (select S_No from S where S_Order = @order)
			end
		end
		if @date is not null
		begin
			update S
			set S_Date = @date
			where S_Order = @order
		end
		if @num is not null
		begin
			select * from St_main where 商品号 = (select S_No from S where S_Order = @order)
			update S
			set S_Num = @num
			where S_Order = @order
			select * from St_main where 商品号 = (select S_No from S where S_Order = @order)
		end
		select * from S_main where 订单号 = @order
	end
end
go
-------------------------------------------------------------------------------------------
--删除销售信息（依订单号）
create procedure Delete_S
(
	@order varchar(10)
)
as
begin
	if @order not in (select S_Order from S)
	begin
		print '没有此订单号'
	end
	else
	begin
		select * from S_main where 订单号 = @order 
		delete 
		from S
		where S_Order = @order
		select * from S_main where 订单号 = @order 
	end
end
go
-------------------------------------------------------------------------------------------
--查询进货信息（依完全订单号，完全商品号，日期月份）
create procedure Select_I
(
	@order varchar(10),
	@no varchar(10),       
	@date char(2)           
)
as
begin
	declare @sql nvarchar(MAX) = '',@para nvarchar(MAX) = ''
	set @sql = 'select * from I_main where 1 = 1'
	set @para = '@order varchar(10),@no varchar(10),@date char(2)'   
	if @order is not null
	begin
		set @sql = @sql + ' and 订单号 = @order'
	end
	if @no is not null
	begin
		set @sql = @sql + ' and 商品号 = @no'
	end
	if @date is not null
	begin
		set @sql = @sql + ' and convert(char(2),进货日期,101) = @date' 
	end
	exec sp_executesql @sql,@para,@order = @order,@no = @no,@date =@date
end
go
-------------------------------------------------------------------------------------------
--添加进货信息（订单号、商品号、进货日期、销售数量）  --（提示添加的商品号必须在C表中存在）
create procedure Insert_I
(
	@order varchar(10),               
	@no varchar(10),                  
	@date date,
	@num int
)
as
begin
	if @no not in (select C_No from C)
	begin
		print '添加的商品不存在!' 
	end
	else if @order in (select I_Order from I)
	begin
		print '订单号已存在!'
	end
	else
	begin
		select * from St_main where 商品号 = @no
		insert into I(I_Order,I_No,I_Date,I_Num)
		values(@order,@no,@date,@num)
		select * from I_main where 订单号 = @order
		select * from St_main where 商品号 = @no
	end
end
go
-------------------------------------------------------------------------------------------
--修改进货信息（依订单号，更新商品号、进货日期、进货数量）(不能改订单号)（商品号必须存在）  --（提示添加的商品号必须在C表中存在）
create procedure Update_I
(
	@order varchar(10),               
	@no varchar(10),                  
	@date date,
	@num int
)
as
begin
	if @order not in (select I_Order from I)
	begin
		print '没有此订单号'
	end
	else
	begin
		if @no is not null
		begin
		if @no not in (select C_No from C)
			begin
				print '没有此商品号'
			end
			else
			begin
				select * from St_main where 商品号 = @no or 商品号 = (select I_No from I where I_Order = @order)
				update S
				set S_No = @no
				where S_Order = @order
				select * from St_main where 商品号 = @no or 商品号 = (select I_No from I where I_Order = @order)
			end
		end
		if @date is not null
		begin
			update S
			set S_Date = @date
			where S_Order = @order
		end
		if @num is not null
		begin
			select * from St_main where 商品号 = (select I_No from I where I_Order = @order)
			update S
			set S_Num = @num
			where S_Order = @order
			select * from St_main where 商品号 = (select I_No  from I where I_Order = @order)
		end
		select * from I_main where 订单号 = @order
	end
end
go
-------------------------------------------------------------------------------------------
--删除进货信息（依订单号）
create procedure Delete_I
(
	@order varchar(10)
)
as
begin
	if @order not in (select I_Order from I)
	begin
		print '没有此订单号'
	end
	else
	begin
		select * from I_main where 订单号 = @order 
		delete 
		from I
		where I_Order = @order
		select * from I_main where 订单号 = @order 
	end
end
go
-------------------------------------------------------------------------------------------
--查询一个商品的所有信息（进货销售信息,依照完全商品号）
create procedure Select_No
(
	@no varchar(10)
)  
as
begin
	if @no not in (select C_No from C)
	begin 
		print '没有此商品'
	end
	else
	begin
		select * from C_main where 商品号 = @no
		select * from I_main where 商品号 = @no
		select * from S_main where 商品号 = @no
		select * from St_main where 商品号 = @no
	end
end
go
-------------------------------------------------------------------------------------------
--查询存储量（依商品号和数量）（输出小于等于数量的商品号（如果有数量参数））
create procedure Select_St
(
	@no varchar(10),
	@num int
)
as
begin
	if @no is not null and @num is null
	begin
		select * from St_main
		where 商品号 = @no
	end
	if @no is null and @num is not null
	begin
		select * from St_main
		where 库存量_件 <= @num
	end
	if @no is not null and @num is not null
	begin
		select * from St_main
		where 商品号 = @no and 库存量_件 <= @num
	end
	if @no is null and @num is null
	begin
		select * from St_main
	end
end
go
-------------------------------------------------------------------------------------------
--全视图查看（输入视图名）
create procedure All_View
(
	@view varchar(20)
)
as
begin
	if @view = 'S'
	begin
		select * from S_main
	end
	if @view = 'C'
	begin
		select * from C_main
	end
	if @view = 'St'
	begin
		select * from St_main
	end
	if @view = 'I'
	begin
		select * from I_main
	end 
	if @view = 'Month'
	begin
		select * from Per_Month_Money
	end
	if @view = 'Hot'
	begin
		select * from Num_Hot
	end
	if @view = 'Money'
	begin
		select * from Money_Hot
	end
end
go

-----------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------

--创建触发器
-------------------------------------------------------------------------------------------
--插入基本销售信息后自动计算销售金额insert 更新销售数量的同时更新总金额update
create trigger S_count
on S
after insert,update
as
	update S
	set S_Price = S.S_Num * C.C_Price
	from C,inserted
	where S.S_No = C.C_No and S.S_Order = inserted.S_Order
go
-------------------------------------------------------------------------------------------
--更新销售单价的同时更新总金额update（多个数据操作）
create trigger C_count
on C
after update
as
	update S
	set S_Price = S.S_Num * C.C_Price
	from C,inserted
	where S.S_No = C.C_No
go
-------------------------------------------------------------------------------------------
--更改销售信息后自动减少库存量 insert,update,delete（约束使大于0）
create trigger St_Update_S
on S
after insert,update,delete
as
begin
	update St
	set St.St_Num = St.St_Num + deleted.S_Num
	from deleted  
	where St.St_No = deleted .S_No

	update St
	set St.St_Num = St.St_Num - inserted.S_Num
	from inserted
	where St.St_No = inserted.S_No
end
go
-------------------------------------------------------------------------------------------
--更新进货信息后自动增加库存 insert,update,delete（约束使大于0）
create trigger St_Update_I
on I
after insert,update,delete
as
begin
	update St
	set St.St_Num = St.St_Num - deleted.I_Num
	from deleted
	where St.St_No = deleted.I_No

	update St
	set St.St_Num = St.St_Num + inserted.I_Num
	from inserted
	where St.St_No = inserted.I_No
end
go
-------------------------------------------------------------------------------------------
--I表插入的商品号在St表中须存在
create trigger St_I_Insert
on I
after insert,update
as
begin
	if not exists (select C_No from C,inserted where C_No = inserted.I_No)
	begin
		print '商品号不存在，无法插入或修改!'
		rollback transaction
	end
end
go
-------------------------------------------------------------------------------------------
--销售日期销售的件数需要大于进货日期之前的所有件数之和
-------------------------------------------------------------------------------------------
--插入商品信息后存储表自动添加0个 insert
create trigger St_C_Insert
on C
after insert
as
begin
	insert into St(St_No)
	select C_No
	from inserted
end
go
-------------------------------------------------------------------------------------------
--删除库存量的同时删除商品信息、销售信息和进货信息 delete
create trigger St_C_Delete
on St
after delete
as
begin
	delete from S
	where S_No = (select St_No from deleted)
	delete from C
	where C_No = (select St_No from deleted)
	delete from I
	where I_No = (select St_No from deleted)
end
go

-----------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------

--直接执行操作
--------------------------------------------------------------------------------
--直接查看视图（商品信息、销售信息，库存量，每月同类型销售总额，每月热销(件)）
exec All_View 'C'
exec All_View 'S'
exec All_View 'St'
exec All_View 'I'
exec All_View 'Month'
exec All_View 'Hot'
exec All_View 'Money'
--------------------------------------------------------------------------------
--对C表操作
--查（完全商品号，部分商品名，部分商品类型）
exec Select_C null,null,null

exec Select_C '010006',null,null

exec Select_C null,'棉',null

exec Select_C null,'棉','男'

exec Select_C null,'女','秋'


--增
exec Select_C '010016',null,null
exec Select_C '010017',null,null
exec Select_St '010016',null
exec Select_St '010017',null

exec Insert_C '010015','男士运动短袖','男生夏裤','lululemon丨Metal Vent Tech 男士运动短袖 T 恤 2.0 LM3CX3S','Photo\010016.png',300

exec Insert_C '010016','男士运动短袖','男生夏裤','lululemon丨Metal Vent Tech 男士运动短袖 T 恤 2.0 LM3CX3S','Photo\010016.png',300


exec Insert_C '010017','百搭呢子大衣','女生冬衣','乌77高品质双面毛呢外套中长款翻领宽松慵懒风含腰带百搭呢子大衣','Photo\010017.png',169
--改（商品号匹配，修改商品名，商品类型，商品描述，商品图片，商品单价）
exec Select_C null,null,'男生'
exec Select_C '010001',null,null
exec Select_S null,'010001',null

exec Update_C '010001','男士运动短袖',null,'男士运动短袖 T 恤',null,null

exec Update_C '010002',null,null,null,null,128

exec Select_No '010111'

exec Update_C '011111',null,null,null,null,null

--删
exec Select_C '010007',null,null
exec Select_S null,'010007',null
exec Select_St '010007',null

exec Delete_C '010006'
exec Select_No '010006'

exec Delete_C '011111'
--------------------------------------------------------------------------------
--对S表操作
--查（完全订单号，完全商品名，部分月份）
exec Select_S null,null,null
exec Select_S NULL,'010006',null
exec Select_S 'A00004','010006',null
exec Select_S NULL,'010006','09'
--增
exec Select_S 'A00013',null,null
exec Select_S 'A00014',null,null
exec Select_S null,'010011',null
exec Select_St '010011',null

exec Insert_S 'A00001','010011','2022-11-10',60
exec Insert_S 'A00013','010111','2022-11-10',60
exec Insert_S 'A00013','010011','2022-11-10',60
exec Insert_S 'A00014','010011','2022-11-11',20
--改（订单号匹配，修改商品号，销售日期，销售件数）
exec Select_S null,'010011',null
exec Select_S 'A00014',null,null
exec Select_St '010011',null
exec Select_St '010006',null

exec Update_S 'A00014',null,null,80

exec Update_S 'A00002','010005',null,30

exec Update_S 'A00001','011111',null,null
--删
exec Select_S 'A00014',null,null
exec Select_St '010006',null

exec Delete_S 'A00010'
exec Delete_S 'A01111'
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--对I表操作
--查（完全订单号，完全商品名，部分月份）
exec Select_I null,null,null
exec Select_I NULL,'010006',null
exec Select_I 'B00004',null,null
exec Select_I NULL,'010006','08'
--增
exec Select_I 'A00013',null,null
exec Select_I 'A00014',null,null
exec Select_I null,'010011',null
exec Select_St '010011',null

exec Insert_I 'A00015','010010','2022-11-10',60
exec Insert_I 'A00013','010111','2022-11-10',60
exec Insert_I 'A00013','010011','2022-11-10',60
exec Insert_I 'A00014','010011','2022-11-11',20
--改（订单号匹配，修改商品号，销售日期，销售件数）
exec Select_I null,null,null
exec Select_I 'A00014',null,null
exec Select_St '010011',null
exec Select_St '010006',null

exec Update_I 'A00014',null,'2022-11-09',30

insert into i
values('B00016','011111','2022-11-09',30)

exec Update_I 'A00001','011111',null,null
--删
exec Select_I 'A00014',null,null
exec Select_St '010006',null

exec Delete_I 'B00001'
exec Delete_I 'A01111'
----------------------------------------------------------------------------
--对St表操作
--查（完全商品号，剩余件数小于等于）
exec Select_St null,null
exec Select_St '010009',null
exec Select_St null,200
exec Select_St '010005',300
--改（商品号匹配，修改库存量）
exec Select_St '010011',null

