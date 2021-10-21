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


int main() {
    inss.clear();
    outss.clear();
    disk.open(FileName, ios::in | ios::out | ios::binary);
    if (!disk.is_open()) {
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
    shmidin = shmget((key_t) 1234, sizeof(struct share), 0666 | IPC_CREAT);
    shmin = shmat(shmidin, nullptr, 0);
    in = (struct share *) shmin;
    //创建传入前端的共享内存
    void *shmout = nullptr;
    struct share *out = nullptr;
    string output;
    int shmidout;
    shmidout = shmget((key_t) 12345, sizeof(struct share), 0666 | IPC_CREAT);
    shmout = shmat(shmidout, nullptr, 0);
    out = (struct share *) shmout;
    in->written = 0;    //将in设置为可写
    out->written = 0;
    int turn = 0;
    while (running) {
//        cout << out->written <<"   " << in->written <<'\n';
            while (out->written) {
                sleep(1);
            }
            //如果out可写
            //cout << "server is writing\n";
        memset(shmout,0, sizeof(struct share));
            output = outss.str();
            strcpy(out->content, output.c_str());
            outss.str("");
//            sleep(3);
            out->written = 1;   //将out设置为可读
            while(!in->written){
                sleep(1);
            }
            //cout << "server is reading\n";
            //如果in可读
                string temp(in->content);
          //      cout << temp<<endl;
          //      cout << in->content << endl;
                inss.clear();
                inss.str(temp);
          //      cout << temp[0]<<' ' << temp[1]<<endl;
                in->written = 0;    //将in设置为可写

                turn = 0;
        string order;
        inss >> order;
        //cout << order <<endl;
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
        inss.str("");
    }
    //删除共享内存
    shmctl(shmidin, IPC_RMID, 0);
    shmctl(shmidout, IPC_RMID, 0);
    return 0;
}