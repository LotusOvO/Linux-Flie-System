//
// Created by 62622 on 2021/10/7.
//

#ifndef OS_BLOCKINODE_H
#define OS_BLOCKINODE_H

#include "header.h"
#include <fstream>
using namespace std;

void readsuperblock() { //读入超级块
    disk.seekg(0);
    disk.read((char *) &superblock.freeblocknum, sizeof(superblock.freeblocknum));
    disk.read((char *) &superblock.freeinodenum, sizeof(superblock.freeinodenum));
    //    outss <<superblock.blocknum << superblock.inodenum;
}

void readbitmap() { //读入位图
    disk.seekg(1024);
    for (auto &i : freeblock) {
        disk.read((char *) &i, sizeof(i));
    }
}

void readinode() {  //读入i节点
    disk.seekg(14 * 1024);
    for (auto &i : inode) {
        disk.read((char *) &i, sizeof(i));
    }
}

void readrootdir() {    //设置为根目录
//    nowdir.clear();
//    for (int i = 0; i < inode[0].blocknum; i++) {
//        disk.seekg(87 * 1024 + inode[0].blocknode[i] * 1024);
//        Dir temp{};
//        disk.read((char *) &temp, sizeof(temp));
//        for (int j = 0; j < temp.num; j++) {
//            nowdir.push_back(temp.dir[j]);
//        }
//    }
    nowinodenum = 0;
    nowdirname += "simdisk";
}

int findfreeinode() {   //申请一个空闲i节点
    if (superblock.freeinodenum == 0) {//没有空闲i节点
        return -1;
    }
    for (int i = 0; i < 1024; i++) {
        if (inode[i].blocknum == 0) {
            //            outss << "newinode: " << i << '\n';
            superblock.freeinodenum -= 1;
            return i;
        }
    }
    return -1;
};

int findfreeblock() {   //申请一个空闲数据块
    if (superblock.freeblocknum <= 0) {//没有空闲数据块
        return -1;
    }
    for (int i = 0; i < 3200; i++) {
        int temp = freeblock[i];
        for (int j = 0; j < 32; j++) {
            if ((temp & 1) == 1) {
                int newblock = i * 32 + j;
                //                outss << "newblock: " << newblock << '\n';
                freeblock[i] -= pow2(j);
                superblock.freeblocknum -= 1;
                return i * 32 + j;
            }
            temp = temp >> 1;
        }
    }
    return -1;
};

void delblock(int index) {  //释放数据块
    freeblock[index / 32] += pow2(index % 32);
    superblock.freeblocknum ++;
}

void delinode(int index) {  //释放i节点
    superblock.freeinodenum ++;
    vector<Dirnode> t;
    if (inode[index].mode[0] == '0') {//目录
        for (int i = 0; i < inode[index].blocknum; i++) {
            Dir temp{};
            disk.seekg((87 + inode[index].blocknode[i]) * 1024);
            disk.read((char *) &temp, sizeof(temp));
            for (int j = 0; j < temp.num; j++) {
                t.push_back(temp.dir[j]);
            }
        }
        inode[index].blocknum = 0;
        for (int i = 2; i < t.size(); i++) {
            delinode(t[i].inodeindex);
        }
    } else {//文件
        for (int i = 0; i < inode[index].blocknum; i++) {
            delblock(inode[index].blocknode[i]);
        }
        inode[index].blocknum = 0;
    }
}

void save(){
    //保存超级块
    disk.seekp(0);
    disk.write((char *) &superblock.freeblocknum, sizeof(superblock.freeblocknum));
    disk.write((char *) &superblock.freeinodenum, sizeof(superblock.freeinodenum));
    //保存位图
    disk.seekp(1024);
    for (auto &i : freeblock) {
        disk.write((char *) &i, sizeof(i));
    }
    //保存i节点
    disk.seekp(14 * 1024);
    for (auto &i : inode) {
        disk.write((char *) &i, sizeof(i));
    }
}

#endif//OS_BLOCKINODE_H

// 超级块（1k）| 数据块位图（13K）| i节点表（73k）| 根目录块（1k）| 数据块（102312k）