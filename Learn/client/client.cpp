/*************************************************************************
	> File Name: clinet1.cpp
	> Author: LHC 
	> Mail: 3115629644@qq.com 
	> Created Time: 2018年09月25日 星期二 18时30分18秒
 ************************************************************************/
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
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdarg.h>

#include <stdio.h>
#include<iostream>
using namespace std;
#define MAX_SIZE 1024


int main(int argc, char *argv[]){       
	char buffer[MAX_SIZE];            //数据缓冲区     
	int port = atoi(argv[2]);         //远端端口
	char *host = argv[1];             //远端ip地址
    int sock_client = socket(AF_INET, SOCK_STREAM, 0); //创建本地套接字描述符
	if (sock_client < 0){    //判断套接字是否创建成功
        perror("Socket: ");
		return -1;
	}

	// 连接服务器，进行TCP三次握手
    struct sockaddr_in dest_addr;
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(port);
	dest_addr.sin_addr.s_addr = inet_addr(host);
	if (connect(sock_client, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0){
        perror("Connect: ");
		return -1;
	}
    printf("连接成功");
    
    //打开读的文件
   const char *files = argv[3];  //传输文件
    FILE *fd = fopen(files, "r");
    if (fd == NULL) {
        printf("can't open the file");
        exit(0);
    }
    int i, ch;
    ch = fgetc(fd);
    for (i = 0; feof(fd) == 0; i++) {
        buffer[i] = (char)ch;
        ch = fgetc(fd);
    }
    buffer[i - 1] = '\0';
    printf("%s\n", buffer);
    fclose(fd);

    send(sock_client, buffer, strlen(buffer), 0);
    close(sock_client);
    return 0;
}
