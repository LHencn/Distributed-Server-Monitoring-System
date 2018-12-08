/*************************************************************************
	> File Name: client.cpp
	> Author: LHC 
	> Mail: 3115629644@qq.com 
	> Created Time: 2018年12月08日 星期六 10时23分38秒
 ************************************************************************/

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <malloc.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#define MAXLINE 4096

int main()
{
   int sockfd;
   struct sockaddr_in servaddr;


   sockfd=socket(AF_INET,SOCK_STREAM,0);
   bzero(&servaddr,sizeof(servaddr));
   servaddr.sin_family=AF_INET;
   servaddr.sin_port=htons(8888);
   servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

   int ret=connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
   getchar();

   close(sockfd);
    return 0;
}

