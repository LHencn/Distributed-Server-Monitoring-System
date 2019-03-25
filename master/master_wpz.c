#include "/Users/王培臻/xiangmu/common.h"
// 最多设置的线程数
#define INS 5
#define listem 10
#define MAX 1000
#define PORT 9011
#define port1 1234


#ifdef _DEBUG
#define DBG(fmt , args...)  printf(fmt, ##args) 
#else
#define DBG(fmt , args...)  
#endif



int start_socket(int port);
void *func(void *arg);

void *func1();

typedef struct Node {
    struct sockaddr_in addr;//地址结构体
    struct Node *next;
} Node, *LinkedList;

typedef struct mypara {
    LinkedList l;
    int num;
} mypara;

int queue[INS + 1] = {0};
LinkedList linkedlist[INS + 1];
FILE *log1[INS + 1];


Node insert(LinkedList head, Node *node, int index) {
    Node *p, ret;
    p = &ret;
    ret.next = head;
    while(p->next && index) {
        p = p->next;
        --index;
    }
    if(index == 0) {
        node->next = p->next;
        p->next = node;
    }
    return ret;
}

int search(Node *head, struct sockaddr_in t_addr) {
    Node *p = head;
    while (p != NULL) {
        if (p->addr.sin_addr.s_addr == t_addr.sin_addr.s_addr) {
            return 1;
        }
        p = p->next;
    }
    return 0;
}


Node *delete_node(LinkedList head, Node *temp, int num) {
    DBG("==> Delete begining <==\n");
    Node ret, *p;
    ret.next = head;
    p = &ret;
    while (p && temp->addr.sin_addr.s_addr != p->next->addr.sin_addr.s_addr) {
        p = p->next;
    }
    if (p) {
        int pthread_mutex_lock(pthread_mutex_t * mutex);
        DBG("****删除IP为：%s , ", inet_ntoa(p->next->addr.sin_addr));
        p->next = p->next->next;
        DBG("delete node\n");
        queue[num]--;
        int pthread_mutex_unlock(pthread_mutex_t * mutex);
        DBG("****%d.log剩余 %d 个元素\n", num, queue[num]);
    }
        sleep(1);
    return ret.next;
}


void output(LinkedList head, int num) {
    Node *p = head;
    char logfile[20];
    sprintf(logfile, "%d.log", num);
    log1[num] = fopen(logfile, "w+");
    while(p) {
        DBG("%s:%d\n", inet_ntoa(p->addr.sin_addr), p->addr.sin_port);
        fprintf(log1[num], "[%s:%d]", inet_ntoa(p->addr.sin_addr), p->addr.sin_port);
        if(p->next) {
            DBG(" ");
            fprintf(log1[num], "%s", " ");
        }
        p = p->next;
    }
    DBG("\n");
    fprintf(log1[num], "\n");
    fflush(log1[num]);
    fclose(log1[num]);
}

void clear(LinkedList head) {
    Node *p, *q;
    p = head;
    while(p) {
        q = p->next;
        free(p);
        p = q;
    }
    return ;
}

// 传出的是最小的下标，而非最小值!!!!!!
int find_min(int n, int *arr) {
    int min = 100, ind = 0;
    for (int i = 0; i < n; i++) {
        if (arr[i] < min) {
            min = arr[i];
            ind = i;
        } 
    }
    return ind;
}

