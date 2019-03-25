/*************************************************************************
	> File Name: common.c
	> Author: LHC 
	> Mail: 3115629644@qq.com 
	> Created Time: 2018年10月03日 星期三 19时08分07秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int get_conf_value(char *pathname, char *key_name, char *value) {
    FILE *fp = fopen (pathname, "r");
    char *a =NULL;
    size_t len =0;
    ssize_t read;
    int length = strlen(key_name);
    while ((read = getline(&a, &len, fp)) != -1) {
        if (strncmp(a, key_name, length) == 0) 
        {
            for (int i = 0; i < strlen(a); i++) {
                if (a[i] == '=')
                {
                    strcpy(value, a + i + 1);
                }
    
            }
        }
    }
    return 0;
}

int write_pi_log (char *PiHealthLog, const char *format, ...) {
    va_list arg;
    int done;
    FILE *pFile = fopen(PiHealthLog, "a+");
    va_start(arg, format);
    time_t time_log = time(NULL);
    struct tm* tm_log = localtime(&time_log);
    fprintf(pFile, "%04d-%02d-%02d %02d:%02d ", tm_log->tm_year + 1900,\
            tm_log->tm_mon + 1, tm_log->tm_mday, tm_log->tm_hour, tm_log->tm_min,\
           |tm_log->tm_sec);
    
    done = vfprintf(pFile, format, arg);
    va_end(arg);
    fflush(pFile);
    fclose(pFile);
    return done;
}
int main() {
    char *pathname = "test.conf";
    char *key_name = "finish";
    char value[10];
    get_conf_value(pathname, key_name, value);
    printf("%s", value);
    return 0;
}

