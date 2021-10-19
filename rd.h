//
// Created by 62622 on 2021/10/18.
//

#ifndef OS_RD_H
#define OS_RD_H
#include "blockinode.h"
#include "header.h"
#include <cstring>

void rd(vector<string> args = {}) {
    if (args.size() == 1) {
        int tempinodeindex;
        string path = pathdivide(args[0], tempinodeindex);
        if (path == "|error|") {//找不到路径
            cout << "Error: can't find the dir\n";
            return;
        }
        if (path == "." || path == "..") {//无法删除的路径
            cout << "Error: Can't delete this dir\n";
            return;
        }
        vector<Dirnode> tempdir;
        //读取目录
        for (int i = 0; i < inode[tempinodeindex].blocknum; i++) {
            Dir temp{};
            disk.seekg((87 + inode[tempinodeindex].blocknode[i]) * 1024);
            disk.read((char *) &temp, sizeof(temp));
            for (int j = 0; j < temp.num; j++) {
                tempdir.push_back(temp.dir[j]);
            }
        }
        int dirindex = 0;
        for (int i = 2; i < tempdir.size(); i++) {
            if (path == tempdir[i].filename) {
                //判断是否是目录
                if (inode[tempdir[i].inodeindex].mode[0] == '0') {
                    dirindex = i;
                } else {
                    cout << "Error: This is not a dir\n";
                    return;
                }
            }
        }
        //目录非空时做二次判定
        Dir tt{};
        disk.seekg((87 + inode[tempdir[dirindex].inodeindex].blocknode[0]) * 1024);
        disk.read((char *) &tt, sizeof(tt));
        if (tt.num > 2) {
            cout << "Warning: The dir is not empty, type \'y\' to continue\n";
            string s;
            getline(cin , s);
            if (s != "y" && s != "Y") {
                cout << "Cancel operation\n";
                return;
            }
        }
        delinode(tempdir[dirindex].inodeindex);
        tempdir.erase(tempdir.begin() + dirindex);
        int bn = 0;//新的块数量
        //保存新的目录信息到数据块
        for (int i = 0; i < tempdir.size() / 40; i++) {
            Dir t{};
            bn++;
            t.num = 40;
            for (int j = 0; j < 40; j++) {
                strcpy(t.dir[j].filename, tempdir[j + i * 40].filename);
                t.dir[j].inodeindex = tempdir[j + i * 40].inodeindex;
            }
            disk.seekp((87 + inode[tempinodeindex].blocknode[i]) * 1024);
            disk.write((char *) &t, sizeof(t));
        }
        int lastnum = tempdir.size() % 40;
        if (lastnum) {
            Dir t{};
            t.num = lastnum;
            for (int i = 0; i < lastnum; i++) {
                strcpy(t.dir[i].filename, tempdir[i + bn * 40].filename);
                t.dir[i].inodeindex = tempdir[i + bn * 40].inodeindex;
            }
            disk.seekp((87 + inode[tempinodeindex].blocknode[bn]) * 1024);
            disk.write((char *) &t, sizeof(t));
            bn++;
        }
        if (bn == inode[tempinodeindex].blocknum - 1) {
            inode[tempinodeindex].blocknum -= 1;
            delblock(inode[tempinodeindex].blocknode[bn]);
        }
        //保存inode数据
        disk.seekp(14 * 1024 + 72 * tempinodeindex);
        disk.write((char *) &inode[tempinodeindex], sizeof(inode[tempinodeindex]));
    } else {
        cout << "Error: Undefined instruction\n";
        return;
    }
}

#endif//OS_RD_H