LinkedList connect_or_delete(LinkedList head, int ind) {
    Node *p = head;
    int sockfd;
    struct sockaddr_in sockaddr;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
    }

    while (p != NULL) {
        sockaddr.sin_family = AF_INET;
        sockaddr.sin_port = htons(PORT);
        sockaddr.sin_addr.s_addr = p->addr.sin_addr.s_addr;
        int conn = connect(sockfd,(struct sockaddr *)&sockaddr, sizeof(struct sockaddr));
        if (conn < 0 ) { 
            perror("Connect");
            Node *q = p->next;
            head  = delete_node(head, p, ind);
            p = p->next;
            //queue[ind]--;
            DBG("delete success!\n");
        }else {
            DBG("connect success!\n");
            int id, len;
            char buffer[MAX];
            struct sockaddr_in c_addr;
            int newfd;

            while ((len = recv(sockfd, &id, 4, 0)) > 0) {
                DBG("%d\n", len);
                struct sockaddr_in client_addr;
                if (send(sockfd, &id, 4, 0) < 0) {
                    perror("send:");
                    continue;
                }
                socklen_t len = sizeof(client_addr);
                DBG("wait connect\n");
                
                if((newfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                    perror("socket");
                    close(newfd);
                    continue;
                }
                
                c_addr.sin_family = AF_INET;
                c_addr.sin_port = htons(2002);
                c_addr.sin_addr.s_addr = p->addr.sin_addr.s_addr;

                if (connect(newfd, (struct sockaddr *) &c_addr, sizeof(struct sockaddr)) < 0) {
                    perror("short connect:");
                    close(newfd);
                    continue;
                }
                
                DBG("recv begining!\n");
                //创建以客户端IP命名的目录
                int len2;
                while ((len2 = recv(newfd, buffer, MAX, 0)) > 0) {
                    buffer[len2] = '\0';

                    DBG("recv %d 字节 %s\n", len2, buffer);
                    char IP[30] = {0};
                    strcpy(IP, inet_ntoa(client_addr.sin_addr));
                    char dirname[1024] = "/Users/王培臻/xiangmu/master/";
                    strcat(dirname, IP);
                    if (NULL == opendir(dirname)) {
                    mkdir(dirname, 0777);
                    }

                    if (id == 101) {
                        DBG("*** start recv cpu ***\n");
                        strcat(dirname, "/cpu.log");
                    } else if (id == 105) {
                        DBG("*** start recv disk ***\n");
                        strcat(dirname, "/disk.log");
                    } else if (id == 104) {
                        DBG("*** start recv mem ***\n");
                        strcat(dirname, "/mem.log");
                    } else if (id == 103) {
                        DBG("*** start recv user ***\n");
                        strcat(dirname, "/user.log");
                    } else if (id == 102) {
                        DBG("*** start recv system ***\n");
                        strcat(dirname, "/system.log");
                    } else if (id == 100) {
                        DBG("*** start recv malicious_process ***\n");
                        strcat(dirname, "/process.log");
                    }

                    FILE *fd;
                    fd = fopen(dirname, "a+");
                    fwrite(buffer, sizeof(char), strlen(buffer), fd);
                    fclose(fd);
                }
                memset(buffer, 0, sizeof(buffer));
                close(newfd);
            }
            //sleep(30);
            close(sockfd);
            p = p->next;
        }
            //close(sockfd);
    }
    return head;
}



char *get_conf_value(char *path_name, char *key_name, char *value);

int main() {
    /*int pthread_mutex_lock(pthread_mutex_t * mutex);
    char value[20];
    char *prename = get_conf_value("./init.conf", "prename", value);
    char *start = get_conf_value("./init.conf", "start", value);
    char *finish = get_conf_value("./init.conf", "finish", value);
    for (int i = atoi(start); i <= atoi(finish); i++) {
        char temp[20];
        strcpy(temp, prename);
        sprintf(temp + strlen(temp) - 1, ".%d", i);
        struct sockaddr_in t_addr;
        t_addr.sin_family = AF_INET;
        t_addr.sin_port = htons(PORT);
        t_addr.sin_addr.s_addr = inet_addr(temp);
        int ind = find_min(INS, queue);
        Node *p, ret;
        p = (Node *)malloc(sizeof(Node));
        p->addr = t_addr;
        p->next = NULL;
        ret = insert(linkedlist[ind], p, queue[ind]);
        linkedlist[ind] = ret.next;
        queue[ind]++;
        //DBG("%s\n", temp);
    }
    int pthread_mutex_unlock(pthread_mutex_t * mutex);
*/
    int pid =fork();
    if(pid > 0) exit(0);


    freopen("/dev/null", "w", stderr);


    for (int i = 0; i < INS; i++) {
        DBG("%d ", queue[i]);
        DBG(" ....... %d.log .......\n", i);
        output(linkedlist[i], i);
    }

    int sockfd;
    int socket_server;
    struct sockaddr_in addr;
    
    pthread_t t[INS + 1];//存储线程ID
    mypara para[INS + 1];//每个线程有一个独立的数据区，不会发生冲突
    for (int i = 0; i < INS; i++) {
        para[i].l = linkedlist[i];
        // 设置num的值后将para传入任务，可用来标识线程
        para[i].num = i; 
        if(pthread_create(&t[i], NULL, func, (void *)&para[i]) == -1) {
            //线程ID(只有在进程环境下有意义)、属性、函数、函数参数(多个参数用结构体)
            //创建结束后值执行结束时间不确定
            DBG("error\n");
            return -1;
            //exit(1);
        }
    }

    if(pthread_create(&(t[5]), NULL, func1, NULL) == -1) {
        DBG("error\n");
        return -1;
    }

    socket_server = start_socket(port1);
    while(1) {
        struct sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);
        if((sockfd = accept(socket_server, (struct sockaddr*) &client_addr, &len)) < 0) {
            perror("accept");
            close(sockfd);
            return -1;
        }
        DBG("hi dog!\n");
        struct sockaddr_in t_addr;
        t_addr.sin_family = AF_INET;
        t_addr.sin_port = htons(PORT);
        char *temp = inet_ntoa(client_addr.sin_addr);
        t_addr.sin_addr.s_addr = inet_addr(temp);

        int num = 0;
        while(num <= 5) {
            int ans = search(linkedlist[num], t_addr);
            if (ans == 1) {
                DBG("exist!\n");
                num = 0;
                break;
            }
            if (num == 5) {
                int sub = find_min(INS, queue);
                Node *p, ret;
                p = (Node *)malloc(sizeof(Node));
                p->addr = t_addr;
                p->next = NULL;
                int pthread_mutex_lock(pthread_mutex_t * mutex);
                ret = insert(linkedlist[sub], p, queue[sub]);
                linkedlist[sub] = ret.next;
                queue[sub]++;
                DBG("insert success!\n");
                output(linkedlist[sub], sub);
                int pthread_mutex_unlock(pthread_mutex_t * mutex); 
            }
            num++;
        }

    }
    //　先让子进程执行完毕
    pthread_join(t[0], NULL); //等待当前执行的线程完毕
    pthread_join(t[1], NULL);
    pthread_join(t[2], NULL);
    pthread_join(t[3], NULL);
    pthread_join(t[4], NULL);
    

    DBG("\n"); 
    return 0;
}

