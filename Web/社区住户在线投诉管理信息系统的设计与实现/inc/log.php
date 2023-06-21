<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
<?php 
if (session_status() == PHP_SESSION_NONE) 
{
    session_start();
}
include("wall.php");
include("remove_xss.php");
$identity = $_SESSION['identity'];
$username = $_SESSION['username'];
$open  = remove_xss($_POST['open']);
?>

<?php
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
        header("url=#");
    }
?>

<?php 
if($identity == "admin")
{
    $sql = "SELECT DISTINCT LID,CASE WHEN UIdentity = 'user' THEN HName WHEN UIdentity = 'admin' THEN AName END AS Name, LTime,LIP,LUsername,LEvent,LNote 
            FROM log,host,user,admin 
            WHERE LUsername = UUsername and (UID = HNo or UID = AID)
            ORDER BY LID DESC";
    $res = $db->prepare($sql);
    $res->execute();
    $all_page = $res->rowCount();
}
?>

<link rel="stylesheet" href="css/more_log.css">
<h2 id="title">用户日志</h2>
<table class="result">
    <tr class="result_tip">
        <td class="log_username"><div>用户账号</div></td>
        <td class="log_name"><div>用户姓名</div></td>
        <td class="log_time"><div>访问时间</div></td>
        <td class="log_IP"><div>访问IP</div></td>
        <td class="log_event"><div>执行操作</div></td>
        <td class="log_note"><div>备注</div></td>
    </tr>

    <?php
        $i = 0;
        while($i < 10)
        {
            $row = $res->fetch(PDO::FETCH_ASSOC);
            if($row)   
            {
                $username = remove_xss($row['LUsername']);
                $name = remove_xss($row['Name']);
                $time = remove_xss($row['LTime']);
                $ip = remove_xss($row['LIP']);
                $event = remove_xss($row['LEvent']);
                $note = remove_xss($row['LNote']);
                
                echo "<tr class='result_value'>";
                echo "<td class='log_username'><span>${username}</span></td>
                <td class='log_name'><span>${name}</span></td>
                <td class='log_time'><span>${time}</span></td>
                <td class='log_TIP'><span>${ip}</span></td>
                <td class='log_event'><span>${event}</span></td>
                <td class='log_note'><span>${note}</span></td>
                </tr>";

                $i++;
            }
            else
            {
                break;
            }
        }
        if($i < 10)
        {
            for($j = $i;$j < 10;$j++)
                echo "<tr class='result_value'></tr>";
        }
        $page = 1;
        $all_page = intdiv($res->rowCount(), 10) + 1;
        ?>
</table>
<div class="turn_page">
    <button id="turn_left_log" class="turn_left" value="turn_left" onclick="turn_log('left');">上一页</button>
    <button id="turn_right_log" class="turn_right" value="turn_right" onclick="turn_log('right');">下一页</button>
    <div class="all_pages">总页数：<span class="page_value" id="page_value_all_log"><?=$all_page;?></span>&nbsp;当前页数：<span class="page_value" id="page_value_log"><?=$page?></span></div>
</div>
<div class="footer">
    ©2023 社区住户在线投诉管理信息系统
</div>
