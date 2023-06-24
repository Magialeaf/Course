// 我要投诉，每次都重新绑定
var rebound_want_compaint = function()
{
    // 切换到我要投诉界面
    var write_complaints = document.getElementById("write_complaints");
    var post_complaints = document.getElementById("post_complaints");
    var I_want_compaint = function ()
    {
        // 创建 AJAX 请求对象
        var xhr = new XMLHttpRequest();
        // 发送 AJAX 请求
        xhr.open("GET", "inc/write.php", true);
        xhr.send();
        // 处理 AJAX 响应
        xhr.onreadystatechange = function() {
            if (xhr.readyState == 4 && xhr.status == 200) {
                // 获取content元素
                var content = document.querySelector(".content");
                // 清空content元素的内容
                content.innerHTML = "";
                // 将返回的 HTML 内容插入到 content 元素中
                content.innerHTML = xhr.responseText;
                // 对顶栏高亮进行设置
                const links = document.querySelectorAll('.top dt a');
                links.forEach(link => {link.style.color = '#bec3de';});
                var now_page = document.getElementById("post_complaints");
                if(now_page != null)
                {
                    now_page.style.color = "white";
                }           
            }
        };
        // 处理 AJAX 请求错误
        xhr.onerror = function() {
            alert("网络错误！请检查网络配置！");
        }
    }
    write_complaints.addEventListener("click", I_want_compaint);
    if(post_complaints != null)
    {
        post_complaints.addEventListener("click", I_want_compaint);
    }
}

rebound_want_compaint();

// 信件详情和下一页，每次重新绑定
var rebound_want_read_letter = function()
{
    // 切换到我要投诉界面
    var read_letters = document.querySelectorAll("#read_letter");
    var I_want_read_letter = function ()
    {
        var value = this.value;
        // 创建 AJAX 请求对象
        var xhr = new XMLHttpRequest();
        // 发送 AJAX 请求
        xhr.open("POST", "inc/read.php", true);
        xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
        xhr.send("CID="+value);
        // 处理 AJAX 响应
        xhr.onreadystatechange = function() {
            if (xhr.readyState == 4 && xhr.status == 200) {
                // 获取content元素
                var content = document.querySelector(".content");
                // 清空content元素的内容
                content.innerHTML = "";
                // 将返回的 HTML 内容插入到 content 元素中
                content.innerHTML = xhr.responseText;     
            }
        };
        // 处理 AJAX 请求错误
        xhr.onerror = function() {
            alert("网络错误！请检查网络配置！");
        }
    }
    if(read_letters != null)
    {
        read_letters.forEach(function(read_letter)
        {
            read_letter.addEventListener("click", I_want_read_letter);
        });
    }
}

rebound_want_read_letter();

// 切换到个人投诉界面
var personal_complaints= document.getElementById("personal_complaints");
if(personal_complaints != null)
{
    personal_complaints.addEventListener("click", function()
    {
        // 创建 AJAX 请求对象
        var xhr = new XMLHttpRequest();
        // 发送 AJAX 请求
        xhr.open("POST", "inc/content.php", true);
        xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
        xhr.send("open=" + "个人投诉");
        // 处理 AJAX 响应
        xhr.onreadystatechange = function() {
            if (xhr.readyState == 4 && xhr.status == 200) {
                // 获取content元素
                var content = document.querySelector(".content");
                // 清空content元素的内容
                content.innerHTML = "";
                // 将返回的 HTML 内容插入到 content 元素中
                content.innerHTML = xhr.responseText;
                // 对顶栏高亮进行设置
                var title = document.getElementById("title");
                title.innerHTML = "我的投诉";
                const links = document.querySelectorAll('.top dt a');
                links.forEach(link => {link.style.color = '#bec3de';});
                var now_page = document.getElementById("personal_complaints");
                now_page.style.color = "white";
                rebound_want_compaint();
                rebound_want_read_letter();
            }
        };
        // 处理 AJAX 请求错误
        xhr.onerror = function() {
            alert("网络错误！请检查网络配置！");
        }
    });
}

// 切换到社区投诉界面
var community_complaints= document.getElementById("community_complaints");
community_complaints.addEventListener("click", function()
{
    // 创建 AJAX 请求对象
    var xhr = new XMLHttpRequest();
    // 发送 AJAX 请求
    xhr.open("POST", "inc/content.php", true);
    xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
    xhr.send("open=" + "社区投诉");
    // 处理 AJAX 响应
    xhr.onreadystatechange = function() {
        if (xhr.readyState == 4 && xhr.status == 200) {
            // 获取content元素
            var content = document.querySelector(".content");
            // 清空content元素的内容
            content.innerHTML = "";
            // 将返回的 HTML 内容插入到 content 元素中
            content.innerHTML = xhr.responseText;
            // 对顶栏高亮进行设置
            var title = document.getElementById("title");
            title.innerHTML = "社区投诉";
            const links = document.querySelectorAll('.top dt a');
            links.forEach(link => {link.style.color = '#bec3de';});
            var now_page = document.getElementById("community_complaints");
            now_page.style.color = "white";
            rebound_want_compaint();
            rebound_want_read_letter();
        }
    };
    // 处理 AJAX 请求错误
    xhr.onerror = function() {
        alert("网络错误！请检查网络配置！");
    }
});

