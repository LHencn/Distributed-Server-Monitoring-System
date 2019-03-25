#!/bin/bash
#该脚本为获取cpu信息

load=(`cat /proc/loadavg |cut -d " " -f 1-3`)                #load为cpu3个平均负载
#前三个值分别代表系统5分钟、10分钟、15分钟前的平均负载
#第四个值的分子是正在运行的进程数，分母为总进程数
#第五个值是最近运行的进程id
total1=(`cat /proc/stat |head -n 1 |cut -d " " -f 2-`)       #total1代表时间1下的cpu信息
sleep 0.5                                                    #cpu等待0.5s后再执行
total2=(`cat /proc/stat |head -n 1 |cut -d " " -f 2-`)       #total2代表时间2下的cpu信息
date=`date +%Y-%m-%d__%T`                                                               #date为当前的时间，之所以不能放在第一行是因为有sleep暂停cpu0.5s

cpu_total1=0                                                 #代表系统启动后到时间1下的cpu使用时间
for i in ${total1[*]}; do
    cpu_total1=$[${cpu_total1} + $i]
done

cpu_total2=0                                                 #代表系统启动到时间2下的cpu使用使用时间   
for i in ${total2[*]}; do
    cpu_total2=$[${cpu_total2} + $i]
done

cpu_total_time=$[$cpu_total2-$cpu_total1]
idle=$[${total1[3]} - ${total2[3]}]
pcpu=`echo "scale=2; 100*$[$cpu_total_time-$idle]/$cpu_total_time" |bc |awk '{printf"%.2f", $0}'`

temperature=`sensors |head -n 11 |tail -n 1 |cut -d " " -f 5 |cut -c2-5`    #当时的CPU的温度

temp=`echo "temperature <= 50" |bc`
if [[ ${temp} -eq 1 ]]; then
    mark="normal"
else
    temp=`echo "temperature <= 70" |bc`
    if [[ ${temp} -eq 1 ]]; then
        mark="note"
    else
        mark="warning"
    fi
fi

echo $date ${load[0]} ${load[1]} ${load[2]} $pcpu ${temperature}℃ "" $mark
