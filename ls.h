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
            for(int i = 0;i < inode[nowinodenum].blocknum;i++){
                Dir temp{};
                disk.seekg((87+inode[nowinodenum].blocknode[i]) * 1024);
                disk.read((char*)&temp,sizeof(temp));
                for(int j =0;j<temp.num;j ++){
                    tempdir.push_back(temp.dir[j]);
                }
            }
            for (auto &i : tempdir) {//输出当前目录的内容
                cout << i.filename;
                cout << '\n';
            }
            break;
        };
        case 1: {//ls 指定目录
            string path = args[0] + "/.";
            int tempinodeindex;
            vector<Dirnode> tempdir;
            path = pathdivide(path,tempinodeindex);
            if(path == "|error|"){    //找不到路径
                cout << "Error: can't find the dir\n";
                return;
            }
            for(int i = 0;i < inode[tempinodeindex].blocknum;i++){
                Dir temp{};
                disk.seekg((87+inode[tempinodeindex].blocknode[i]) * 1024);
                disk.read((char*)&temp,sizeof(temp));
                for(int j =0;j<temp.num;j ++){
                    tempdir.push_back(temp.dir[j]);
                }
            }
            for (auto &i : tempdir) {//输出当前目录的内容
                cout << i.filename;
                cout << '\n';
            }
            break;
        };
        case 2: {
            break;
        };
    };
    ;
}

#endif//OS_LS_H
