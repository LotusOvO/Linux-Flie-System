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

streampos getFileSize(const char *strFileName) {
    ifstream in(strFileName);
    if (!in.is_open()) return 0;
    in.seekg(0, ios::end);
    streampos sp = in.tellg();
    return sp;
}

void format() {
    //判断已分配磁盘块是否为规定大小
    cout << "----------系统正在初始化----------" << endl;
    if (getFileSize(FileName) != (100 << 20)) {
        ofstream f;
        f.open(FileName, ios::binary);
        cout << f.is_open() << endl;
        //写入100M的数据,开辟磁盘空间
        for (long long i = 0; i < (100 << 20); i++) {
            f.write(static_cast<const char *>(" "), 1);
        }
        f.close();
    }
    disk.open(FileName, ios::in | ios::out | ios::binary);
    //初始化超级块
    superblock.freeinodenum = 1023;
    superblock.freeblocknum = 102312;
    disk.seekp(0);
    disk.write((char *) &superblock, sizeof(superblock));
    //初始化位图
    for (auto &i: freeblock) {
        i = pow2(32) - 1;
    }
    freeblock[0] -= 1;
    disk.seekp(1024);
    disk.write((char *) freeblock, sizeof(freeblock));
    //初始化i节点
    for (auto &i: inode) {
        i.blocknum = 0;
    }
    inode[0].blocknum = 1;
    inode[0].blocknode[0] = 0;
    strcpy(inode[0].time, gettime().c_str());
    strcpy(inode[0].owner, "root");
    strcpy(inode[0].mode, "0111111");
    disk.seekp(14*1024);
    disk.write((char*)inode,sizeof(inode));
    //初始化根目录
    Dir t{};
    t.num = 2;
    strcpy(t.dir[0].filename,"..");
    t.dir[0].inodeindex = 0;
    strcpy(t.dir[1].filename,".");
    t.dir[1].inodeindex = 0;
    disk.seekp(87 * 1024);
    disk.write((char*)&t,sizeof(t));
    disk.close();
}

// 超级块（1k）| 数据块位图（13K）| i节点表（73k）| 根目录块（1k）| 数据块（102312k）
#endif //OS_FORMAT_H
