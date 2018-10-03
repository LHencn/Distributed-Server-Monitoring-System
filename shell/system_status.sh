#!/bin/bash
#输出系统运行状况脚本
date=`date +%Y-%m-%d_%T`                                  #当前时间
host=`hostname`                                           #host为主机名
version_os=`uname -o`                                     #version_os为os版本
version_kernel=`uname -r`                                 #version_kernel为内核版本 
runTime=`top |head -n 1 |awk '{print $4, $5}'`            #runTime为运行时间
loadTime=`top |head -n 1 |awk '{print $10, $11, $12}'`    #loadTime为负载情况
zone_total=(`df -m |grep "sda" |awk '{print $2}'`)        #zone_total数组为磁盘各分区总容量情况，单位为M
zone_used=(`df -m |grep "sda" |awk '{print $3}'`)
disk_total=0                                              #disk_total磁盘总存储量
disk_used=0                                               #disk_used已用磁盘总量

for i in `seq 1 ${#zone_total[@]}`; do
    disk_total=$[$disk_total+${zone_total[$[$i-1]]}]
done

for i in `seq 1 ${#zone_used[@]}`; do
    disk_used=$[$disk_used+${zone_used[$[$i-1]]}]
done

disk_used_percentage=`echo "scale=2; ${disk_used} * 100 / ${disk_total}" |bc |awk '{printf"%.2f", $0}'`         #磁盘占用率

memory_total=`free -m |head -n 2 |tail -n 1 |awk '{print $2}'`                                                  #内存总容量

memory_used=`free -m |head -n 2 |tail -n 1 |awk '{print $3}'`                                                   #内存已用量

memory_used_percentage=`echo "scale=2; ${memory_used} * 100 / ${memory_total}" |bc |awk '{printf"%.2f", $0}'`   #内存占用率

cpu_temperature=`sensors |head -n 11 |tail -n 1 |cut -d " " -f 5 |cut -c2-5`                                    #内存温度


#****************磁盘报警级别*********************
temp=`echo "$disk_used_percentage <= 80" |bc`             #temp存入小数间比较大小的值
if [[ ${temp} -eq 1 ]]; then
    disk_mark="normal"
else
    temp=`echo "$disk_used_percentage <= 90" |bc`
    if [[ $temp -eq 1 ]]; then
        disk_mark="note"
    else 
        disk_mark="warning"
    fi
fi
#*************************************************

#****************CPU温度报警级别******************
temp=`echo "$cpu_temperature <= 50" |bc`
if [[ ${temp} -eq 1 ]]; then
    cpu_mark="normal"
else 
    temp=`echo "$cpu_temperature <= 70" |bc`
    if [[ $temp -eq 1 ]]; then
        cpu_mark="note"
    else
        cpu_mark="warning"
    fi
fi
#*************************************************

#****************MEM百分比报警级别****************
temp=`echo "memory_used_percentage <= 70" |bc`
if [[ ${temp} -eq 1 ]]; then
    mem_mark="normal"
else
    temp=`echo "$memory_used_percentage <= 80" |bc`
    if [[${temp} -eq 1 ]]; then
        mem_mark="note"
    else
        mem_mark="warning"
    fi
fi
#*************************************************


echo ${date} ${host} $version_os $version_kernel $runTime ${loadTime[0]} ${loadTime[1]} ${loadTime[2]} ${disk_total}M ${disk_used_percentage}% ${memory_total}M ${memory_used_percentage}% ${cpu_temperature}℃ "" ${disk_mark} ${mem_mark} ${cpu_mark}