void *func(void *argv) {
    struct mypara *para;
    para = (struct mypara *)argv;
    LinkedList l = para->l;
    while(1) {
        if (linkedlist[para->num] == NULL) {
            sleep(30);
        }
        int pthread_mutex_lock(pthread_mutex_t * mutex);
        linkedlist[para->num] = connect_or_delete(linkedlist[para->num], para->num);
        DBG ("-------%d---------%d\n", para->num, queue[para->num]);
        output(linkedlist[para->num], para->num);
        int pthread_mutex_unlock(pthread_mutex_t * mutex);
        sleep(1);
    }
    pthread_exit(NULL);
}

void *func1() {
   int socket_server = start_socket(1996);
    while(1) {
        struct sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);
        int coon;
        if ((coon = accept(socket_server, (struct sockaddr *) &client_addr, &len)) < 0) {
            perror("accept\n");
            close(coon);
            exit(0);
        }
        char buffer[1024];
        int len2;
        while ((len2 = recv(coon, buffer, MAX, 0)) > 0) {
            buffer[len2] = '\0';
            DBG("==> 成功接收警告信息 %d 字节 %s\n", len2, buffer);
            DBG("recv %d 字节 %s\n", len2, buffer);

            char IP[30] = {0};
            strcpy(IP, inet_ntoa(client_addr.sin_addr));
            char dirname[1024] = "/Users/王培臻/xiangmu/master/";
            strcat(dirname, IP);
            if (NULL == opendir(dirname)) {
                mkdir(dirname, 0777);
            }
            strcat(dirname, "/warning.log");
            FILE *fd;
            fd = fopen(dirname, "a+");
            fwrite(buffer, sizeof(char), strlen(buffer), fd);
            fclose(fd);
        }
        memset(buffer, 0, sizeof(buffer));
        close(coon);
    } 
    close(socket_server);
}

char *get_conf_value(char *path_name, char *key_name, char *value) {
    FILE *fp = fopen(path_name, "r");
    if (fp == NULL) {
        perror("error open file");
        exit(0);
    }
    int read_cnt;
    char *line = NULL;
    size_t len = 0;
    while ((read_cnt = getline(&line, &len, fp)) != -1) {
        int lenb = strlen(key_name);
        value = strstr(line, key_name);  //如果找不到子串则返回NULL
        if (value == NULL) continue;     //找到则返回第一次出现的位置
        value = value + lenb + 1;        //额外加1用来跳过'='号
        //DBG("%s is %s", key_name, value);
        //　找到后就break这样可以得到正确的value
        break;
    }
    return value;
}

int start_socket(int port) {
    int socket_server, sockfd;
    struct sockaddr_in s_addr;
    socklen_t len;

    if((socket_server = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return -1;
    }

    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(port);
    s_addr.sin_addr.s_addr = htons(INADDR_ANY);

    int opt = 1;
    setsockopt(socket_server, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt));

    if((bind(socket_server, (struct sockaddr*) &s_addr, sizeof(struct sockaddr))) < 0) {
        perror("bind");
        return -1;
    }
    
    if(listen(socket_server, listem) < 0) {
        perror("listem");
        return -1;
    }
    return socket_server;
}