// 切换到管理员注册界面
var register_admin = document.getElementById("register_admin");
if(register_admin != null)
{
    register_admin.addEventListener("click", function()
    {
        // 创建 AJAX 请求对象
        var xhr = new XMLHttpRequest();
        // 发送 AJAX 请求
        xhr.open("GET", "inc/register_admin.php", true);
        xhr.send();
        // 处理 AJAX 响应
        xhr.onreadystatechange = function() {
            if (xhr.readyState == 4 && xhr.status == 200) {
                // 获取content元素
                var content = document.querySelector(".content");
                // 清空content元素的内容
                content.innerHTML = "";
                // 将返回的 HTML 内容插入到 content 元素中
                content.innerHTML = xhr.responseText;
                // 对顶栏高亮进行设置
                const links = document.querySelectorAll('.top dt a');
                links.forEach(link => {link.style.color = '#bec3de';});
                var now_page = document.getElementById("register_admin");
                now_page.style.color = "white";
            }
        };
        // 处理 AJAX 请求错误
        xhr.onerror = function() {
            alert("网络错误！请检查网络配置！");
        }
    });   
}

// 切换到日志查看界面
var read_log= document.getElementById("read_log");
if(read_log != null)
{
    read_log.addEventListener("click", function()
    {
        // 创建 AJAX 请求对象
        var xhr = new XMLHttpRequest();
        // 发送 AJAX 请求
        xhr.open("GET", "inc/log.php", true);
        xhr.send();
        // 处理 AJAX 响应
        xhr.onreadystatechange = function() {
            if (xhr.readyState == 4 && xhr.status == 200) {
                // 获取content元素
                var content = document.querySelector(".content");
                // 清空content元素的内容
                content.innerHTML = "";
                // 将返回的 HTML 内容插入到 content 元素中
                content.innerHTML = xhr.responseText;
                // 对顶栏高亮进行设置
                const links = document.querySelectorAll('.top dt a');
                links.forEach(link => {link.style.color = '#bec3de';});
                var now_page = document.getElementById("read_log");
                now_page.style.color = "white";
            }
        };
        // 处理 AJAX 请求错误
        xhr.onerror = function() {
            alert("网络错误！请检查网络配置！");
        }
    });   
}

// 点击和移出身份框
var user_id_content = document.getElementById("user_id_content");
var identity_box = document.getElementById("identity-box");
user_id_content.addEventListener('click', function() 
{
    if (identity_box.style.display === 'block') 
    {
        identity_box.style.display = 'none';
    } 
    else 
    {
        identity_box.style.display = 'block';
    }
});

// 点击头像框
var show_info_box = document.getElementById("show-info-box");
show_info_box.addEventListener("click", function()
{
    // 创建 AJAX 请求对象
    var xhr = new XMLHttpRequest();
    // 发送 AJAX 请求
    xhr.open("GET", "inc/people.php", true);
    xhr.send();
    // 处理 AJAX 响应
    xhr.onreadystatechange = function() {
        if (xhr.readyState == 4 && xhr.status == 200) {
            // 获取content元素
            var content = document.querySelector(".content");
            // 清空content元素的内容
            content.innerHTML = "";
            // 将返回的 HTML 内容插入到 content 元素中
            content.innerHTML = xhr.responseText;
            // 对顶栏高亮进行设置
            const links = document.querySelectorAll('.top dt a');
            links.forEach(link => {link.style.color = '#bec3de';});
        }
    };
    // 处理 AJAX 请求错误
    xhr.onerror = function() {
        alert("网络错误！请检查网络配置！");
    }
});
// 鼠标移动至头像框
var info_box = document.getElementById("info-box");
show_info_box.addEventListener("mouseenter", function() 
{
    info_box.style.display = "block";
});
show_info_box.addEventListener("mouseleave", function() 
{
    info_box.style.display = "none";
});

// 点击其他地方隐藏身份
document.addEventListener('click', function(event) 
{
    if (event.target !== identity_box && event.target !== user_id_content) 
    {
        identity_box.style.display = 'none';
    }
});

