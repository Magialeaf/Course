<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
<?php 
if (session_status() == PHP_SESSION_NONE) 
{
    session_start();
}
date_default_timezone_set('Asia/Shanghai');
include("wall.php");
include("remove_xss.php");
?>

<?php
    $flag = 1;
    $username = $_SESSION['username'];
    $answer = remove_xss($_POST['answer']);
    $answer_content = remove_xss($_POST['answer_content']);
    try
    {
        // 数据处理
        if(mb_strlen( $answer_content, "UTF-8") == 0 || mb_strlen( $answer_content, "UTF-8") > 199)
        {
            throw new Exception('');
        }
    }
    catch( Exception $e)
    {
        $result = "数据错误，回复失败！";
        $flag = 0;
    }
?>


<?php
if($flag == 1)
{
    try
    {
        $ip = "localhost";
        $database = "mysystem";
        $db_username = "root";
        $db_password = "root";
        $dsn="mysql:host=${ip};dbname=${database}";
        $db=new PDO($dsn,$db_username,$db_password);
        $db->query("set names utf8"); //设置字符编码

        // 先获取管理员信息
        $sql = "SELECT AName FROM user,admin WHERE AID = UID and UUsername = :username";
        $res = $db->prepare($sql);
        $res->bindParam(':username', $username);
        $res->execute();
        $row = $res->fetch(PDO::FETCH_ASSOC);

        // 写入回复信息
        if($row > 0)
        {
            $admin_name = $row['AName'];
            // 获得ID
            
            $current_time = date('Y-m-d H:i:s');
            $state = '已回复';

            // 插入数据
            $sql = "UPDATE complaint SET CFinTime=:Time, CFinAdmin=:Name, CFinContent=:Content, CState=:State WHERE CID=:ID";
            $res = $db->prepare($sql);
            $res->bindParam(':ID', $answer);
            $res->bindParam(':Time', $current_time,PDO::PARAM_STR);
            $res->bindParam(':Name', $admin_name);
            $res->bindParam(':Content', $answer_content);
            $res->bindParam(':State', $state);
            if(!$res->execute())
            {
                $result = "信息有误，回复失败！";
                $flag = 0;
            }
        }
        else
        {
            $result = "管理员信息有误，回复失败！";
            $flag = 0;
        }
    }
    catch( Exception $e)
    {
        $result = "数据库错误，回复失败！";
        $flag = 0;
    }
}
?>

<?php
if($flag == 1)
{
    $result = "回复成功！";
    // 插入日志
    $time = date('Y-m-d H:i:s');
    $ip = $_SERVER['REMOTE_ADDR'];
    $event = '回复投诉';
    $sql = 'INSERT INTO log (LTime,LIP,LUsername,LEvent,LNote) VALUES (:time,:IP,:username,:event,:note)';
    $res = $db->prepare($sql);
    $res->bindParam(':time', $time, PDO::PARAM_STR);
    $res->bindParam(':IP', $ip);
    $res->bindParam(':username', $username);
    $res->bindParam(':event', $event);
    $res->bindParam(':note', $answer);
    $res->execute();
} 
header("Location:{$_SERVER['HTTP_REFERER']}?result=".urlencode($result));
?>