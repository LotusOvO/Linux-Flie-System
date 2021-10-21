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
        outss << "Error: Undefined instruction\n";
        return;

    } else if (args.size() == 2) {
        string content; //文件内容
        //复制
        if (args[0][0] == '@') { //读取系统文件
            string from = args[0].substr(1);
            fstream hostfile(from, ios::in | ios::out);
            if (!hostfile.is_open()) {
                outss << "Error: can't find the path\n";
                return;
            }
            stringstream ss;
            ss << hostfile.rdbuf();
            content = ss.str();
            hostfile.close();
        } else {    //读取模拟文件
            vector<string> p = {args[0]};
            cat(p, content);
            if(content == "|error|"){
                outss << "Error: can't find the path\n";
                return;
            }
        }
        //粘贴
        if(args[1][0] == '@'){  //新建系统文件
            string to = args[1].substr(1);
            fstream hostfile(to, ios::in | ios::out);
            if (!hostfile.is_open()) {
                outss << "Error: can't find the path\n";
                return;
            }
            hostfile << content;
            hostfile.close();
        }
        else{   //新建模拟文件
            vector<string> p ={args[1],content};
            mf(p);
        }
    } else {
        outss << "Error: Undefined instruction\n";
        return;
    }
}

#endif//OS_COPY_H
