<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>

<?php
if (session_status() == PHP_SESSION_NONE) 
{
    session_start();
}
date_default_timezone_set('Asia/Shanghai');
include("remove_xss.php");

$flag = 1;
// 信息判断
try
{
    if($_POST["register"])
    {
        $register = remove_xss($_POST["register"]);
        if($register == "register" || $register == "register_admin")
        {
            if($_POST["username"])
                $username = remove_xss($_POST["username"]);
            if($_POST["password"])
                $password = remove_xss($_POST["password"]);
            if($_POST["No"])
                $No = remove_xss($_POST["No"]);
            if($_POST["name"])
                $name = remove_xss($_POST["name"]);
            if($_POST["sex"])
                $sex = remove_xss($_POST["sex"]);
            if($_POST["phone"])
                $phone = remove_xss($_POST["phone"]);
                
            if (!preg_match('/^[a-zA-Z0-9]+$/', $username))
                throw new Exception('');
            if(!preg_match('/^(?=.*[a-z])(?=.*[A-Z])(?=.*\d)(?=.*[@$!%*?&])[A-Za-z\d@$!%*?&]{6,18}$/',$password))
                throw new Exception('');
            if(!preg_match('/^[0-9]{0,5}$/',$No))
                throw new Exception('');
            if(mb_strlen($name, "UTF-8")== 0 || mb_strlen($name, "UTF-8") > 19)
                throw new Exception('');
            if($sex != "male" && $sex != "female")
                throw new Exception('');
            if(!preg_match('/^[1-9]\d{10}$/',$phone))
                throw new Exception('');

            if($register == "register")
            {
                if($_POST["work"])
                    $work = remove_xss($_POST["work"]);
                if($_POST["time"])
                    $time = remove_xss($_POST["time"]);
                if($_POST["area"])
                    $area = remove_xss($_POST["area"]);
                if($_POST["people"])
                    $people = remove_xss($_POST["people"]);
                if(mb_strlen($work, "UTF-8") == 0 || mb_strlen($work, "UTF-8") > 19)
                    throw new Exception('');
                if(strtotime($time) === false)
                    throw new Exception('');
                if(is_float($area) && $area > 0 && $area < 1000)
                    throw new Exception('');
                if(is_int($people) && $people > 0 && $people < 100)
                    throw new Exception('');
            } 
        }
        else
        {
            throw new Exception('');
        } 
    }
    else
    {
        echo "错误！正在返回上一页面，请稍等...";
        header("refresh:3; url={$_SERVER['HTTP_REFERER']}");
        $flag = 0;
    }
}
catch (Exception $e) 
{
    echo "数据存在错误！正在返回上一页面，请稍等...";
    header("refresh:3; url={$_SERVER['HTTP_REFERER']}");
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
        echo "数据库连接错误！正在返回上一页面，请稍等...";
        header("refresh:3; url={$_SERVER['HTTP_REFERER']}");
        $flag = 0;
    }
}

