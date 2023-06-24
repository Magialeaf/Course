<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
<?php 
if (session_status() == PHP_SESSION_NONE) 
{
    session_start();
}
if (!isset($_SESSION['username'])) 
{ // 判断session是否存在
    echo "<script>alert('登录已过期！');window.location.href='index.html';</script>"; // 弹出提示框并重定向
    exit(); // 终止脚本
}
?>

<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width">
    <title>社区住户投诉信箱</title>
    <link rel="stylesheet" href="css/main_page.css">
</head>
<body>
    <div class="container">
        <header class="header">
            <?php include("./inc/header.php");?>
        </header>
        <div class="content" id="content">
            <?php include("./inc/content.php");?>
        </div>
    </div>
    <script src="js/ajax_main.js" type="text/javascript" charset="UTF-8"></script>
    <script src="js/write_page.js" type="text/javascript" charset="UTF-8"></script>
    <script src="js/register_page.js" type="text/javascript" charset="UTF-8"></script>
</body>
</html>

