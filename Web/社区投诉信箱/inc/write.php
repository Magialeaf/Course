<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
<?php 
if (session_status() == PHP_SESSION_NONE) 
{
    session_start();
}
include("wall.php");
?>

<?php
    try
    {
        $ip = "localhost";
        $database = "mysystem";
        $db_username = "root";
        $db_password = "root";
        $dsn="mysql:host=${ip};dbname=${database}";
        $db=new PDO($dsn,$db_username,$db_password);
        $db->query("set names utf8"); //设置字符编码

        $sql = "SELECT HPhone, HName FROM host WHERE HNo = (SELECT UID FROM user WHERE UUsername = :username)";
        $res = $db->prepare($sql);
        $res->bindParam(':username', $_SESSION['username']);
        $res->execute();
        $row = $res->fetch(PDO::FETCH_ASSOC);
        if($row > 0)
        {
            $name = $row['HName'];
            $phone = $row['HPhone'];
        }
        else
        {
            $name = "个人信息有误，无法获得信息！";
            $phone = "个人信息有误，无法获得信息！";
        }
    }
    catch( PDOException $e)
    {
        $name = "数据库错误，无法获得信息！";
        $phone = "数据库错误，无法获得信息！";
    }
?>

<?php echo '<link rel="stylesheet" href="css/write_page.css">' ?>
<h2 id="title">投诉</h2>
<form class="write_information" action="inc/check_write.php" method="POST" onsubmit="return check_write();">  
    <div class="person_information">
    <h3>个人信息</h3>
    <p>
        <label><span>*</span>姓名<input type="text" name="name" autocomplete="off" value="<?=$name?>" readonly/></label>
    </p>
    <p>
        <label><span>*</span>联系电话<input type="text" name="phone" value="<?=$phone?>" autocomplete="off" readonly/></label>
    </p>
    </div>
    <div class="complaint_information">
    <h3>投诉信息</h3>
    <p>
        <label><span>*</span>投诉标题<input type="text" name="compaint_title" id="compaint_title" autocomplete="off"/></label>
    </p>
    <p>
        <label><span>*</span>投诉分类</label>
        <input type="radio" id="pollution" name="task" value="污染" checked="checked" hidden>
        <label for="pollution">污染</label>
        <input type="radio" id="safety" name="task" value="治安" hidden>
        <label for="safety">治安</label>
        <input type="radio" id="facilities" name="task" value="设施" hidden>
        <label for="facilities">设施</label>
        <input type="radio" id="dispute" name="task" value="纠纷" hidden>
        <label for="dispute">纠纷</label>
        <input type="radio" id="other" name="task" value="其他" hidden>
        <label for="other">其他</label>
    </p>
    <p id="complaint_content">
        <label><span>*</span>投诉内容<textarea class="compaint_content" name="compaint_content" id="compaint_content" autocomplete="off" onload="rebound_max_content();"></textarea></label>
    </p>
    <p>
        <label><span>*</span>是否公开</label>
        <label class="isPublic_tip"><input type="radio" name="isPublic" value="公开" checked="checked">公开</label>
        <label class="isPublic_tip"><input type="radio" name="isPublic" value="私密">私密</label>  
    </p>
    </div>
    <div class="submit_compaint" >
        <button type="submit">提交</button>
    </div>
    <div class="footer">
    ©2023 社区住户在线投诉管理信息系统
    </div>
</form>
<?php 