if($flag == 1)
{
    // 存入信息（账号重复验证）
    try
    {
        // 设置计算强度，值越大计算时间越长，但越难破解
        $options = ['cost' => 12];
        $hash = password_hash($password, PASSWORD_BCRYPT, $options);
        
        $sql = "select UUsername,UPassword from user where UUsername= :username";
        $res = $db->prepare($sql);
        $res->bindParam(':username', $username);
        $res->execute();
        if ($res->rowCount() > 0) 
        {
            echo "账号已存在，注册失败！正在返回上一页面，请稍等...";
            header("refresh:3; url={$_SERVER['HTTP_REFERER']}");
            $flag = 0;
        }
        else
        {
            if($register == "register")
            {
                // host:HNo、HName、HSex、HWork、HTime、HArea、HPeople、HPhone。
                // user:UUsername、UPassword、UIdentity、UID。
                $identity = "user";
                // 插入数据到host表
                $sql = 'INSERT INTO host (HNo,HName,HSex,HWork,HTime,HArea,HPeople,HPhone) VALUES (:No, :name, :sex, :work, :time, :area, :people, :phone)';
                $stmt = $db->prepare($sql);
                // 绑定参数
                $date = strtotime($time);
                $date_formatted = date('Y-m-d', $date); 
                $stmt->bindParam(':No', $No);
                $stmt->bindParam(':name', $name);
                $stmt->bindParam(':sex', $sex);
                $stmt->bindParam(':phone', $phone);
                $stmt->bindParam(':work', $work);
                $stmt->bindParam(':time', $date_formatted, PDO::PARAM_STR);
                $stmt->bindParam(':area', $area, PDO::PARAM_STR);
                $stmt->bindParam(':people', $people, PDO::PARAM_INT);
                // 执行插入操作
                if(!$stmt->execute())
                {
                    echo "该房号业主已注册账号！<br/>";
                    throw new Exception('');
                }
   
                $count = 0;
                $time = date('Y-m-d H:i:s');
                // 插入数据到user表
                $sql = 'INSERT INTO user (UUsername, UPassword, UIdentity, UID, UloginCount, UloginTime) VALUES (:username, :password, :identity, :No, :count ,:time)';
                $stmt = $db->prepare($sql); 
                $stmt->bindParam(':username', $username);
                $stmt->bindParam(':password', $hash);
                $stmt->bindParam(':identity', $identity);
                $stmt->bindParam(':No', $No);
                $stmt->bindParam(':count', $count, PDO::PARAM_INT);
                $stmt->bindParam(':time', $time, PDO::PARAM_STR);
                if(!$stmt->execute())
                {
                    throw new Exception('');
                }
            }
            else if($register == "register_admin")
            {
                // admin:AID、AName、ASex、APhone。
                $identity = "admin";

                $sql = 'INSERT INTO admin (AID, AName, ASex, APhone) VALUES (:No, :name, :sex, :phone)';
                $stmt = $db->prepare($sql); 
                $stmt->bindParam(':No', $No);
                $stmt->bindParam(':name', $name);
                $stmt->bindParam(':sex', $sex);
                $stmt->bindParam(':phone', $phone);
                if(!$stmt->execute())
                {
                    throw new Exception('');
                }

                $count = 0;
                $time = date('Y-m-d H:i:s');
                // 插入数据到user表
                $sql = 'INSERT INTO user (UUsername, UPassword, UIdentity, UID, UloginCount, UloginTime) VALUES (:username, :password, :identity, :No, :count ,:time)';
                $stmt = $db->prepare($sql); 
                $stmt->bindParam(':username', $username);
                $stmt->bindParam(':password', $hash);
                $stmt->bindParam(':identity', $identity);
                $stmt->bindParam(':No', $No);
                $stmt->bindParam(':count', $count, PDO::PARAM_INT);
                $stmt->bindParam(':time', $time, PDO::PARAM_STR);
                if(!$stmt->execute())
                {
                    throw new Exception('');
                }
            }
            else
            {
                throw new Exception('');
            }
        }
    }
    catch (Exception $e) 
    {
        echo "注册失败！正在返回上一页面，请稍等...";
        header("refresh:3; url={$_SERVER['HTTP_REFERER']}");
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
    $event = '用户注册';
    $note = '成功';
    // 注册成功更新事件
    $sql = 'INSERT INTO log (LTime,LIP,LUsername,LEvent,LNote) VALUES (:time,:IP,:username,:event,:note)';
    $res = $db->prepare($sql);
    $res->bindParam(':time', $time, PDO::PARAM_STR);
    $res->bindParam(':IP', $ip);
    $res->bindParam(':username', $username);
    $res->bindParam(':event', $event);
    $res->bindParam(':note', $note);
    $res->execute();

    echo "注册成功！正在进入系统，请稍等...";
    header("refresh:3; url=../main.php");
}
?>