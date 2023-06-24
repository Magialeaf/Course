// user写投诉
function check_write()
{
    var title = document.getElementById("compaint_title");
    var content = document.getElementById("compaint_content");

	if(title.value == "")
	{		
		title.focus();
		title.placeholder = "请输入标题";
		return false;
	}
	else if(title.value.length > 19)
	{
		title.focus();
		title.value="";
		title.placeholder = "标题长度应小于20个字";
		return false;
	}

    if(content.value == "")
    {		
        content.focus();
        content.placeholder = "请输入内容";
        return false;
    }
    else if(content.value.length > 199)
    {
        content.focus();
        content.value="";
        content.placeholder = "内容长度应小于200个字";
        /*触发一次check后会触发最长检测（因为php中无法插入该函数所以在此处执行）*/
        rebound_max_comcontent();
        return false;
    }

    return true;
}

function rebound_max_comcontent()
{
    var content = document.getElementById("compaint_content");
    if(content != null)
    {
        const maxLength = 200; // 最大长度
        content.addEventListener('input', function() {
        if (content.value.length > maxLength) 
        {
            content.value = content.value.slice(0, maxLength); // 截取前200个字符
        }});
    }  
}

// admin回复投诉
function check_answer()
{
    var content = document.getElementById("answer_content");
    if(content.value == "")
    {		
        content.focus();
        content.placeholder = "请输入内容";
        return false;
    }
    else if(content.value.length > 199)
    {
        content.focus();
        content.value="";
        content.placeholder = "内容长度应小于200个字";
        /*触发一次check后会触发最长检测（因为php中无法插入该函数所以在此处执行）*/
        rebound_max_anscontent();
        return false;
    }

    return true;
}

function rebound_max_anscontent()
{
    var content = document.getElementById("answer_content");
    if(content != null)
    {
        const maxLength = 200; // 最大长度
        content.addEventListener('input', function() {
        if (content.value.length > maxLength) 
        {
            content.value = content.value.slice(0, maxLength); // 截取前200个字符
        }});
    }  
}

const urlParams = new URLSearchParams(window.location.search);
const loginStatus = urlParams.get('result');
if (loginStatus)
{
    // 在页面中显示内容
    const url = window.location.href.split('?')[0];
    window.history.replaceState(null, null, url);
    alert(loginStatus);
}
