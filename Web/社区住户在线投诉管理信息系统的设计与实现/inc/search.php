<?php 
if (session_status() == PHP_SESSION_NONE) 
{
    session_start();
}
include("wall.php");
include("remove_xss.php");
?>


<?php 
    // 搜搜内容
    $target = addslashes(remove_xss($_POST['search_content']));
    // 目标标签
    $task = addslashes(remove_xss($_POST['task']));
    // 是否搜索
    $is_search = remove_xss($_POST['search']);
    // 用户名
    $username = addslashes(remove_xss($_POST['username']));
    // 身份
    $identity = remove_xss($_POST['identity']);
    // 当前页数
    $page = remove_xss($_POST['page']);
    // 是否点了别的标签
    $new_page = remove_xss($_POST['new_page']);
?>

<?php
    $flag = 1;
    try
    {
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
        $flag = 0;
    }
?>

<?php
// 查询的所有情况：
// 查询方式：1.带标签搜索查询 2.非搜索全标签 3.非搜索指定标签
// 身份：1.社区管理员（pubadmin） 2.社区个人（pubuser） 3.个人（priuser） 
if($flag == 1)
{
    if($identity == 'pubadmin')
    {
        $sql = "SELECT CID,CTitle,CPublic,CFinTime,CFinAdmin,CState from complaint where '1'";
    }
    else if($identity == 'priuser')
    {
        $sql = "SELECT CID, CTitle, CPublic, CFinTime, CFinAdmin, CState FROM complaint,host,user WHERE UID = HNo and HName = CName and UUsername = '${username}'";
    }
    else
    {
        $sql = "SELECT CID, CTitle, CPublic, CFinTime, CFinAdmin, CState FROM complaint,host,user WHERE UID = HNo and HName = CName and (CPublic = '公开' or (CPublic = '私密' and UUsername = '${username}'))";
    }

    if($task == 'all')
    { }
    else
    {
        $sql .= " and CType = '${task}'";
    }

    if($is_search == 'true')
    {
        $sql .= " and CONCAT(COALESCE(CID, ''), ' ', COALESCE(CTime, ''), ' ', COALESCE(CName, ''), ' ', COALESCE(CType, ''), ' ', COALESCE(CTitle, ''), ' ', COALESCE(CContent, ''), ' ', COALESCE(CPublic, ''), ' ', COALESCE(CFinTime, ''), ' ', COALESCE(CFinAdmin, ''), ' ', COALESCE(CFinContent, ''), ' ', COALESCE(CState, '')) LIKE '%{$target}%'";
    }

    if($new_page == 'true')
    {
        $res = $db->prepare($sql);
        $res->execute();
        $count = $res->rowCount(); // 获取行数
        $page = 1;
    }

    $start = ($page - 1) * 10;
    $sql .= "ORDER BY CID DESC LIMIT ${start},10";

    $res = $db->prepare($sql);
    $res->execute();
    $rows = $res->fetchAll(PDO::FETCH_ASSOC);

    foreach($rows as &$row)
    {
        foreach($row as &$value)
        {
            $value = remove_xss($value);
        }
    }

    echo json_encode(array("count" => $count, "rows" => $rows));
}
?>