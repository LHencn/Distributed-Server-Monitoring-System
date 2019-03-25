/*************************************************************************
	> File Name: client.c
	> Author: LHC 
	> Mail: 3115629644@qq.com 
	> Created Time: 2018年11月24日 星期六 20时10分59秒
 ************************************************************************/

#include "common.c"

int send_log(int data_sock, int retcode, char *client) {
    char data_dir[50] = "./Log";
    char logdir[100] = {0};
    char data[MAX_SIZE] = {0};
    generate_name(retcode, logdir, data_dir);//./Log与logdir路径连接
    FILE *fp = fopen(logdir, "r");
    /*
    文件使用方式 　　　　　　　意 义
    “rt”　　　　　　只读打开一个文本文件，只允许读数据
    “wt”　　　　　　只写打开或建立一个文本文件，只允许写数据
    “at”　　　　　　追加打开一个文本文件，并在文件末尾写数据
    “rb”　　　　　　只读打开一个二进制文件，只允许读数据
    “wb”　　　　 　 只写打开或建立一个二进制文件，只允许写数据
    “ab” 　　　　 　追加打开一个二进制文件，并在文件末尾写数据
    “rt+”　　　　　 读写打开一个文本文件，允许读和写
    “wt+”　　　　　 读写打开或建立一个文本文件，允许读写
    “at+”　　　　　 读写打开一个文本文件，允许读，或在文件末追加数 据
    “rb+”　　　　　 读写打开一个二进制文件，允许读和写
    “wb+”　　　　　 读写打开或建立一个二进制文件，允许读和写
    “ab+” 　　　　　读写打开一个二进制文件，允许读，或在文件末追加数据
    */
    int cnt = 0;
    printf("%s data : \n", logdir);
    while (fgets(data, MAX_SIZE - 1, fp)) {//获取文件；
        printf("%s\n", data);
        send(data_sock, data, strlen(data), 0);//发送文件；
    }
    fclose(fp);
    return 0;
}

int main(int argc, char **argv) {
    int server_listen, socket_fd, port, pid, data_listen;
    if (argc == 1) {//如果输入值为一个，转入默认端口号；
        port = 8888;
    } else {
        port = atoi(argv[1]);//如果输入值为一个，转入默认端口号；
    }

    server_listen = socket_create(port);//创建短链接监听；
    data_listen = socket_create(port + 1);;//创建短链接监听；
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);
    if ((socket_fd = accept(server_listen, (struct sockaddr*)&client_addr, &len)) < 0) {
        printf("accept error\n");
    }//等待长连接；

    char buff_peer[64] = {0};
    inet_ntop(AF_INET, (void *)&client_addr.sin_addr, buff_peer, 63);//把对端的IP地址存在buff_peer中；

    for (int i = 0; i < 6; i++){
        int retcode = 100 + i, socket_data;//文件标识码，等待短连接返回值；
        send_response(socket_fd, retcode);//发送文件状态码；
        pid = fork();
        if (pid == 0) {
            if ((socket_data = accept(data_listen, (struct sockaddr*)&client_addr, &len)) < 0)
                break;
            recv_response(socket_fd);//接收可以发送文件的信息；
            send_log(socket_data, retcode, buff_peer);//发送文件；
            close(socket_data);//关闭；
            exit(0);//退出；
        } else {
            wait(NULL);//等待子进程结束；
            recv_response(socket_fd);//接收确认信息，确认对方已经接收上一份文件，可以发送下一份文件;
        }
    }
    close(socket_fd);
    close(server_listen);//关闭短链接监听；
    close(data_listen);//关闭长链接监听；
    return 0;

}
