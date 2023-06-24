<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
<?php 
if (session_status() == PHP_SESSION_NONE) 
{
    session_start();
}
include("wall.php");
?>

<link rel="stylesheet" href="css/register_page.css">
<link rel="stylesheet" href="css/more_register.css">
<div class="login_page">
	<form class="login_form" action="inc/check_register.php" method="POST" onsubmit="return check_register();">
		<p>
			<input type="text" id="username" name="username" placeholder="账号" autocomplete="off"/>
		</p>			
		<p>
			<input type="password" id="password"  name="password" placeholder="密码" autocomplete="off"/>
		</p>
		<p>
			<input type="password" id="repassword" name="password" placeholder="确认密码" autocomplete="off"/>
		</p>
		<p>
			<input type="text" id="No" name="No" placeholder="工号" autocomplete="off"/>
		</p>	
		<p>
			<input type="text" id="name" name="name" placeholder="姓名" autocomplete="off"/>
		</p>	
		<p>
			性别：
			<label><input type="radio" name="sex" value="male" checked="checked">男</label>
			<label><input type="radio" name="sex" value="female">女</label>
		</p>	
		<p>
			<input type="text" id="phone" name="phone" placeholder="联系电话" autocomplete="off"/>
		</p>
		<p>
			<button type="submit" name="register" value="register_admin">注册</button>
		</p>
	</form>
</div>
<div class="footer">
    ©2023 社区住户在线投诉管理信息系统
</div>
