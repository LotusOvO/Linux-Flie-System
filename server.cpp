//
// Created by 62622 on 2021/10/21.
//

#include "ipc.h"
#include "Format.h"
#include "blockinode.h"
#include "cat.h"
#include "cd.h"
#include "copy.h"
#include "del.h"
#include "header.h"
#include "ls.h"
#include "md.h"
#include "mf.h"
#include "rd.h"
#include "info.h"
#include "check.h"
#include <cstring>
#include <iostream>
#include <sstream>

using namespace std;


int main(){
    stringstream inss;
    stringstream outss;
    inss.clear();
    outss.clear();
    disk.open(FileName, ios::in | ios::out | ios::binary);
    if(!disk.is_open()){
        outss << "Disk not found!\n";
        return 0;
    }
    readsuperblock();
    readbitmap();
    readinode();
    readrootdir();
    outss << nowdirname << ">";
    int running = 1; //程序是否继续运行
    //创建传入后端的共享内存
    void *shmin = nullptr;
    struct share *in = nullptr;
    string input;
    int shmidin;
    shmidin = shmget((key_t)1234, sizeof(struct share), 0666|IPC_CREAT);
    shmin = shmat(shmidin,nullptr,0);
    in = (struct share*)shmin;
    //创建传入前端的共享内存
    void *shmout = nullptr;
    struct share *out = nullptr;
    string output;
    int shmidout;
    shmidout = shmget((key_t)12345, sizeof(struct share), 0666|IPC_CREAT);
    shmout = shmat(shmidout,nullptr,0);
    out = (struct share*)shmout;
    in->written = 0;    //将in设置为可写
    while(running){
        if(out->written == 0){  //如果out可写
            output = outss.str();
            outss.clear();
            strcpy(out->content,output.c_str());
            out->written = 1;   //将out设置为可读
        }
        else{
            sleep(1);
            continue;
        }
        if(in->written){    //如果in可读
            string temp = in->content;
            inss.str(temp);
            in->written = 0;    //将in设置为可写
        }
        else{
            sleep(1);
            continue;
        }
        string order;
        inss >> order;
        if (order == "exit" || order == "EXIT") {
            save();
            disk.close();
            running = 0;
        }
        if (order == "ls") {
            vector<string> temp;
            string s;
            while (inss >> s) {
                temp.push_back(s);
            }
            ls(temp);
            goto End;
        }
        if (order == "md") {
            vector<string> temp;
            string s;
            while (inss >> s) {
                temp.push_back(s);
            }
            md(temp);
            goto End;
        }
        if (order == "cd") {
            vector<string> temp;
            string s;
            while (inss >> s) {
                temp.push_back(s);
            }
            cd(temp);
            goto End;
        }
        if (order == "rd") {
            vector<string> temp;
            string s;
            while (inss >> s) {
                temp.push_back(s);
            }
            rd(temp);
            goto End;
        }
        if (order == "mf") {
            vector<string> temp;
            string s;
            inss >> s;
            temp.push_back(s);
            string content;
            string t;
            while (inss >> t) {
                content.append(t + " ");
            }
            content = content.substr(0, content.length() - 1);
            temp.push_back(content);
            mf(temp);
            goto End;
        }
        if (order == "cat") {
            vector<string> temp;
            string s;
            string content;
            while (inss >> s) {
                temp.push_back(s);
            }
            cat(temp, content);
            outss << content << '\n';
            goto End;
        }
        if (order == "del") {
            vector<string> temp;
            string s;
            while (inss >> s) {
                temp.push_back(s);
            }
            del(temp);
            goto End;
        }
        if (order == "copy") {
            vector<string> temp;
            string s;
            while (inss >> s) {
                temp.push_back(s);
            }
            copy(temp);
            goto End;
        }
        if (order == "info") {
            vector<string> temp;
            string s;
            while (inss >> s) {
                temp.push_back(s);
            }
            info(temp);
            goto End;
        }
        if (order == "check") {
            vector<string> temp;
            string s;
            while (inss >> s) {
                temp.push_back(s);
            }
            check(temp);
            goto End;
        }

        outss << "Error: Undefined instruction\n";
        End:
        outss << nowdirname << ">";
        inss.clear();
    }
    return 0;
}