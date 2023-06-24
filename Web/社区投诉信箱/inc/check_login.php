<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>

<?php
if (session_status() == PHP_SESSION_NONE) 
{
    session_start();
}
date_default_timezone_set('Asia/Shanghai');
include("remove_xss.php");

$flag = 1;
// 获得数据
if(remove_xss($_POST["login"]))
{
    $username = remove_xss($_POST["username"]);
    $password = remove_xss($_POST["password"]);
}
else
{
    header("Location:{$_SERVER['HTTP_REFERER']}?result=".urlencode('出错了！'));
    $flag = 0;
}

if($flag == 1)
{
    // 连接数据库
    $ip = "localhost";
    $database = "mysystem";
    $db_username = "root";
    $db_password = "root";
    try
    {
        $dsn="mysql:host=${ip};dbname=${database}";
        $db=new PDO($dsn,$db_username,$db_password);
        $db->query("set names utf8"); //设置字符编码
    }
    catch( PDOException $e)
    {
        header("Location:{$_SERVER['HTTP_REFERER']}?result=".urlencode('数据库连接错误！'));
        $flag = 0;
    }
}

if($flag == 1)
{
    // 验证信息
    try
    {
        $sql = "select UUsername,UPassword,UIdentity,UloginCount,UloginTime from user where UUsername = :username";
        $res = $db->prepare($sql);
        $res->bindParam(':username', $username);
        $res->execute();
        $row = $res->fetch(PDO::FETCH_ASSOC);
        if($row > 0)
        {
            $current_time = new DateTime();
            $db_time = new DateTime($row['UloginTime']);
            $diff = $db_time->getTimestamp() - $current_time->getTimestamp(); // 计算时间差
            $minutes = floor($diff / 60); // 转换为分钟数
            if($minutes > 0)
            {
                $result = "登入失败！账户已锁定，还剩${minutes}分钟";
                throw new Exception('');
            }
            else
            {
                if(password_verify($password,$row['UPassword']))
                {
                    $identity = $row['UIdentity'];
                }
                else
                {
                    $less = 4 - $row['UloginCount'];
                    $next = $row['UloginCount'] + 1;
                    if($less > 0)
                    {
                        $result = "登入失败！密码错误！还剩${less}次机会";
                        // 更新次数
                        $sql = "update user set UloginCount=:count where UUsername = :username";
                        $res = $db->prepare($sql);
                        $res->bindParam(':count', $next,PDO::PARAM_INT);
                        $res->bindParam(':username', $username);
                        $res->execute();
                        // 添加失败日志
                        $time = date('Y-m-d H:i:s');
                        $ip = $_SERVER['REMOTE_ADDR'];
                        $event = '用户登录';
                        $note = '失败';
                        $sql = 'INSERT INTO log (LTime,LIP,LUsername,LEvent,LNote) VALUES (:time,:IP,:username,:event,:note)';
                        $res = $db->prepare($sql);
                        $res->bindParam(':time', $time, PDO::PARAM_STR);
                        $res->bindParam(':IP', $ip);
                        $res->bindParam(':username', $username);
                        $res->bindParam(':event', $event);
                        $res->bindParam(':note', $note);
                        $res->execute();
                        throw new Exception('');
                    }
                    else
                    {
                        $result = "登入失败！账户已锁定，还剩30分钟";
                        $current_time = time(); // 获取当前时间戳
                        $future_time = strtotime('+30 minutes', $current_time); // 加上30分钟后的时间戳
                        $formatted_time = date('Y-m-d H:i:s', $future_time); // 格式化时间
                        // 更新次数和时间
                        $sql = "update user set UloginCount=:count,UloginTime=:time where UUsername = :username";
                        $res = $db->prepare($sql);
                        $res->bindParam(':count', $next,PDO::PARAM_INT);
                        $res->bindParam(':time', $formatted_time, PDO::PARAM_STR);
                        $res->bindParam(':username', $username);
                        $res->execute();
                        // 添加失败日志
                        $time = date('Y-m-d H:i:s');
                        $ip = $_SERVER['REMOTE_ADDR'];
                        $event = '用户登录';
                        $note = '失败';
                        $sql = 'INSERT INTO log (LTime,LIP,LUsername,LEvent,LNote) VALUES (:time,:IP,:username,:event,:note)';
                        $res = $db->prepare($sql);
                        $res->bindParam(':time', $time, PDO::PARAM_STR);
                        $res->bindParam(':IP', $ip);
                        $res->bindParam(':username', $username);
                        $res->bindParam(':event', $event);
                        $res->bindParam(':note', $note);
                        $res->execute();
                        throw new Exception('');
                    }
                }  
            }
        }
        else
        {
            $result = "登入失败！账号不存在！<br />";
            throw new Exception('');
        }
    }
    catch (Exception $e) 
    {     
        header("Location:{$_SERVER['HTTP_REFERER']}?result=".urlencode($result));
        $flag = 0;
    }   
}

if($flag == 1)
{
    // 设置cookie和session
    // setcookie('identity',$username.'|'.$identity,time() + 24*60*60*7);
    $_SESSION['username'] = $username;
    $_SESSION['identity'] = $identity;


    $time = date('Y-m-d H:i:s');
    $ip = $_SERVER['REMOTE_ADDR'];
    $event = '用户登录';
    $note = '成功';
    $next = 0;
    // 登录成功更新次数和下次登录时间
    $sql = "update user set UloginCount=:count,UloginTime=:time where UUsername = :username";
    $res = $db->prepare($sql);
    $res->bindParam(':count', $next,PDO::PARAM_INT);
    $res->bindParam(':time', $time, PDO::PARAM_STR);
    $res->bindParam(':username', $username);
    $res->execute();

    // 登录成功更新事件
    $sql = 'INSERT INTO log (LTime,LIP,LUsername,LEvent,LNote) VALUES (:time,:IP,:username,:event,:note)';
    $res = $db->prepare($sql);
    $res->bindParam(':time', $time, PDO::PARAM_STR);
    $res->bindParam(':IP', $ip);
    $res->bindParam(':username', $username);
    $res->bindParam(':event', $event);
    $res->bindParam(':note', $note);
    $res->execute();

    header("Location:../main.php");
}
?>

