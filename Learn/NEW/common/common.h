/*************************************************************************
	> File Name: common.h
	> Author: LHC 
	> Mail: 3115629644@qq.com 
	> Created Time: 2019年02月27日 星期三 16时24分50秒
 ************************************************************************/

#ifndef _COMMON_H
#define _COMMON_H

#include <pthread.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/file.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdarg.h>

int create_socket(int);
#endif
