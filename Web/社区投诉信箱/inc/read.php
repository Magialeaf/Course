<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
<?php 
if (session_status() == PHP_SESSION_NONE) 
{
    session_start();
}
date_default_timezone_set('Asia/Shanghai');
include("wall.php");
include('remove_xss.php');
?>


<?php
    $CID = remove_xss($_POST['CID']);
    $username = $_SESSION['username'];
    $identity = $_SESSION['identity'];
?>

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
if($flag == 1)
{
    try
    {
        // 获得日志信息，如果最新的那个在10分钟内未被访问，则添加日志
        $sql = "SELECT LID,LTime,LIP,LUsername,LEvent,LNote FROM log WHERE LUsername = :username and LEvent = '查看信件' and LNote= :cid order by LID desc";
        $res = $db->prepare($sql);
        $res->bindParam(':username', $username);
        $res->bindParam(':cid', $CID);
        $res->execute();
        $row = $res->fetch(PDO::FETCH_ASSOC);
        if($row > 0)
        {
            $current_time = new DateTime();
            $db_time = new DateTime($row['LTime']);
            $diff =  $current_time->getTimestamp() - $db_time->getTimestamp(); // 计算时间差
            $minutes = floor($diff / 60); // 转换为分钟数
            if($minutes > 10)
            {
                // 查看信件事件
                $time = date('Y-m-d H:i:s');
                $ip = $_SERVER['REMOTE_ADDR'];
                $event = '查看信件';
                $note = $CID;
                $sql = 'INSERT INTO log (LTime,LIP,LUsername,LEvent,LNote) VALUES (:time,:IP,:username,:event,:note)';
                $res = $db->prepare($sql);
                $res->bindParam(':time', $time, PDO::PARAM_STR);
                $res->bindParam(':IP', $ip);
                $res->bindParam(':username', $username);
                $res->bindParam(':event', $event);
                $res->bindParam(':note', $note);
                $res->execute();
            }
        }
        else
        {
            // 查看信件事件
            $time = date('Y-m-d H:i:s');
            $ip = $_SERVER['REMOTE_ADDR'];
            $event = '查看信件';
            $note = $CID;
            $sql = 'INSERT INTO log (LTime,LIP,LUsername,LEvent,LNote) VALUES (:time,:IP,:username,:event,:note)';
            $res = $db->prepare($sql);
            $res->bindParam(':time', $time, PDO::PARAM_STR);
            $res->bindParam(':IP', $ip);
            $res->bindParam(':username', $username);
            $res->bindParam(':event', $event);
            $res->bindParam(':note', $note);
            $res->execute();
        }
        // 获得信息
        $sql = "SELECT CName, CType, CTitle, CContent, CFinTime, CFinAdmin, CFinContent, CState FROM complaint WHERE CID = :cid";
        $res = $db->prepare($sql);
        $res->bindParam(':cid', $CID);
        $res->execute();
        $row = $res->fetch(PDO::FETCH_ASSOC);
    }
    catch( PDOException $e)
    {
        // $result = "数据获取失败！";
        // header("url={$_SERVER['HTTP_REFERER']}");
    }
}
?>

<?php echo '<link rel="stylesheet" href="css/read.css">' ?>
<div class="read_complaint">
    <h2 class="read_title">投诉详情</h2>
    <div class="user_request">
        <div class="show_border"><span class="read_tip">投诉标题：</span><span class="read_result"><?=remove_xss($row['CTitle'])?></span></div>
        <div class="show_border">
            <div class="one_row">
                <span class="read_tip">信件分类：</span><span class="read_result"><?=remove_xss($row['CType'])?></span>
            </div>
            <div class="one_row">
                <span class="read_tip">业主姓名：</span><span class="read_result"><?=remove_xss($row['CName'])?></span>
            </div>
        </div>
        <div class="show_border"><span class="read_tip">投诉内容：<br/></span><pre class="read_result"><?=remove_xss($row['CContent'])?></pre></div>
        <div class="show_border">
            <div class="one_row">
                <span class="read_tip">答复时间：</span><span class="read_result"><?=remove_xss($row['CFinTime'])?></span>
            </div>
            <div class="one_row">
                <span class="read_tip">管理姓名：</span><span class="read_result"><?=remove_xss($row['CFinAdmin'])?></span>
            </div>
        </div>
        <div class="show_border"><span class="read_tip">答复内容：</span><pre class="read_result"><?=remove_xss($row['CFinContent'])?></pre></div>
    </div>
</div>

<?php 
if($identity == "admin" && $row['CState'] == "处理中")
{
    echo "
    <form class='answer_form' action='inc/check_answer.php' method='POST' onsubmit='return check_answer();'>
        <textarea type='text' class='answer_content' id='answer_content' name='answer_content' placeholder='在此处回复业主'></textarea>
        <button type='submit'  class='answer' name='answer' value='${CID}'>确认回复</button>
    </form>
    ";
}
?>

<div class="footer">
    ©2023 社区住户在线投诉管理信息系统
</div>