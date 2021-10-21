//
// Created by 62622 on 2021/10/16.
//

#ifndef OS_LS_H
#define OS_LS_H

#include "header.h"
#include <cstring>
#include <vector>

using namespace std;

void ls(vector<string> args = {}) {
    int argnum = args.size();
    switch (argnum) {
        case 0: {                   //单独ls指令
            vector<Dirnode> tempdir;
            for (int i = 0; i < inode[nowinodenum].blocknum; i++) {
                Dir temp{};
                disk.seekg((87 + inode[nowinodenum].blocknode[i]) * 1024);
                disk.read((char *) &temp, sizeof(temp));
                for (int j = 0; j < temp.num; j++) {
                    tempdir.push_back(temp.dir[j]);
                }
            }
            for (int i = 2; i < tempdir.size(); i++) {//输出当前目录的内容
                outss << tempdir[i].filename;
                outss << '\n';
            }
            break;
        };
        case 1: {
            if (args[0] == "-a") {    //输出所有目录
                vector<Dirnode> tempdir;
                for (int i = 0; i < inode[nowinodenum].blocknum; i++) {
                    Dir temp{};
                    disk.seekg((87 + inode[nowinodenum].blocknode[i]) * 1024);
                    disk.read((char *) &temp, sizeof(temp));
                    for (int j = 0; j < temp.num; j++) {
                        tempdir.push_back(temp.dir[j]);
                    }
                }
                for (auto &i: tempdir) {//输出当前目录的内容(包括.和..)
                    outss << i.filename;
                    outss << '\n';
                }
                break;
            } else if (args[0] == "-l") {   //输出信息
                vector<Dirnode> tempdir;
                for (int i = 0; i < inode[nowinodenum].blocknum; i++) {
                    Dir temp{};
                    disk.seekg((87 + inode[nowinodenum].blocknode[i]) * 1024);
                    disk.read((char *) &temp, sizeof(temp));
                    for (int j = 0; j < temp.num; j++) {
                        tempdir.push_back(temp.dir[j]);
                    }
                }
                char c[8];
                strcpy(c, "dwrxwrx");
                for (int i = 2; i < tempdir.size(); i++) {//输出当前目录的内容
                    outss << ((inode[tempdir[i].inodeindex].mode[0] == '1') ? 'f' : 'd');
                    for (int j = 1; j < 6; j++) {
                        if (inode[tempdir[i].inodeindex].mode[j] == '1') {
                            outss << c[j];
                        } else {
                            outss << '-';
                        }
                    }
                    outss << "  " << inode[tempdir[i].inodeindex].owner;
                    outss << "  " << strtime(inode[tempdir[i].inodeindex].time);
                    outss << "  " << tempdir[i].filename;
                    outss << '\n';
                }
                break;
            } else if(args[0] == "-la"){
                vector<Dirnode> tempdir;
                for (int i = 0; i < inode[nowinodenum].blocknum; i++) {
                    Dir temp{};
                    disk.seekg((87 + inode[nowinodenum].blocknode[i]) * 1024);
                    disk.read((char *) &temp, sizeof(temp));
                    for (int j = 0; j < temp.num; j++) {
                        tempdir.push_back(temp.dir[j]);
                    }
                }
                char c[8];
                strcpy(c, "dwrxwrx");
                for (int i = 0; i < tempdir.size(); i++) {//输出当前目录的内容
                    outss << ((inode[tempdir[i].inodeindex].mode[0] == '1') ? 'f' : 'd');
                    for (int j = 1; j < 6; j++) {
                        if (inode[tempdir[i].inodeindex].mode[j] == '1') {
                            outss << c[j];
                        } else {
                            outss << '-';
                        }
                    }
                    outss << "  " << inode[tempdir[i].inodeindex].owner;
                    outss << "  " << strtime(inode[tempdir[i].inodeindex].time);
                    outss << "  " << tempdir[i].filename;
                    outss << '\n';
                }
                break;
            }else{//ls 指定目录
                string path = args[0] + "/.";
                int tempinodeindex;
                vector<Dirnode> tempdir;
                path = pathdivide(path, tempinodeindex);
                if (path == "|error|") {    //找不到路径
                    outss << "Error: can't find the dir\n";
                    return;
                }
                for (int i = 0; i < inode[tempinodeindex].blocknum; i++) {
                    Dir temp{};
                    disk.seekg((87 + inode[tempinodeindex].blocknode[i]) * 1024);
                    disk.read((char *) &temp, sizeof(temp));
                    for (int j = 0; j < temp.num; j++) {
                        tempdir.push_back(temp.dir[j]);
                    }
                }
                for (int i = 2; i < tempdir.size(); i++) {//输出当前目录的内容
                    outss << tempdir[i].filename;
                    outss << '\n';
                }
//            for (auto &i : tempdir) {//输出当前目录的内容
//                outss << i.filename;
//                outss << '\n';
//            }
                break;
            }
        };
        case 2: {
            if (args[1] == "-a") {
                string path = args[0] + "/.";
                int tempinodeindex;
                vector<Dirnode> tempdir;
                path = pathdivide(path, tempinodeindex);
                if (path == "|error|") {    //找不到路径
                    outss << "Error: can't find the dir\n";
                    return;
                }
                for (int i = 0; i < inode[tempinodeindex].blocknum; i++) {
                    Dir temp{};
                    disk.seekg((87 + inode[tempinodeindex].blocknode[i]) * 1024);
                    disk.read((char *) &temp, sizeof(temp));
                    for (int j = 0; j < temp.num; j++) {
                        tempdir.push_back(temp.dir[j]);
                    }
                }
                for (auto &i: tempdir) {//输出当前目录的内容(包括.和..)
                    outss << i.filename;
                    outss << '\n';
                }
            } else if (args[1] == "-l") {
                string path = args[0] + "/.";
                int tempinodeindex;
                vector<Dirnode> tempdir;
                path = pathdivide(path, tempinodeindex);
                if (path == "|error|") {    //找不到路径
                    outss << "Error: can't find the dir\n";
                    return;
                }
                for (int i = 0; i < inode[tempinodeindex].blocknum; i++) {
                    Dir temp{};
                    disk.seekg((87 + inode[tempinodeindex].blocknode[i]) * 1024);
                    disk.read((char *) &temp, sizeof(temp));
                    for (int j = 0; j < temp.num; j++) {
                        tempdir.push_back(temp.dir[j]);
                    }
                }
                char c[8];
                strcpy(c, "dwrxwrx");
                for (int i = 2; i < tempdir.size(); i++) {//输出当前目录的内容
                    outss << ((inode[tempdir[i].inodeindex].mode[0] == '1') ? 'f' : 'd');
                    for (int j = 1; j < 6; j++) {
                        if (inode[tempdir[i].inodeindex].mode[j] == '1') {
                            outss << c[j];
                        } else {
                            outss << '-';
                        }
                    }
                    outss << "  " << inode[tempdir[i].inodeindex].owner;
                    outss << "  " << strtime(inode[tempdir[i].inodeindex].time);
                    outss << "  " << tempdir[i].filename;
                    outss << '\n';
                }
            } else if (args[1] == "-la") {
                string path = args[0] + "/.";
                int tempinodeindex;
                vector<Dirnode> tempdir;
                path = pathdivide(path, tempinodeindex);
                if (path == "|error|") {    //找不到路径
                    outss << "Error: can't find the dir\n";
                    return;
                }
                for (int i = 0; i < inode[tempinodeindex].blocknum; i++) {
                    Dir temp{};
                    disk.seekg((87 + inode[tempinodeindex].blocknode[i]) * 1024);
                    disk.read((char *) &temp, sizeof(temp));
                    for (int j = 0; j < temp.num; j++) {
                        tempdir.push_back(temp.dir[j]);
                    }
                }
                char c[8];
                strcpy(c, "dwrxwrx");
                for (int i = 0; i < tempdir.size(); i++) {//输出当前目录的内容
                    outss << ((inode[tempdir[i].inodeindex].mode[0] == '1') ? 'f' : 'd');
                    for (int j = 1; j < 6; j++) {
                        if (inode[tempdir[i].inodeindex].mode[j] == '1') {
                            outss << c[j];
                        } else {
                            outss << '-';
                        }
                    }
                    outss << "  " << inode[tempdir[i].inodeindex].owner;
                    outss << "  " << strtime(inode[tempdir[i].inodeindex].time);
                    outss << "  " << tempdir[i].filename;
                    outss << '\n';
                }
            }
            break;
        };
        default:{
            outss << "Error: Undefined instruction\n";
            return;
        }
    };
}

#endif//OS_LS_H