// 切换主页页数
function turn(direction,username,identity)
{
    var page_value_all = document.querySelector("#page_value_all").textContent;
    var page_value = document.querySelector('#page_value');

    if(direction == 'left' && parseInt(page_value.textContent) > 1)
    {
        page_value.textContent  =  parseInt(page_value.textContent) - 1;
        search_page(false,username,identity,false);
    }
    else if(direction == 'right' && parseInt(page_value.textContent) < parseInt(page_value_all))
    {
        page_value.textContent  =  parseInt(page_value.textContent) + 1;
        search_page(false,username,identity,false);
    }
}
// 搜索
function search_page(is_search,username,identity,new_page)
{
    var search_content = document.getElementById("search_content").value;
    var task = document.querySelector('input[name="task"]:checked').value;
    var page_value = document.querySelector('#page_value');

    var xhr = new XMLHttpRequest();
    xhr.open("POST", "inc/search.php", true);
    xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
    xhr.send("search_content=" + search_content + "&task=" + task + "&search=" + is_search + "&username=" + username + "&identity=" + identity + "&page=" + page_value.textContent + "&new_page=" + new_page);
    xhr.onreadystatechange = function() 
    {
        if (xhr.readyState == 4 && xhr.status == 200) 
        {
            var result = JSON.parse(xhr.responseText);
            var rows = result.rows;
            var count = result.count;
            var result_value= document.querySelectorAll(".result_value");
            result_value.forEach(function(row)
            {
                row.innerHTML = "";
            })
            if (rows.length > 0) 
            {
                var result_value= document.querySelectorAll(".result_value");
                result_value.forEach(function(row, index) {
                    if (index < rows.length) {
                        var button_text = rows[index]['CPublic'] == '私密' ? '私密' : '详情';
                        var button_color = rows[index]['CPublic'] == '私密' ? 'red' : '';
                        row.innerHTML = `
                            <td class='option'><button id='read_letter' value='${rows[index]['CID']}' style='color: ${button_color};'>${button_text}</button></td>
                            <td class='number'><span>${rows[index]['CID']}</span></td>
                            <td class='title'><span>${rows[index]['CTitle']}</span></td>
                            <td class='staff'><span>${rows[index]['CFinAdmin'] ? rows[index]['CFinAdmin'] : ''}</span></td>
                            <td class='time'><span>${rows[index]['CFinTime'] ? rows[index]['CFinTime'] : ''}</span></td>
                            <td class='state'><span>${rows[index]['CState']}</span></td>
                        `;
                    } else {
                        row.innerHTML = "";
                    }
                });
            }
            if(new_page == true)
            {
                var page_value_all= document.querySelector("#page_value_all");
                if(count == 0)
                {
                    page_value_all.innerHTML = 1;
                }
                else
                {
                    page_value_all.innerHTML = Math.ceil(count / 10);
                }
                page_value.textContent = 1;
            }
            rebound_want_read_letter();
        }
    };
    return true;
}


// 切换日志页数
function turn_log(direction)
{
    var page_value_all = document.querySelector("#page_value_all_log").textContent;
    var page_value = document.querySelector('#page_value_log');

    if(direction == 'left' && parseInt(page_value.textContent) > 1)
    {
        page_value.textContent  =  parseInt(page_value.textContent) - 1;
        switch_log_page(page_value.textContent);
    }
    else if(direction == 'right' && parseInt(page_value.textContent) < parseInt(page_value_all))
    {
        page_value.textContent  =  parseInt(page_value.textContent) + 1;
        switch_log_page(page_value.textContent);
    }
}
// 切换日志页数
function switch_log_page()
{
    var page_value = document.querySelector('#page_value_log');

    var xhr = new XMLHttpRequest();
    xhr.open("POST", "inc/switch_log.php", true);
    xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
    xhr.send("page_value=" + page_value.textContent);
    xhr.onreadystatechange = function() 
    {
        if (xhr.readyState == 4 && xhr.status == 200) 
        {
            var result = JSON.parse(xhr.responseText);
            var rows = result.rows;
            var result_value= document.querySelectorAll(".result_value");
            result_value.forEach(function(row)
            {
                row.innerHTML = "";
            })
            if (rows.length > 0) 
            {
                var result_value= document.querySelectorAll(".result_value");
                result_value.forEach(function(row, index) {
                    if (index < rows.length) {
                        row.innerHTML = `
                            <td class='log_username'><span>${rows[index]['LUsername']}</span></td>
                            <td class='log_name'><span>${rows[index]['Name']}</span></td>
                            <td class='log_time'><span>${rows[index]['LTime']}</span></td>
                            <td class='log_TIP'><span>${rows[index]['LIP']}</span></td>
                            <td class='log_event'><span>${rows[index]['LEvent']}</span></td>
                            <td class='log_note'><span>${rows[index]['LNote']}</span></td>
                        `;
                    } else {
                        row.innerHTML = "";
                    }
                });
            }
        }
    };
    return true;
}