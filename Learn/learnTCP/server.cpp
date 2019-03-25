/*************************************************************************
	> File Name: server.cpp
	> Author: LHC 
	> Mail: 3115629644@qq.com 
	> Created Time: 2018年12月08日 星期六 10时22分49秒
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
   int listenfd,connfd;
   socklen_t  clilen;
   struct sockaddr_in cliaddr,servaddr;
 
   listenfd=socket(AF_INET,SOCK_STREAM,0);
   bzero(&servaddr,sizeof(servaddr));
 
   servaddr.sin_family=AF_INET;
   servaddr.sin_addr.s_addr=INADDR_ANY;
   servaddr.sin_port=htons(8888);
 
   bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr));  
   listen(listenfd,2);
 
   getchar();
   connfd=accept(listenfd,(struct sockaddr *)&cliaddr,&clilen);
 
   close(connfd);
   close(listenfd);
    return 0;
}

