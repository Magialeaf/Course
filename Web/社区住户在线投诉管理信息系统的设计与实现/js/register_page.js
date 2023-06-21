// 检测注册数据
function check_register() 
{
    var username = document.getElementById("username");
    var password = document.getElementById("password");
    var repassword = document.getElementById("repassword");
    var No = document.getElementById("No");
    var name = document.getElementById("name");
    var sex = document.getElementById("sex");
    var work = document.getElementById("work");
	var time = document.getElementById("time");
    var area = document.getElementById("area");
    var people = document.getElementById("people");
    var phone = document.getElementById("phone");
	
	if(username.value == "")
	{
		username.focus();
		username.placeholder = "请输入账号";
		return false;
	}
	else if( username.value.length < 6 || username.value.length > 12 )
	{
		username.focus();
		username.value = "";
		username.placeholder = "账号长度应在6-12之间！";
		return false;
	}
	else
	{
		var pattern = /^[a-zA-Z0-9]+$/;
		if (!pattern.test(username.value)) 
		{
			username.focus();
			username.value = "";
			username.placeholder = "账号只能由数字或字母组成！";
			return false;
		}
	}
	
	
	if( password.value == '')
	{
		password.focus();
		password.placeholder = "请输入密码！";
		return false;
	}
	else if(password.value.length < 6 || password.value.length > 18)
	{
		password.focus();
		password.value = "";
		password.placeholder = "密码格式长度应在6-18之间！";
		return false;
	}
	else
	{
		var pattern = /^(?=.*[a-z])(?=.*[A-Z])(?=.*\d)(?=.*[^\da-zA-Z]).+$/;
		if (!pattern.test(password.value)) 
		{
			password.focus();
			password.value = "";
			password.placeholder = "格式错误，应包含大小写字母，数字和特殊字符！";
			return false;
		}
	}
	
	
	if(repassword.value != password.value)
	{
		repassword.focus();
		repassword.value = "";
		repassword.placeholder = "两次输入的密码不一致！";
		return false;
	}
	
	if(No.value == "")
	{
		No.focus();
		No.placeholder = "请输入！";
		return false;
	}
	else if(No.value.length > 6)
	{
		No.focus();
		No.value = "";
		No.placeholder = "该项应小于等于6位";
		return false;
	}
	else		
	{
		var pattern = /^[0-9]+$/;
		if (!pattern.test(No.value))
		{
			No.focus();
			No.value = "";
			No.placeholder = "该项只能是数字！";
			return false;
		}
	}
	
	if(name.value == "")
	{		
		name.focus();
		name.placeholder = "请输入姓名";
		return false;
	}
	else if(name.value.length > 19)
	{
		name.focus();
		name.value="";
		name.placeholder = "名字长度应小于20位";
		return false;
	}
	
	if(work !== null)
	{
		if(work.value == "")
		{		
			work.focus();
			work.placeholder = "请输入职业";
			return false;
		}
		else if(work.value.length > 19)
		{
			work.focus();
			work.value="";
			work.placeholder = "职业长度应小于20位";
			return false;
		}
		
		var input_time = new Date(time.value);
		var now = new Date();
		if(time.value == "")
		{
			alert("请输入时间！");
			return false;
		}
		else if(input_time > now)
		{
			alert("输入的时间有误！");
			time.value == "";
			return false;
		}
		
		
		if(area.value == "")
		{
			area.focus();
			area.placeholder = "面积不能为空";
			return false;
		}
		else	
		{
			var pattern = /^[1-9]\d*(\.\d+)?$/;
			if (!pattern.test(area.value))
			{
				area.focus();
				area.value = "";
				area.placeholder = "面积格式错误！";
				return false;
			}
			else
			{
				if(area.value > 1000)
				{
					area.focus();
					area.value = "";
					area.placeholder = "面积不能大于1000！";
					return false;
				}
			}
		}
		
		
		if(people.value == "")
		{
			people.focus();
			people.placeholder = "人数不能为空";
			return false;
		}
		else
		{
			var pattern = /^[0-9]{1,2}$/;
			if (!pattern.test(people.value))
			{
				people.focus();
				people.value = "";
				people.placeholder = "人数必须小于100";
				return false;
			}
		}
	}
	
	
	if(phone.value == "")
	{
		phone.focus();
		phone.placeholder = "手机号不能为空";
		return false;
	}
	else
	{
		var pattern = /^[1-9]\d{10}$/;
		if (!pattern.test(phone.value))
		{
			phone.focus();
			phone.value = "";
			phone.placeholder = "手机号必须是11位整数！";
			return false;
		}
	}
	
	return true;
}