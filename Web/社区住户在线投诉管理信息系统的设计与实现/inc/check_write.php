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
    $name = remove_xss($_POST["name"]);
    $username = $_SESSION['username'];
    $phone = remove_xss($_POST["phone"]);
    $compaint_title = remove_xss($_POST["compaint_title"]);
    $task = remove_xss($_POST["task"]);
    $compaint_content = remove_xss($_POST["compaint_content"]);
    $isPublic = remove_xss($_POST["isPublic"]);
    try
    {
        // 数据处理
        if(mb_strlen($compaint_title, "UTF-8") == 0 || mb_strlen($compaint_title, "UTF-8") > 19)
        {
            throw new Exception('');
        }
        if(mb_strlen($compaint_content, "UTF-8") == 0 || mb_strlen($compaint_content, "UTF-8") > 199)
        {
            throw new Exception('');
        }
        if(!in_array($task, array('污染','治安','设施','纠纷','其他')))
        {
            $task = '其他';
        }
        if(!in_array($isPublic, array('公开','私密')))
        {
            $isPublic = '公开';
        }
    }
    catch( Exception $e)
    {
        $result = "数据错误，投诉失败！";
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

        // 先验证姓名和账号真实性
        $sql = "SELECT UUsername,HName FROM user u INNER JOIN host h ON HNo = UID WHERE UUsername = :username and HName = :name and HPhone = :phone";
        $res = $db->prepare($sql);
        $res->bindParam(':username', $username);
        $res->bindParam(':name', $name);
        $res->bindParam(':phone', $phone);
        $res->execute();
        $row = $res->fetch(PDO::FETCH_ASSOC);
        if($row > 0)
        {
            // 获得ID
            $current_time = date('Y-m-d H:i:s');
            $current_time_str = substr(str_replace('-', '', $current_time), 0, 8);
            $sql = "SELECT CID FROM complaint where CID like '${current_time_str}%'";
            $res = $db->prepare($sql);
            $res->execute();
            $row_count = $res->rowCount();
            $moreID = str_pad($row_count + 1, 3, '0', STR_PAD_LEFT); 
            $ID = $current_time_str.$moreID;
            $state = '处理中';
            // 插入数据
            $sql = "INSERT INTO complaint (CID,CTime,CName,CType,CTitle,CContent,CPublic,CState) VALUES (:ID,:Time,:Name,:Type,:Title,:Content,:Public,:State)";
            $res = $db->prepare($sql);
            $res->bindParam(':ID', $ID);
            $res->bindParam(':Time', $current_time,PDO::PARAM_STR);
            $res->bindParam(':Name', $name);
            $res->bindParam(':Type', $task);
            $res->bindParam(':Title', $compaint_title);
            $res->bindParam(':Content', $compaint_content);
            $res->bindParam(':Public', $isPublic);
            $res->bindParam(':State', $state);
            if(!$res->execute())
            {
                $result = "信息有误，投诉失败！";
                $flag = 0;
            }
        }
        else
        {
            $result = "个人信息有误，投诉失败！";
            $flag = 0;
        }
    }
    catch( Exception $e)
    {
        $result = "数据库错误，投诉失败！";
        $flag = 0;
    }
}
?>

<?php
if($flag == 1)
{
    $result = "投诉成功！";
    // 插入日志
    $time = date('Y-m-d H:i:s');
    $ip = $_SERVER['REMOTE_ADDR'];
    $event = '发布投诉';
    $sql = 'INSERT INTO log (LTime,LIP,LUsername,LEvent,LNote) VALUES (:time,:IP,:username,:event,:note)';
    $res = $db->prepare($sql);
    $res->bindParam(':time', $time, PDO::PARAM_STR);
    $res->bindParam(':IP', $ip);
    $res->bindParam(':username', $username);
    $res->bindParam(':event', $event);
    $res->bindParam(':note', $ID);
    $res->execute();
} 
header("Location:{$_SERVER['HTTP_REFERER']}?result=".urlencode($result));
?>