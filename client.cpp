//
// Created by 62622 on 2021/10/21.
//

#include "ipc.h"
#include <sstream>
#include <cstring>
#include <string>
#include <iostream>

using namespace std;

int main(){
    int running = 1; //程序是否继续运行
    //创建传入后端的共享内存
    void *shmin = nullptr;
    struct share *in = nullptr;
    string input;
    int shmidin;
    shmidin = shmget((key_t)1234, sizeof(struct share), 0666|IPC_CREAT);
    shmin = shmat(shmidin,(void*)0,0);
    in = (struct share*)shmin;
    //创建传入前端的共享内存
    void *shmout = nullptr;
    struct share *out = nullptr;
    string output;
    int shmidout;
    shmidout = shmget((key_t)12345, sizeof(struct share), 0666|IPC_CREAT);
    shmout = shmat(shmidout,(void*)0,0);
    out = (struct share*)shmout;
    out->written = 0;   //将out设为可写
    while(running){
        if(out->written){   //如果out可读
            output = out->content;
            cout << output;
            out->written = 0; //将out设为可写
        }
        else{
            sleep(1);
            continue;
        }
        if(in->written == 0){   //如果in可写
            getline(cin,input);
            strcpy(in->content,input.c_str());
            in->written = 0;    //将in设置为可读
            if(input == "exit"||input == "EXIT"){
                running = 0;
            }
        }
        else{
            sleep(1);
            continue;
        }
    }
    //删除共享内存
    shmctl(shmidin,IPC_RMID,0);
    shmctl(shmidout,IPC_RMID,0);
    return 0;
}
