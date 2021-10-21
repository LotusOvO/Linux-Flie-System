//
// Created by 62622 on 2021/10/17.
//

#ifndef OS_MD_H
#define OS_MD_H
#include "header.h"
#include <cmath>
#include <cstring>

void md(vector<string> args = {}) {
    if (args.size() == 1) {
        int tempinodeindex; //当前目录i节点
        string path = pathdivide(args[0], tempinodeindex);
        if (path == "|error|") {//找不到路径
            outss << "Error: can't find the dir\n";
            return;
        }
        Dir temp{}; //当前目录
        for (int i = 0; i < inode[tempinodeindex].blocknum; i++) {//判断是否重名
            disk.seekg((87 + inode[tempinodeindex].blocknode[i]) * 1024);
            disk.read((char *) &temp, sizeof(temp));
            for (int j = 0; j < temp.num; j++) {
                if (temp.dir[j].filename == path) {
                    outss << "Error: This is the same name\n";
                    return;
                }
            }
        }
        int isnew = 0;  //是否为新的数据块
        if (temp.num == 40) {//数据块存满，新增数据块
            isnew = 1;
            if (inode[tempinodeindex].blocknum == 10) {
                outss << "Error: can't new more file\n";
                return;
            }
            int newblock = findfreeblock();
            inode[tempinodeindex].blocknode[inode[tempinodeindex].blocknum] = newblock;
            inode[tempinodeindex].blocknum += 1;
        }
        //添加新的目录，为目录寻找空闲的i节点
        int newinode = findfreeinode();
        int n = inode[tempinodeindex].blocknum;
        disk.seekg((87 + inode[tempinodeindex].blocknode[n - 1]) * 1024);
        disk.read((char *) &temp, sizeof(temp));
        if(isnew) temp.num = 0;
        strcpy(temp.dir[temp.num].filename, path.c_str());
        temp.dir[temp.num].inodeindex = newinode;
        temp.num += 1;
        //保存目录信息
        disk.seekp((87 + inode[tempinodeindex].blocknode[n - 1]) * 1024);
        disk.write((char *) &temp, sizeof(temp));
        //根据父目录填写子目录属性
        inode[newinode].fsize = 0;
        strcpy(inode[newinode].mode, inode[tempinodeindex].mode);
        strcpy(inode[newinode].owner, inode[tempinodeindex].owner);
        strcpy(inode[newinode].time, gettime().c_str());
        inode[newinode].blocknum = 1;
        //分配新的数据块
        inode[newinode].blocknode[0] = findfreeblock();
        //保存inode数据
        disk.seekp(14 * 1024 + 72 * newinode);
        disk.write((char *) &inode[newinode], sizeof(inode[newinode]));
        //初始化目录
        Dir t{};
        t.num = 2;
        strcpy(t.dir[0].filename, "..");
        t.dir[0].inodeindex = tempinodeindex;
        strcpy(t.dir[1].filename, ".");
        t.dir[1].inodeindex = newinode;
        //保存目录到数据块
        disk.seekp((87 + inode[newinode].blocknode[0]) * 1024);
        disk.write((char *) &t, sizeof(t));
        return;
    }
    outss << "Error: Undefined instruction\n";
}


#endif//OS_MD_H
