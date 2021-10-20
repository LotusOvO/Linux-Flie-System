//
// Created by 62622 on 2021/10/19.
//

#ifndef OS_CAT_H
#define OS_CAT_H
#include "header.h"

void cat(vector<string> args,string &cont) {
    if (args.size() == 1) {
        int tempinodeindex; //当前目录i节点
        string path = pathdivide(args[0], tempinodeindex);
        if (path == "|error|") {//找不到路径
            cont = "|error|";
            cout << "Error: can't find the dir\n";
            return;
        }
        Dir temp{};
        int fileinode = -1; //文件i节点
        for (int i = 0; i < inode[tempinodeindex].blocknum; i++) {//寻找该文件
            disk.seekg((87 + inode[tempinodeindex].blocknode[i]) * 1024);
            disk.read((char *) &temp, sizeof(temp));
            for (int j = 0; j < temp.num; j++) {
                if (temp.dir[j].filename == path) {
                    fileinode = temp.dir[j].inodeindex;
                    break;
                }
            }
        }
        if (fileinode == -1) {
            cont = "|error|";
            cout << "Error: Can't find this file\n";
            return;
        }
        if (inode[fileinode].mode[0] == '0') {
            cont = "|error|";
            cout << "Error: This is not a file\n";
            return;
        }
        for (int i = 0; i < inode[fileinode].blocknum; i++) {
            char c[1000];
            disk.seekg((87 + inode[fileinode].blocknode[i]) * 1024);
            disk.read(c, 1000);
            cont.append(c);
        }
//        cout << '\n';
        return;
    } else {
        cont = "|error|";
        cout << "Error: Undefined instruction\n";
        return;
    }
}

#endif//OS_CAT_H
