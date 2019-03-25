/*************************************************************************
	> File Name: client.c
	> Author: LHC 
	> Mail: 3115629644@qq.com 
	> Created Time: 2019年02月27日 星期三 16时25分30秒
 ************************************************************************/

#include "./common/common.h"
#define Cport 3333
#define Sport 4444
#define SIP 10.161.57.11

int main() {
    int sockfd = create_socket(Cport);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(Sport);
    server_addr.sin_addr.s_addr = inet_addr("SIP");
    char *msg = "I am client!";
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr) < 0)) {
        perror("connect:");
        exit(-1);
    }
    if (send(sockfd, (const void *)&msg, sizeof(msg), 0) < 0) {
        perror("send:");
        exit(-1);
    }
    return 0;
}
