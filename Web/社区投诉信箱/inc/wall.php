<?php
header('Content-type:text/html;charset=utf-8');
if (!isset($_SESSION['username'])) 
{ // 判断session是否存在
    echo "<script>alert('登录已过期！');window.location.href='../index.html';</script>"; // 弹出提示框并重定向
    exit(); // 终止脚本
}
?>

