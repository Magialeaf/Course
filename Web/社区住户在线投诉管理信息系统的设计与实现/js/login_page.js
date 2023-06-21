// 跳转到注册页面
function jump_register() 
{
	window.location.href="./register.html";
}

// 检测登录数据
function check_login() 
{
	var username = document.getElementById("username").value;
	var password = document.getElementById("password").value;

	if( username == '')
	{
		document.getElementById("res_tip").innerHTML = "请输入账号！";
		document.getElementById("username").focus();
		return false;
	}
	else if( username.length < 6 || username.length > 12 )
	{
		document.getElementById("res_tip").innerHTML = "账号长度应在6-12之间！";
		document.getElementById("username").focus();
		return false;
	}
	else
	{
		var pattern = /^[a-zA-Z0-9]+$/;
		if (!pattern.test(username)) 
		{
			document.getElementById("res_tip").innerHTML = "账号只能由数字或字母组成！";
			document.getElementById("username").focus();
			return false;
		}
	}

	if( password == '')
	{
		document.getElementById("res_tip").innerHTML = "请输入密码！";
		document.getElementById("password").focus();
		return false;
	}
	else if(password.length < 6 || password.length > 18)
	{
		document.getElementById("res_tip").innerHTML = "密码格式长度应在6-18之间！";
		document.getElementById("password").focus();
		return false;
	}
	else
	{
		var pattern = /^(?=.*[a-z])(?=.*[A-Z])(?=.*\d)(?=.*[^\da-zA-Z]).+$/;
		if (!pattern.test(password)) 
		{
			document.getElementById("res_tip").innerHTML = "密码格式错误，应该包含大小写字母，数字和特殊字符！";
			document.getElementById("password").focus();
			return false;
		}
	}

	return true;
}

const urlParams = new URLSearchParams(window.location.search);
const loginStatus = urlParams.get('result');
if (loginStatus)
{
	// 在页面中显示内容
	const url = window.location.href.split('?')[0];
	window.history.replaceState(null, null, url);
	const resTip = document.getElementById('res_tip');
	resTip.innerHTML = loginStatus;
}


{/* <script>
// 检查是否存在cookie
if (document.cookie.indexOf('username') !== -1) {
	// 自动填充用户名和密码
	var username = getCookie('username');
	var password = getCookie('password');
	document.getElementsByName('username')[0].value = username;
	document.getElementsByName('password')[0].value = password;
}

// 获取cookie
function getCookie(name) {
	var cookies = document.cookie.split(';');
	for (var i = 0; i < cookies.length; i++) {
		var cookie = cookies[i].trim();
		if (cookie.indexOf(name) === 0) {
			return cookie.substring(name.length + 1);
		}
	}
	return '';
}
</script> */}