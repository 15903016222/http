#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <pthread.h>
#include <errno.h>
#include "http.h"

//线程数据
typedef struct __threadData__ {
    int num;
    int fd;
    pthread_t tid;
} threadData, *pThreadData;

int count = 0;

//处理用户的连接的线程方法
void *handle(void *arg){
    pThreadData pData = (pThreadData *)arg;

    printf ("%s[%d], count[%d] \n", __func__, __LINE__, count++);

EXIT:
    close(pData->fd);
    free(pData);
    return NULL;
}

int main(int argc,char *argv[]){
    struct sockaddr_in client_addr;     //获取客户机信息
    socklen_t length = sizeof(client_addr); //类型大小
    pthread_t *thread = NULL;       //处理事务线程
    int conn,err=0;
    int httpServerFd = socket(AF_INET,SOCK_STREAM,0);
    int   opt   =   1;
    struct sockaddr_in server_sockaddr;
    threadData *temp = NULL;

    setsockopt(httpServerFd,   SOL_SOCKET,   SO_REUSEADDR, &opt, sizeof(int));
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(atoi(argv[1]));
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(httpServerFd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1){
        printf("绑定失败!\n");
        return 0;
    }

    //listen，成功返回0，出错返回-1
    if(listen(httpServerFd,10) == -1){
        printf("监听失败!\n");
        return 0;
    }

    printf ("多线程测试开始...\n");

    while(1){
        //成功返回非负描述字，出错返回-1
        conn = accept(httpServerFd, (struct sockaddr*)&client_addr, &length);
        if(conn==-1){
            printf("客户机连接出错!\n");
            return 0;
        }
        temp = (threadData *)malloc(sizeof(threadData));
        if (NULL == temp) {
            printf ("malloc 错误. \n");
            exit(-1);
        }

        temp->fd = conn;

        err = pthread_create(&temp->tid,NULL,handle,(void *)temp);
        if(err != 0 ){
            printf("create thread fail, err[%d], errno[%d]\n", err, errno);
            strerror(errno);
            exit(0);
        }
    }

}
