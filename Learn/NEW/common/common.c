/*************************************************************************
	> File Name: common.c
	> Author: LHC 
	> Mail: 3115629644@qq.com 
	> Created Time: 2019年02月28日 星期四 19时39分35秒
 ************************************************************************/

#include <stdio.h>
#include "./common.h"
/*
 * 传入端口值监控此客户端的任意IP的此端口，返回绑定的socket
 *
 */
int create_socket(int port) { 
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    if (sockfd < 0) {
        perror("create_socket:");
        exit(-1);
    }
    struct sockaddr_in myaddr;
    /*
    FILE *f = popen("ip addr|grep inet|grep -v 127.0.0.1|grep -v inet6|awk '{print $2}'|sed 's/'\'/.*$//'", "r");
    char buf[20];
    fgets(buf, 20, f);
    printf("%s\n");
    */
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = INADDR_ANY;
    myaddr.sin_port = htons(port);
    if (bind(sockfd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
        perror("socket:");
        exit(-1);
    }
    return sockfd;
}

