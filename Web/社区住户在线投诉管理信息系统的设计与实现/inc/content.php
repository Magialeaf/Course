<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
<?php 
if (session_status() == PHP_SESSION_NONE) 
{
    session_start();
}
include("wall.php");
include("remove_xss.php");

if($_POST['identity'])
{
    $_SESSION['identity'] = remove_xss($_POST['identity']);
    $identity = $_SESSION['identity'];
}
else
{
    $identity = $_SESSION['identity'];
}
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
if($open == "个人投诉")
{
    $sql = "SELECT CID, CTitle, CPublic, CFinTime, CFinAdmin, CState FROM complaint,host,user WHERE UID = HNo and HName = CName and UUsername = ? ORDER BY CID DESC";
    $params[] = $username;
    $con_identity = "pri";
}
else
{
    if($identity == "admin")
    {
        $sql = "SELECT CID, CTitle, CPublic, CFinTime, CFinAdmin, CState FROM complaint ORDER BY CID DESC";
    }
    else
    {
        $sql = "SELECT CID, CTitle, CPublic, CFinTime, CFinAdmin, CState FROM complaint,host,user WHERE UID = HNo and HName = CName and (CPublic = '公开' or (CPublic = '私密' and UUsername = ?)) ORDER BY CID DESC";
        $params[] = $username;
    }
    $con_identity = "pub";
}
$res = $db->prepare($sql);
$res->execute($params);
$con_identity .= $identity;
?>

<h2 id="title">社区投诉</h2>
<div class="select_sql">
    <form class="select_form" method="POST" onsubmit="return false;">
        <p>
            <input type="text" name="search_content" id="search_content" placeholder="输入搜索内容" autocomplete="off"/>
            <button type="button" name="search_btn" id="search_btn" onclick="search_page(true,'<?=$username?>','<?=$con_identity?>',true);">搜索</button>			
        </p>
        <dl class="classification">
            <dt class="task_list">事务分类：</dt>
            <input type="radio" id="all" name="task" value="all" checked="checked" hidden onclick="search_page(false,'<?=$username?>','<?=$con_identity?>',true);">
            <label for="all">全部</label>
            <input type="radio" id="pollution" name="task" value="污染" hidden onclick="search_page(false,'<?=$username?>','<?=$con_identity?>',true);">
            <label for="pollution">污染</label>
            <input type="radio" id="safety" name="task" value="治安" hidden onclick="search_page(false,'<?=$username?>','<?=$con_identity?>',true);">
            <label for="safety">治安</label>
            <input type="radio" id="facilities" name="task" value="设施" hidden onclick="search_page(false,'<?=$username?>','<?=$con_identity?>',true);">
            <label for="facilities">设施</label>
            <input type="radio" id="dispute" name="task" value="纠纷" hidden onclick="search_page(false,'<?=$username?>','<?=$con_identity?>',true);">
            <label for="dispute">纠纷</label>
            <input type="radio" id="other" name="task" value="其他" hidden onclick="search_page(false,'<?=$username?>','<?=$con_identity?>',true);">
            <label for="other">其他</label>
        </dl>
    </form>
    <button id="write_complaints" class="write">我要投诉</button>
</div>
<table class="result">
    <tr class="result_tip">
        <td class="option"><div>操作</div></td>
        <td class="number"><div>信件编号</div></td>
        <td class="title"><div>信件标题</div></td>
        <td class="staff"><div>回复人员</div></td>
        <td class="time"><div>回复时间</div></td>
        <td class="state"><div>处理状态</div></td>
    </tr>
    <?php
        $i = 0;
        while($i < 10)
        {
            $row = $res->fetch(PDO::FETCH_ASSOC);
            if($row)   
            {
                echo "<tr class='result_value'>";
                if($row['CPublic'] == '私密')
                {
                    echo "<td class='option'><button id='read_letter' value='${row['CID']}'>私密</button></td>";
                    echo "<style>#read_letter[value='${row['CID']}']{ color: red;}</style> ";         
                }
                else
                {
                    echo "<td class='option'><button id='read_letter' value='${row['CID']}'>详情</button></td>";
                }
                echo "
                <td class='number'><span>${row['CID']}</span></td>
                <td class='title'><span>${row['CTitle']}</span></td>
                <td class='staff'><span>${row['CFinAdmin']}</span></td>
                <td class='time'><span>${row['CFinTime']}</span></td>
                <td class='state'><span>${row['CState']}</span></td>
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
    <button id="turn_left" class="turn_left" value="turn_left" onclick="turn('left','<?=$username?>','<?=$con_identity?>');">上一页</button>
    <button id="turn_right" class="turn_right" value="turn_right" onclick="turn('right','<?=$username?>','<?=$con_identity?>');">下一页</button>
    <div class="all_pages">总页数：<span class="page_value" id="page_value_all"><?=$all_page;?></span>&nbsp;当前页数：<span class="page_value" id="page_value"><?=$page?></span></div>
</div>
<div class="footer">
    ©2023 社区住户在线投诉管理信息系统
</div>
