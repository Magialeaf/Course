<?php 
if (session_status() == PHP_SESSION_NONE) 
{
    session_start();
}
include("wall.php");
include("remove_xss.php");
?>


<?php 
    // 目标页数
    $page_value = remove_xss($_POST['page_value']);
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
if($flag == 1)
{
    $start = ($page_value - 1) * 10;

    $sql = "SELECT DISTINCT LID,CASE WHEN UIdentity = 'user' THEN HName WHEN UIdentity = 'admin' THEN AName END AS Name, LTime,LIP,LUsername,LEvent,LNote 
    FROM log,host,user,admin 
    WHERE LUsername = UUsername and (UID = HNo or UID = AID)
    ORDER BY LID DESC LIMIT :start,10";
    $res = $db->prepare($sql);
    $res->bindParam(':start',$start,PDO::PARAM_INT);
    $res->execute();
    $rows = $res->fetchAll(PDO::FETCH_ASSOC);

    foreach($rows as &$row)
    {
        foreach($row as &$value)
        {
            $value = remove_xss($value);
        }
    }
    
    echo json_encode(array("rows" => $rows));
}
?>