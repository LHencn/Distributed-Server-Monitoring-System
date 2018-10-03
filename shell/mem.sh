#!/bin/bash

last=$1                                                                  #last为上一个动态平均值，手动输入
date=`date +%Y-%m-%d_%T`                                                 #date为当前时间
n=(`free -m |head -n 2 |tail -n 1 |awk '{print $2, $4, $3}'`)            #n[0]代表total，n[1]代表free，n[2]代表used
per=`echo "scale=1; ${n[2]}/${n[0]}" | bc | awk '{printf"%0.1f",$0}'`
ave=`echo "scale=1; 0.3*${last}+0.7*${per}" | bc | awk '{printf"%0.1f",$0}'`
echo $date ${n[0]}M ${n[2]}M ${per}% ${ave}%
