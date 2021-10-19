//
// Created by 62622 on 2021/10/19.
//

#ifndef OS_COPY_H
#define OS_COPY_H
#include "cat.h"
#include "header.h"
#include "mf.h"
#include <sstream>

void copy(vector<string> args) {
    if (args.size() == 1) {
        cout << "Error: Undefined instruction\n";
        return;

    } else if (args.size() == 2) {
        string content;
        //复制
        if (args[0][0] == '@') { //系统文件
            string from = args[0].substr(1);
            fstream hostfile(from, ios::in | ios::out);
            if (!hostfile.is_open()) {
                cout << "Error: cna't find the path\n";
                return;
            }
            stringstream ss;
            ss << hostfile.rdbuf();
            content = ss.str();
            hostfile.close();
        } else {    //模拟文件
            vector<string> p = {args[0]};
            cat(p, content);
            if(content == "|error|"){
                cout << "Error: cna't find the path\n";
                return;
            }
        }
        //粘贴
        if(args[1][0] == '@'){  //系统文件
            string to = args[1].substr(1);
            fstream hostfile(to, ios::in | ios::out);
            if (!hostfile.is_open()) {
                cout << "Error: cna't find the path\n";
                return;
            }
            hostfile << content;
            hostfile.close();
        }
        else{   //模拟文件
            vector<string> p ={args[1],content};
            mf(p);
        }
    } else {
        cout << "Error: Undefined instruction\n";
        return;
    }
}

#endif//OS_COPY_H
