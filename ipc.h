//
// Created by 62622 on 2021/10/21.
//

#ifndef OS_IPC_H
#define OS_IPC_H

#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <sys/shm.h>
#include <string>

struct share {
    int written;    //1表示可读，0表示可写
    char content[10000];    //传输的文本
};


#endif //OS_IPC_H
