/*************************************************************************
	> File Name: master.c
	> Author: LHC 
	> Mail: 3115629644@qq.com 
	> Created Time: 2018年11月25日 星期日 11时00分58秒
 ************************************************************************/

#include "common.c"
#include "common.h"
#define INS 5
#define __DEBUG__ 
#ifdef __DEBUG__ 
#define DEBUG(format,...) printf(format, ##__VA_ARGS__)
#else 
#define DEBUG(format,...) 
#endif 

/*********************创建链表监控服务器************************/
typedef struct Node{                      //链表节点,多线程负责监听服务器，采用链表将服务器串联起来
    struct sockaddr_in client_addr;       
    struct Node *next; 
}Node, *LinkedList;

LinkedList List[INS];                     //创建INS个链表

/*********************链表中插入节点******************/
LinkedList insert_List(LinkedList List, struct sockaddr_in client_addr) {
    Node *p = List;
    while (p->next) {
        p = p->next;                                           
    }
    Node node;
    node.client_addr = client_addr;
    node.next = NULL;
    p->next = &node;
    return List;
}

/*********************链表中删除节点******************/
//LinkedList delete_List(LinkedList List, )


/*********************初始化**************************/
int main() {
    char start[20], prename[20], finish[20];
    get_conf("./.init.conf", "prename", prename);                       //服务器子网prename
    get_conf("./.init.conf", "start", start);                           //服务器开始ip start
    get_conf("./.init.conf", "finish", finish);                         //服务器结束ip finish
    struct sockaddr_in client_addr;                                     //单个服务器网络节点
    client_addr.sin_family = AF_INET;
    int len = strlen(prename);
    prename[len - 1] = '.';
    len = strlen(prename);
    prename[len] = '\0';                                                //设置char数组末端为\0
    
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);                     //创建本地套接字
    if (socketfd < 0) {
        perror("Socket: ");
        return -1;
    } 

    //连接子网和末端数字形成IP
    char temp[20];                                                      //保存prename
    strcpy(temp, prename);
    char string[5];                                                     //进行连接IP
    int j = 0;                                                          //做循环链表，循环插入网络节点
    for (int i = atoi(start) + 1; i <= atoi(finish); i++) {             //将配置文件中的网络节点顺序放入INS个链表中
        sprintf(string, "%d", i);
        len = strlen(string);
        string[len] = '\0';
        strcpy(prename, temp);
        strcat(prename, string);
        DEBUG("IP = %s\n", prename);
        client_addr.sin_addr.s_addr = inet_addr(prename);
        client_addr.sin_port = htons(55555);                            //设置服务器端口号 

        //TCP连接
        if (connect(socketfd, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0) {
            perror("Connect: ");
            printf("第 %d 个监控服务器连接失败!!!\n", i);
            continue;                                                   //若初始化连接失败，则跳到下一个被监控的服务器
        }
        printf("第 %d 个监控服务器连接成功!\n", i);
        List[j % 5] = insert_List(List[j % 5], client_addr);
        j++; 
    }
    
    

    return 0;
}

