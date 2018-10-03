#!/bin/bash
#通过脚本输出磁盘总量、剩余量占用比例等信息

date=`date +%Y-%m-%d__%T`                #date为当前时间

eval `df -m |grep "sda" |awk '{printf("partition_total["NR"]=%d; partition_used["NR"]=%d; partition_percentage["NR"]=%s; partition_path["NR"]=%s\n", $2, $3, $5, $6)}'`

disk_total=0
disk_used_percentage=0
disk_used=0

for i in `seq 1 ${#partition_total[@]}`; do
   #disk_total=$[ $disk_total + ${partition_total[$[i-1]]} ]
   disk_total=$[ $disk_total + ${partition_total[$i]} ]
   # disk_used=$[ $disk_used + ${partition_used[$[i-1]]} ]
   disk_used=$[ $disk_used + ${partition_used[$i]} ]
done

disk_used_percentage=`echo "scale=2; $[$disk_used * 100 / $disk_total]" |bc |awk '{print"%.2f", $0}'`
disk_free=$[$disk_total-$disk_used]

echo ${date} 0 disk $disk_total ${disk_free} ${disk_used_percentage}% 

for i in `seq 1 ${#partition_total[@]}`; do
    echo $date 1 ${partition_path[$[i-1]]} ${partition_total[$[i-1]]} ${partition_used[$[i-1]]} ${partition_percentage[$[i-1]]}
done
