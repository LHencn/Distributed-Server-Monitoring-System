#!/bin/bash
date=`date +"%Y-%m-%d__%T"`                                    #date代表当前时间
user_num=`cat /etc/passwd | grep "bash" | cut -d ":" -f 1 | wc -l`  #user_num代表用户总数
active_user=`last | head -n -2 | cut -d " " -f 1 | sort | uniq -c | head -n 3 | awk '{print $2}' | xargs | tr " " ","`  #active_users代表近期最活跃的3个用户
root_user=`cat /etc/group | sort | grep "sudo" | cut -d ":" -f 4`   #root_users为具有root权限的用户
current_user=`who | awk '{print $1 $5 $2 "\n"}' | tr "(" "_" | tr ")" "_" | xargs | tr " " ","`  #current_name代表当前在线用户
ip=`ifconfig wlo1 | head -n 2 | grep "inet" | awk '{print $2}'`
echo $date $user_num [${active_user}] [${root_user}] [${current_user}_$ip]
