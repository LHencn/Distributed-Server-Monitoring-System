int buffer[BUFFER_SIZE];/*实际存放数据的数组*/
pthread_mutex_t lock;/*互斥体lock，用于对缓冲区的互斥操作*/
int readpos,writepos; /*读写指针*/
pthread_cond_t notempty;/*缓冲区非空的条件变量*/
pthread_cond_t notfull;/*缓冲区未满 的条件变量*/
