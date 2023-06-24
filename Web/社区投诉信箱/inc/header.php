<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
<?php 
if (session_status() == PHP_SESSION_NONE) 
{
    session_start();
}
include("wall.php");
$identity_en = $_SESSION['identity'];
if($identity_en == 'user')
{
    $identity = "普通业主";
}
else if($identity_en == 'admin')
{
    $identity = "管理员";
}
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
        if($_SESSION['identity'] == "user")
        {
            $col = 'HName';
            $sql = "SELECT ${col} FROM host WHERE HNo = (SELECT UID FROM user WHERE UUsername = :username)";
        }
        else if($_SESSION['identity'] == "admin")
        {
            $col = "AName";
            $sql = "SELECT ${col} FROM admin WHERE AID = (SELECT UID FROM user WHERE UUsername = :username)";
        }
        $res = $db->prepare($sql);
        $res->bindParam(':username', $_SESSION['username']);
        $res->execute();
        $row = $res->fetch(PDO::FETCH_ASSOC);
        if($row > 0)
        {
            $name = $row[$col];
        }
        else
        {
            $name = "姓名获取失败！";
        }
    }
    catch( PDOException $e)
    {
        $name = "姓名获取失败！";
    }
?>

<h1>社区住户投诉信箱</h1>
<dl class="top">
    <dt class="head_img" id="show-info-box">
        <div class="info-box" id="info-box">
            <span>姓名：<?=$name?></span>
            <span>账号：<?=$_SESSION['username']?></span>
        </div>
    </dt>
    <dt class="user_id" id="show-identity-box">
        <div class="user_id_content" id="user_id_content"><?=$identity?></div>
        <div class="identity-box" id="identity-box">
            <span class="identity-box-tip">身份</span>
            <?php
                if($identity_en == 'user')
                {
                    echo '<button id="switch_identity" value="user">普通业主</button>';
                }
                else if($identity_en == 'admin')
                {
                    echo '<button id="switch_identity" value="admin">管理员</button>';
                }
            ?>
        </div>
    </dt>
    <?php
    if($identity_en == 'admin')
    {
        echo '
        <dt><a id="community_complaints">社区投诉</a></dt>
        <dt><a id="read_log">日志查看</a></dt>
        ';
        // <dt><a id="register_admin">管理注册</a></dt>
    }
    else
    {
        echo '
        <dt><a id="personal_complaints">个人投诉</a></dt>
        <dt><a id="community_complaints">社区投诉</a></dt>
        <dt><a id="post_complaints">发布投诉</a></dt>';
    }
    ?>
</dl>