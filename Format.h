//
// Created by 62622 on 2021/9/14.
//

#ifndef OS_FORMAT_H
#define OS_FORMAT_H

#include<iostream>
#include<fstream>
#include<cstdio>
#include "header.h"
using namespace std;

streampos getFileSize(const char* strFileName)
{
    ifstream in(strFileName);
    if (!in.is_open()) return 0;
    in.seekg(0, ios::end);
    streampos sp = in.tellg();
    return sp;
}

void format(){
    //判断已分配磁盘块是否为规定大小
    if(getFileSize(FileName) == (100 << 20)){
        return;
    }
    cout << "----------系统正在初始化----------" <<endl;
    ofstream f;
    f.open(FileName,ios::binary);
    cout << f.is_open() <<endl;
    //写入100M的数据,开辟磁盘空间
    for(long long i =0;i<(100 << 20);i++){
        f.write(static_cast<const char *>(" "), 1);
    }
    f.close();
}

#endif //OS_FORMAT_H
