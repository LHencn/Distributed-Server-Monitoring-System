/*************************************************************************
	> File Name: master_gl.c
	> Author: LHC 
	> Mail: 3115629644@qq.com 
	> Created Time: 2018年12月07日 星期五 10时39分52秒
 ************************************************************************/

#include<stdio.h>
#define INS 5

typedef struct Node {
    struct sockaddr_in addr;
    struct Node *next;
}Node, *LinkedList;
LinkedList linkedlist[INS + 1];
int queue[INS + 1];     //存储每个链表中有几个节点
void init_linkedlist(int n);

int main() {
    init_linkedlist(INS);
    pthread_t t[INS + 1];  //线程标识符数组
    /*
     *  typedef unsigned long int pthread_t;
        come from /usr/include/bits/pthreadtypes.h
        用途：pthread_t用于声明线程ID。
        sizeof(pthread_t) =8
        pthread_t，在使用printf打印时，应转换为u类型。
    */
    char value[20] = {0}, start[10] = {0}, finish[10] = {0}, port[10] = {0}; //port客户端开的端口
    get_conf_value("./config.conf", "prename", value);
    get_conf_value("./config.conf", "start", start);
    get_conf_value("./config.conf","finish", finish);
    get_conf_value("./config.conf", "port", port);
    for (int i = atoi(start); i <= atoi(finish); i++) { //将网络节点加入链表中
        char ip[100];
        sprintf(ip, "%s.%d", value, i); //形成IP 
        struct sockaddr_in addr;
        addr.sin_port = htons(atoi(port));
        addr.sin_addr.s_addr = inet_addr(ip);
        int sub = find_min(INS, queue);  //最短链表
        Node *p; 
        p = (Node *)malloc(sizeof(Node));
        p->addr = addr;
        p->next = NULL;
        insert(linkedlist[sub], p, queue[sub]);
        queue[sub]++;
    }
    for (int i = 0; i < INS; i++) {
        
    }

}

/****************初始化链表***********************/
void init_linkedlist(int n) {
    for (int i = 0; i < n; i++) {
        linkedlist[i] = (LinkedList)malloc(sizeof(Node));
        linkedlist[i]->next = NULL;
    }
    return ;
}

/****************寻找最短链表*********************/
int find_min(int N, int *addr) {
    int min = arr[0];
    int ans = 0;
    for (int i = 1; i < N; i++) {
        if (min > arr[i]) {
            min = arr[i];
            ans = i;
        }
    }
    return ans;
}

/****************插入链表*************************/
void insert(LinkedList head, Node *node, int index) {
    Node *p = head;
    while (p && index) {
        p = p->next;
        index--;
    }
    if (index == 0) {
        p->next = node;
    } else {
        printf("insert error!!!\n");
    }
    return ;
}
