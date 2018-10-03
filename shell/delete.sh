#!/bin/bash
#安全的rm命令

if [ $1 == '-r' ]; then
    mv $2 ~Documents/trash/
fi
