/*************************************************************************
	> File Name: master.c
	> Author: LHC 
	> Mail: 3115629644@qq.com 
	> Created Time: 2018年11月24日 星期六 20时11分57秒
 ************************************************************************/

#include"common.c"

int master_get(int data_sock, int reqcode, char *client) {//套接字，文件类型标识码，slave端IP地址；
    int size;
    char data_dir[50] = "/tmp";
    char data[MAX_SIZE] = {0};//data存放接收文件数据；
    char arg[MAX_SIZE] = {0};//arg存放完整路径名；
    char logdir[100] = {0};//logdir存放路径名；
    strcpy(logdir, data_dir);//路径名前面加'/tmp';
    //if (logdir[sizeof(logdir - 1) != '/'])
    strcat(logdir, "/");
    strcat(logdir, client);//加上slave端的IP地址是为了了解哪个树莓派上发来的信息;
    mkdir(logdir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);//创建以logdir命名的文件；
   /*int mkdir(const char *psth, mode_t mode)
        path 目录名
        mode 用户权限
    S_IRWXU 00700权限，代表该文件所有者拥有读，写和执行操作的权限
    S_IRUSR(S_IREAD) 00400权限，代表该文件所有者拥有可读的权限
    S_IWUSR(S_IWRITE) 00200权限，代表该文件所有者拥有可写的权限
    S_IXUSR(S_IEXEC) 00100权限，代表该文件所有者拥有执行的权限
    S_IRWXG 00070权限，代表该文件用户组拥有读，写和执行操作的权限
    S_IRGRP 00040权限，代表该文件用户组拥有可读的权限
    S_IWGRP 00020权限，代表该文件用户组拥有可写的权限
    S_IXGRP 00010权限，代表该文件用户组拥有执行的权限
    S_IRWXO 00007权限，代表其他用户拥有读，写和执行操作的权限
    S_IROTH 00004权限，代表其他用户拥有可读的权限
    S_IWOTH 00002权限，代表其他用户拥有可写的权限
    S_IXOTH 00001权限，代表其他用户拥有执行的权限*/
    generate_name(reqcode, arg, logdir);//将路径名加上'/cpu.log'
    printf("arg: %s\n", arg);

    FILE *fd = fopen(arg, "a+");//文件指针指向arg文件;
    int times = 0;
    while ((size = recv(data_sock, data, MAX_SIZE, 0)) > 0) {
        printf("%d data: %s\n", ++times, data);
        fwrite(data, 1, size, fd);
    /*fwrite()函数以二进制方式向文件流中写入数据，其原型为：
    size_t fwrite(void * buffer, size_t size, size_t count, FILE * stream);
    buffer为数据源地址，size为每个单元的字节数，count为单元个数，stream为文件流指针。
    fwrite()函数每次向stream中写入count个单元，每个单元大小为size个字节；文件流的位置指针后移size * count 字节;
    返回成功写入的单元个数。如果小于count，则说明发生了错误，文件流错误标志位将被设置，
    随后可以通过ferror()函数判断。
        */
        memset(data, 0, sizeof(data));//data初始化，刷新缓冲区；
    }
    fclose(fd);
    return 0;
}


int main(int argc, char **argv) {//argc参数总个数，argv参数值;
    int socket_fd, port;//建立连接的返回值，端口号；
    char *ip_addr;//ip地址；
    if (argc == 1) {//如果参数只有一个，那么就用默认地址和默认端口;
        ip_addr = "192.168.1.74";
        port = 8888;
    } else if (argc == 3){//参数有三个，argv[1]是地址，argv[2]是端口号；
        ip_addr = argv[1];
        port = atoi(argv[2]);//字符串转换成数字；
    } else {
        printf("Use : ./master ip_addr port\n");//表示输入错误；
    }

    socket_fd = socket_connect(port, ip_addr);
    if (socket_fd < 0) {
        printf("socket_fd error\n");
    }
    while (1) {
        int retcode, pid;
        if ((retcode = recv_response(socket_fd)) <= 0) {
            break;
        }
        printf("retcode: %d\n", retcode);
        pid = fork();
        if (pid == 0) {//pid = 0 是子进程，!= 0 是父进程；
            int socket_data = socket_connect(port + 1, ip_addr);//短链接;
            if (socket_data < 0) {
                printf("socket_data error\n");
                exit(1);
            }
            send_response(socket_fd, 200);//发送确认码，可以开始发送信息了；
            printf("send_response\n");
            master_get(socket_data, retcode, ip_addr);//接收文件信息；
            close(socket_data);
            exit(0);//退出之后，子进程结束；
        } else {
            wait(NULL);
            send_response(socket_fd, 201);
             //本次文件接收结束，可以进行下次连接（子进程连接，发下一次文件）；
        }
    }
    return 0;
}
