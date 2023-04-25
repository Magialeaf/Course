#!/bin/bash
echo "选择测试案例:"
echo "1.变量read"
read -p "choose:" n
if [ $n -eq 1 ]
then
   testread
else
   echo "End\n"
fi

function testread()
{
    local res=0
    local get=0
    printf "res=%d\n" $get
    echo "Enter res + n?"
    read -p "n:" get
    ((res+=get))
    printf "res=%d\n" $get
}



