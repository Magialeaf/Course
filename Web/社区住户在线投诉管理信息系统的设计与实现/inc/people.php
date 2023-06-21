<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
<?php 
if (session_status() == PHP_SESSION_NONE) 
{
    session_start();
}
include("wall.php");
$identity = $_SESSION['identity'];
$username = $_SESSION['username'];
?>

<link rel="stylesheet" href="css/people.css">

<?php
// 连接数据库
    try
    {
        $flag = 1;
        $ip = "localhost";
        $database = "mysystem";
        $db_username = "root";
        $db_password = "root";
        $dsn="mysql:host=${ip};dbname=${database}";
        $db=new PDO($dsn,$db_username,$db_password);
        $db->query("set names utf8"); //设置字符编码
    }
    catch( PDOException $e)
    {
        // $result = "连接数据库失败！";
        // header("url={$_SERVER['HTTP_REFERER']}");
        $flag = 0;
    }
?>


<?php
if($identity == 'admin' && $flag == 1)
{
    $sql = "SELECT AID,AName,ASex,APhone FROM user,admin where UID = AID and UUsername = :username";
    $res = $db->prepare($sql);
    $res->bindParam(':username', $username);
    $res->execute();
    $row = $res->fetch(PDO::FETCH_ASSOC);

    echo "
    <div class='information'>
        <p>账号：<span>${username}</p>			
        <p>工号：<span>${row['AID']}</span></p>
        <p>姓名：<span>${row['AName']}</span></p>";
    if($row['ASex'] == 'male')
    {
        echo "<p>性别：<span>男</span></p>";
    }
    else
    {
        echo "<p>性别：<span>女</span></p>";
    }
    echo "
        <p>联系电话：<span>${row['APhone']}</span></p>
    <div class='information'>
    ";
}
?>

<?php
if($identity == 'user' && $flag == 1)
{

    $sql = "SELECT HNo,HName,HSex,HWork,HTime,HArea,HPeople,HPhone FROM user,host where UID = HNo and UUsername = :username";
    $res = $db->prepare($sql);
    $res->bindParam(':username', $username);
    $res->execute();
    $row = $res->fetch(PDO::FETCH_ASSOC);

    echo "
    <div class='information'>
        <p>账号：<span>${username}</p>			
        <p>房号：<span>${row['HNo']}</span></p>
        <p>姓名：<span>${row['HName']}</span></p>";
    if($row['HSex'] == 'male')
    {
        echo "<p>性别：<span>男</span></p>";
    }
    else
    {
        echo "<p>性别：<span>女</span></p>";
    }
    echo "
        <p>职业：<span>${row['HWork']}</span></p>	
        <p>入住时间：<span>${row['HTime']}</span></p>
        <p>住房面积(单位平方米)：<span>${row['HArea']}</span></p>
        <p>常驻人口：<span>${row['HPeople']}</span></p>
        <p>联系电话：<span>${row['HPhone']}</span></p>
    <div class='information'>
    ";

}
?>

<a class="exit" href="inc/exit.php">退出登录</a>



