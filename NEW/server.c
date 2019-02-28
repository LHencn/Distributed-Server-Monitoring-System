/*************************************************************************
	> File Name: server.c
	> Author: LHC 
	> Mail: 3115629644@qq.com 
	> Created Time: 2019年02月28日 星期四 19时37分47秒
 ************************************************************************/

#include <stdio.h>
#include "./common/common.h"

#define Sport 4444

void *start_routine(void *arg) {
    
}

int main() {
    int sockfd = create_socket(Sport), sockfdNew;
    struct sockaddr *addr;
    int addrlen = sizeof(addr), len = 100;
    char *buf;
    pthread_t *thread;
    const pthread_attr_t *attr = NULL;
    void *arg, **retval;
    if (pthread_create(thread, attr, start_routine, arg) != 0) {
        perror("pthread_create:");
        exit(-1);
    }
    if (listen(sockfd, 3) < 0) {
        perror("listen:");
        exit(-1);
    }
    if (getpeername(sockfd, addr, (socklen_t *)&addrlen) < 0) {
        perror("getpeername:");
        exit(-1);
    }
    if ((sockfdNew = accept(sockfd, addr, (socklen_t *)&addrlen)) < 0) {
        perror("accept:");
        exit(-1);
    }
    if ((len = recv(sockfd, (void *)buf, len, 0)) < 0) {
        perror("recv:");
        exit(-1);
    }
    printf("%s\n", buf);

    
    if (!pthread_join(*thread, retval)) {
        perror("pthread_join:");
        exit(-1);
    }
    pthread_exit(*retval);
    return 0;

}
